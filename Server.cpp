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
					reqFlag = CreateEvent(NULL,FALSE,FALSE,REQ_FLAG_NAME.c_str());
					endFlag = CreateEvent(NULL,FALSE,FALSE,NULL);
					reqEnabledFlag = CreateEvent(NULL,FALSE,TRUE,REQ_ENABLED_FLAG.c_str());
					requestMutex = CreateMutex(NULL,TRUE,REQ_MUTEX_NAME.c_str());
					inited = createEvent(reqFlag,FALSE,FALSE,REQ_FLAG_NAME,REQ_FLAG_CREATING_MSG) ||
							 createEvent(endFlag,FALSE,FALSE,NULL,END_FLAG_CREATING_MSG) ||
							 createEvent(reqEnabledFlag,FALSE,TRUE,REQ_ENABLED_FLAG,REQ_ENABLED_FLAG_CREATING_MSG) ||
							 createMutex(requestMutex,TRUE,REQ_MUTEX_NAME,REQ_MUTEX_CREATING_MSG);
					if (inited) {
						Request::setEndFlag(endFlag);
						Request::initConverters();
						std::wcerr<<L"Server have been run successfully\n";
					}
			}
	}
}

BOOL Server::createEvent(HANDLE& event, BOOL manualReset, BOOL initialState, const std::string& name, const std::string msg) {
	BOOL result;
	event = CreateEvent(NULL, manualReset, initialState, name.c_str());
	result = (event != NULL);
	outputMsg(msg,result);
	return result;
}

BOOL Server::createMutex(HANDLE& mutex, BOOL initialOwner, const std::string& name, const std::string& msg) {
	BOOL result;
	mutex = CreateMutex(NULL,TRUE,name.c_str());
	result = (mutex != NULL);
	outputMsg(msg,result);
	return result;
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

unsigned Server::runThisServer() {
	unsigned res2 = 0;
	std::wcerr<<"runThisServer enter\n";
	HANDLE events[2];
	events[0] = reqFlag;
	events[1] = endFlag;
	unsigned long result,res3;
	std::string reqName,reqPingName;
	for(;(result=WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,FALSE)) && (result == WAIT_OBJECT_0);) {
		res3 = WaitForSingleObjectEx(requestMutex,INFINITE,TRUE);
		if (res3 == WAIT_IO_COMPLETION) {
			reqName = std::string(buf);
			Request* req_ptr = new Request(reqName);
			_beginthreadex(NULL,0,Request::runRequest,req_ptr,0,NULL);
			SetEvent(reqEnabledFlag);
			ReleaseMutex(requestMutex);
		}
	}
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

