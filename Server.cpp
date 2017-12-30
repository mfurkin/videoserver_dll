/*
 * server.cpp
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */

#include "Server.h"
std::string Server::SERVER_LOG_NAME = "Server";
std::string Server::SERVER_CTOR_TAG = "Server::ctor";
std::string Server::SERVER_CREATE_EVENT_TAG = "Server::createEvent";
std::string Server::SERVER_OUTPUTMSG_TAG="Server::outputMsg";
std::string Server::SERVER_CREATE_MUTEX_TAG="Server::createMutex";
std::string Server::SERVER_CONTROL_SYNCHRO_TAG="Server::controlSynchro";
std::string Server::SERVER_CHECK_SYNCHRO_TAG="Server::checkSynchro";
std::string Server::SERVER_OUTPUT_WAIT_RESULT_TAG="Server::outputWaitResult";
std::string Server::SERVER_MAIN_THREAD_TAG="Server_main_thread";
std::string Server::SERVER_LOG_DIR_NAME="\\videoserver";
std::string Server::SERVER_LOG_FILE_NAME="\\videoserver.log";
Server::Server():LoggerEngine() {
	size_t size = MAX_PATH*MAX_REQ_NUM;
	inited = FALSE;
	initLogger();
	fileInput = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,size,REQ_FILE_NAME.c_str());
	if (fileInput == NULL)
		logPtr(SERVER_CTOR_TAG,"File mapping could not be created error code=",GetLastError());
	else {
			buf = (LPSTR)MapViewOfFile(fileInput,FILE_MAP_ALL_ACCESS,0,0,size);
			memset(buf,0,size);
			if (buf == NULL)
				logPtr(SERVER_CTOR_TAG,"Shared memory could not be created error code=",GetLastError());
			else {
				log(SERVER_CTOR_TAG,"Server ctor pt1");
					inited = createEvent(&reqFlag,FALSE,FALSE,REQ_FLAG_NAME,REQ_FLAG_CREATING_MSG) &&
							 createEvent(&endFlag,FALSE,FALSE,END_FLAG_NAME,END_FLAG_CREATING_MSG) &&
							 createEvent(&reqEnabledFlag,FALSE,TRUE,REQ_ENABLED_FLAG,REQ_ENABLED_FLAG_CREATING_MSG) &&
							 createMutex(&requestMutex,FALSE,REQ_MUTEX_NAME,REQ_MUTEX_CREATING_MSG);
					logPtr(SERVER_CTOR_TAG,"Server ctor pt2  reqFlag=",(unsigned)reqFlag);
					logPtr(SERVER_CTOR_TAG,"Server ctor pt2 endFlag=",(unsigned)endFlag);
					logPtr(SERVER_CTOR_TAG,"Server ctor pt2 reqEnabledFlag=",(unsigned)reqEnabledFlag);
					logPtr(SERVER_CTOR_TAG,"Server ctor pt2 requestMutex=",(unsigned)requestMutex);
					initFileUtils();
					initLogger();
					if (inited) {
						Request::setEndFlag(endFlag);
						Request::initConverters();
						Request::setServerPtr(this);
						log(SERVER_CTOR_TAG,"Server have been run successfully");
					}
			}
	}
}

BOOL Server::createEvent(HANDLE* event_ptr, BOOL manualReset, BOOL initialState, const std::string& name, const std::string msg) {
	BOOL result;
	logString(SERVER_CREATE_EVENT_TAG, "enter name=",(std::string&)name);
	*event_ptr = CreateEvent(NULL, manualReset, initialState, name.c_str());
	result = (*event_ptr != NULL);
	logString(SERVER_CREATE_EVENT_TAG,"exit name=",(std::string&)name);
	logPtr(SERVER_CREATE_EVENT_TAG,"exit result=",result);
	outputMsg(msg,result);
	logString(SERVER_CREATE_EVENT_TAG," pt2 name=",(std::string&)name);
	return result;
}

BOOL Server::createMutex(HANDLE* mutex_ptr, BOOL initialOwner, const std::string& name, const std::string& msg) {
	BOOL result;
	logString(SERVER_CREATE_MUTEX_TAG,"enter name=",(std::string&)name);
	*mutex_ptr = CreateMutex(NULL,initialOwner,name.c_str());
	result = (*mutex_ptr != NULL);
	logString(SERVER_CREATE_MUTEX_TAG,"exit name=",(std::string&)name);
	logPtr(SERVER_CREATE_MUTEX_TAG,"exit result=",result);
	outputMsg(msg,result);
	logString(SERVER_CREATE_MUTEX_TAG," pt2 name=",(std::string&)name);
	return result;
}

void Server::controlSynchro(std::string correct, std::string error, unsigned long result) {
	if (result)
		log(SERVER_CONTROL_SYNCHRO_TAG,correct);
	else
		logPtr(SERVER_CONTROL_SYNCHRO_TAG,error,GetLastError());
}

void Server::outputMsg(const std::string& msg, BOOL result) {
	if  (!(result))
		logPtr(SERVER_OUTPUTMSG_TAG,msg,result);
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
	log(SERVER_CHECK_SYNCHRO_TAG,"enter");
	WaitForSingleObjectEx(synchro,INFINITE,TRUE);
	log(SERVER_CHECK_SYNCHRO_TAG,"exit");
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
	logString(SERVER_OUTPUT_WAIT_RESULT_TAG,"enter msg=",msg);
	switch(result) {
		case WAIT_IO_COMPLETION:
			log(SERVER_OUTPUT_WAIT_RESULT_TAG,"result=WAIT_IO_COMPLETION has been got");
			break;
		case WAIT_TIMEOUT:
			log(SERVER_OUTPUT_WAIT_RESULT_TAG,"result=WAIT_TIMEOUT has been got");
			break;
		case WAIT_FAILED:
			logPtr(SERVER_OUTPUT_WAIT_RESULT_TAG,"result is WAIT_FAILED error code=",GetLastError());
			break;
		default:
			if ((result >= WAIT_OBJECT_0) &&(result<obj_max)) {
				unsigned number = result - WAIT_OBJECT_0;
				logPtr(SERVER_OUTPUT_WAIT_RESULT_TAG,"result is WAIT_OBJECT number=",number);
			} else
					if ((result >= WAIT_ABANDONED_0) && (result<abandoned_max)) {
						unsigned number = result - WAIT_ABANDONED_0;
						logPtr(SERVER_OUTPUT_WAIT_RESULT_TAG,"result is WAIT_ABANDONED number=",number);
					}
			break;
	}
}
void Server::initLogger() {
	HMODULE hDll = LoadLibrary("liblogger.dll");
	createLoggerProc = (CreateLoggerProc) GetProcAddress(hDll,"createLogger");
	logProc = (LogProc) GetProcAddress(hDll,"log");
	logPtrProc = (LogPtrProc) GetProcAddress(hDll,"logPtr");
	deleteLoggerProc = (DeleteLoggerProc) GetProcAddress(hDll,"deleteLogger");
	std::string fname = getLogFName(NULL);
	fname.append("\\client.log");
	createLogger(SERVER_LOG_NAME,fname);
}
unsigned Server::runThisServer() {
	unsigned res2 = 0;
	log(SERVER_MAIN_THREAD_TAG,"runThisServer enter");
//	std::wcerr<<"runThisServer enter\n";
	HANDLE events[2];
	events[0] = reqFlag;
	events[1] = endFlag;
	unsigned long result,res3,res4;
	std::string reqName,reqPingName;

	res4 = SetEvent(reqEnabledFlag);
	controlSynchro("ReqEnabledFlag have been set successfully","It was errors during ReqEnabledFlag setting",res4);
	std::cerr<<"Waiting requests..\n";
	for(;(!(result=WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,FALSE)));) {
		outputWaitResult(result,2,"Server::runThisServer in the loop pt1");
		res3 = WaitForSingleObjectEx(requestMutex,INFINITE,TRUE);
		outputWaitResult(res3,1,"Server::runThisServer in the loop pt2");
		if (res3 == WAIT_OBJECT_0) {
			reqName = std::string(buf);
			logString(SERVER_MAIN_THREAD_TAG,"Request has been got name=",reqName);
			logPtr(SERVER_MAIN_THREAD_TAG,"Request has been got name len=",reqName.length());
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

void Server::createLogger(std::string& name, std::string& fname) {
	if  (createLoggerProc)
		createLoggerProc(name.c_str(),fname.c_str());
}

void Server::deleteLogger(std::string& name) {
	if (deleteLoggerProc)
		deleteLoggerProc(name.c_str());
}

void Server::log(std::string& tag, std::string msg) {
	if (logProc)
		logProc(SERVER_LOG_NAME.c_str(),tag.c_str(),msg.c_str());
}

void Server::logPtr(std::string& tag, std::string msg, unsigned ptr) {
	if (logPtrProc)
		logPtrProc(SERVER_LOG_NAME.c_str(),tag.c_str(),msg.c_str(),ptr);
}

void Server::logString(std::string& tag, std::string msg, std::string& msg2) {
	std::string msgAll = msg.append(msg2);
	log(tag,msgAll);
}

std::string Server::getLogFName(LoggerEngine* logger_ptr) {
	std::string result;
	if (!(fileUtilsInited))
		result= "";
	else {
		static char res[MAX_PATH];
		getLogPathProc((char*)res,(char*)SERVER_LOG_DIR_NAME.c_str(),(void*)logger_ptr);
		result = std::string(res);
	}
	return result;
}

void Server::initFileUtils() {
	int result = TRUE;
	HMODULE hDll = LoadLibrary("fileutils.dll");
	if (!(hDll))
		result = FALSE;
	else {
		getLogPathProc  = (GetLogPathProc) GetProcAddress(hDll,"getLogPath");
		getLogFnameProc = (GetLogFnameProc) GetProcAddress(hDll,"getLogFname");
		result = getLogPathProc && getLogFnameProc;
	}
	fileUtilsInited = result;
}
