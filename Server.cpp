/*
 * server.cpp
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */

#include "Server.h"

Server::Server() {
	size_t size = MAX_PATH*MAX_REQ_NUM;
	inited = FALSE;
	fileInput = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,size,REQ_FILE_NAME.c_str());
	if (fileInput == NULL)
		std::wcerr<<L"File mapping could not be created error code="<<GetLastError()<<"\n";
	else {
			buf = (LPSTR)MapViewOfFile(fileInput,FILE_MAP_ALL_ACCESS,0,0,size);
			memset(buf,0,size);
			if (buf == NULL)
				std::wcerr<<L"Shared memory could not be created error code="<<GetLastError()<<"\n";
			else {
				/*
					reqFlag = CreateEvent(NULL,FALSE,FALSE,REQ_FLAG_NAME.c_str());
					logPtr("Server ctor pt1  reqFlag=",(unsigned)reqFlag);
					endFlag = CreateEvent(NULL,FALSE,FALSE,NULL);
					logPtr("Server ctor pt1 endFlag=",(unsigned)endFlag);
					reqEnabledFlag = CreateEvent(NULL,FALSE,TRUE,REQ_ENABLED_FLAG.c_str());
					logPtr("Server ctor pt1 reqEnabledFlag=",(unsigned)reqEnabledFlag);
					requestMutex = CreateMutex(NULL,TRUE,REQ_MUTEX_NAME.c_str());
					logPtr("Server ctor pt1 requestMutex=",(unsigned)requestMutex);
					*/
				std::cerr<<"Server ctor pt1\n";
					inited = createEvent(&reqFlag,FALSE,FALSE,REQ_FLAG_NAME,REQ_FLAG_CREATING_MSG) &&
							 createEvent(&endFlag,FALSE,FALSE,END_FLAG_NAME,END_FLAG_CREATING_MSG) &&
							 createEvent(&reqEnabledFlag,FALSE,TRUE,REQ_ENABLED_FLAG,REQ_ENABLED_FLAG_CREATING_MSG) &&
							 createMutex(&requestMutex,FALSE,REQ_MUTEX_NAME,REQ_MUTEX_CREATING_MSG);
//							 createMutex(requestMutex,TRUE,REQ_MUTEX_NAME,REQ_MUTEX_CREATING_MSG);
					logPtr("Server ctor pt2  reqFlag=",(unsigned)reqFlag);
					logPtr("Server ctor pt2 endFlag=",(unsigned)endFlag);
					logPtr("Server ctor pt2 reqEnabledFlag=",(unsigned)reqEnabledFlag);
					logPtr("Server ctor pt2 requestMutex=",(unsigned)requestMutex);
					if (inited) {
						Request::setEndFlag(endFlag);
						Request::initConverters();
						std::wcerr<<L"Server have been run successfully\n";
					}
			}
	}
}

BOOL Server::createEvent(HANDLE* event_ptr, BOOL manualReset, BOOL initialState, const std::string& name, const std::string msg) {
	BOOL result;
	std::cerr<<"createEvent enter name="<<name<<"\n";
	*event_ptr = CreateEvent(NULL, manualReset, initialState, name.c_str());
	result = (*event_ptr != NULL);
	std::cerr<<"createEvent exit name="<<name<<" result="<<result<<"\n";
	outputMsg(msg,result);
	std::cerr<<"createEvent  pt2 name="<<name<<"\n";
	return result;
}

BOOL Server::createMutex(HANDLE* mutex_ptr, BOOL initialOwner, const std::string& name, const std::string& msg) {
	BOOL result;
	std::cerr<<"createMutex enter name="<<name<<"\n";
	*mutex_ptr = CreateMutex(NULL,initialOwner,name.c_str());
	result = (*mutex_ptr != NULL);
	std::cerr<<"createMutex exit name="<<name<<" result="<<result<<"\n";
	outputMsg(msg,result);
	std::cerr<<"createMutex  pt2 name="<<name<<"\n";
	return result;
}

void Server::controlSynchro(std::string correct, std::string error, unsigned long result) {
	if (result)
		std::cerr<<correct<<"\n";
	else
		std::cerr<<error<<" error code = "<<GetLastError()<<"\n";
}

void Server::logPtr(std::string msg, unsigned ptr) {
	char st[MAX_PATH];
	sprintf(st,"%s ptr:%0x\n",msg.c_str(),ptr);
	std::cerr<<st;
}

void Server::outputMsg(const std::string& msg, BOOL result) {
	if  (!(result))
		std::cerr<<msg.c_str()<<" error code = "<<GetLastError()<<"\n";
}

Server::~Server() {
	CloseHandle(fileInput);
	CloseHandle(reqFlag);
	CloseHandle(endFlag);
	CloseHandle(reqEnabledFlag);
	CloseHandle(requestMutex);
}

unsigned Server::runServer(void* p) {
	Server* ptr = (Server*) p;
	return (*ptr).runThisServer();
}
void Server::checkSynchro(HANDLE synchro) {
	std::cerr<<"checkSynchro enter\n";
	WaitForSingleObjectEx(synchro,INFINITE,TRUE);
	std::cerr<<"checkSynchro exit\n";

}

void Server::checkMutex(HANDLE mutex) {
	checkSynchro(mutex);
	ReleaseMutex(mutex);
}

void Server::checkEvent(HANDLE event) {
	checkSynchro(event);
	SetEvent(event);
}
void Server::outputWaitResult(unsigned result, unsigned num, std::string msg) {
	unsigned obj_max= WAIT_OBJECT_0+num, abandoned_max = WAIT_ABANDONED_0+num;
	std::cerr<<"outputWautResult enter "<<msg<<" result="<<result<<"\n";
	switch(result) {
		case WAIT_IO_COMPLETION:
			std::cerr<<"result=WAIT_IO_COMPLETION has been got\n";
			break;
		case WAIT_TIMEOUT:
			std::cerr<<"result=WAIT_TIMEOUT has been got\n";
			break;
		case WAIT_FAILED:
			std::cerr<<"result is WAIT_FAILED error code="<<GetLastError()<<"\n";
			break;
		default:
			if ((result >= WAIT_OBJECT_0) &&(result<obj_max)) {
				unsigned number = result - WAIT_OBJECT_0;
				std::cerr<<"result is WAIT_OBJECT number="<<number<<"\n";
			} else
					if ((result >= WAIT_ABANDONED_0) && (result<abandoned_max)) {
						unsigned number = result - WAIT_ABANDONED_0;
						std::cerr<<"result is WAIT_ABANDONED number="<<number<<"\n";
					}
			break;
	}
}

unsigned Server::runThisServer() {
	unsigned res2 = 0;
	std::wcerr<<"runThisServer enter\n";
	HANDLE events[2];
//	HANDLE mutex= CreateMutex(NULL,FALSE,NULL);
	events[0] = reqFlag;
	events[1] = endFlag;
	unsigned long result,res3,res4;
	//,res5;
	std::string reqName,reqPingName;

	res4 = SetEvent(reqEnabledFlag);
	controlSynchro("ReqEnabledFlag have been set successfully","It was errors during ReqEnabledFlag setting",res4);
//	checkEvent(reqEnabledFlag);
//	checkMutex(mutex);
//	std::cerr<<"some mutex checked. Request mutex to check..\n";
/*
	checkMutex(requestMutex);
/
	res5 = ReleaseMutex(requestMutex);
	controlSynchro("ReqMutex have been released successfully","it was errors during ReqMutex releasing",res5);
*/
// WAIT_OBJECT_0 = 0, поэтому отрицание в условии
//	for(;(!(result=WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,FALSE))) && (result == WAIT_OBJECT_0);) {
	std::cerr<<"Waiting requests..\n";
	for(;(!(result=WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,FALSE)));) {
		outputWaitResult(result,2,"Server::runThisServer in the loop pt1");
		res3 = WaitForSingleObjectEx(requestMutex,INFINITE,TRUE);
		outputWaitResult(res3,1,"Server::runThisServer in the loop pt2");
		if (res3 == WAIT_OBJECT_0) {
			reqName = std::string(buf);
			std::cerr<<"Request has been got name="<<reqName<<" length="<<reqName.length()<<"\n";
			Request* req_ptr = new Request(reqName);
			_beginthreadex(NULL,0,Request::runRequest,req_ptr,0,NULL);
			std::cerr<<"Request has been started\n";
			SetEvent(reqEnabledFlag);
			ReleaseMutex(requestMutex);
			std::cerr<<"Request started, waiting new request\n";
		}
	}
	outputWaitResult(result,2,"runThisServer after loop");
	std::wcerr<<"runThisServer exit\n";
	return res2;
}

void Server::startServer() {
	Server& server = getServer();
	_beginthreadex(NULL,0,Server::runServer,&server,0,NULL);
}

void Server::stopServer() {
	getServer().stopThisServer();
}

Server& Server::getServer() {
	static Server server;
	return server;
}

void Server::stopThisServer() {
	SetEvent(endFlag);
}

void __attribute__((dllexport)) stopServer() {
	Server::stopServer();
}

void __attribute__((dllexport)) startServer() {
	Server::startServer();
}

