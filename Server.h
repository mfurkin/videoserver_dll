/*
 * server.h
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */

#ifndef SERVER_SERVER_H_

#define SERVER_SERVER_H_

#include <iostream>
#include <Shlobj.h>
#include <string>
#include <process.h>
#include <wtypes.h>
#include <winbase.h>
#include "FileUtils.h"
#include "LoggerTypes.h"
#include "LoggerEngine.h"
#include "server_names.h"
#include "Request.h"
enum {MAX_REQ_NUM=10};


class Server : public  LoggerEngine {
public:
	virtual ~Server();
	static unsigned WINAPI runServer(void* p);
	unsigned runThisServer();
	static Server& getServer();
	void static startServer();
	void static stopServer();
	void stopThisServer();
	void log(std::string& tag,  std::string msg);
	void logPtr(std::string& tag, std::string msg, unsigned ptr);
	void logString(std::string& tag, std::string msg, std::string& msg2);
private:
	Server();
	Server(const Server&) = delete;
	Server& operator =(const Server& ) = delete;
	BOOL inited;
	void controlSynchro(std::string correct, std::string error,  unsigned long result);
	void initFileUtils();
	void initLogger();
	void createLogger(std::string& name, std::string& fname);
	void deleteLogger(std::string& name);
	void checkSynchro(HANDLE synchro);
	void checkMutex(HANDLE mutex);
	void checkEvent(HANDLE event);
	BOOL createEvent(HANDLE* event_ptr, BOOL manualReset, BOOL initialState, const std::string& name, const std::string msg);
	BOOL createMutex(HANDLE* mutex_ptr, BOOL initialOwner, const std::string& name, const std::string& msg);
	void outputMsg(const std::string&msg, BOOL result);
	void outputWaitResult(unsigned result, unsigned num, std::string msg);
	virtual std::string getLogFName(LoggerEngine* logger_ptr);
	CreateLoggerProc createLoggerProc;
	LogProc logProc;
	LogPtrProc logPtrProc;
	DeleteLoggerProc deleteLoggerProc;
	LPSTR buf;
	HANDLE fileInput,reqFlag,endFlag,reqEnabledFlag;
	static std::string 	SERVER_LOG_NAME,SERVER_CTOR_TAG,SERVER_CREATE_EVENT_TAG,SERVER_OUTPUTMSG_TAG,SERVER_CREATE_MUTEX_TAG,
						SERVER_CONTROL_SYNCHRO_TAG,SERVER_CHECK_SYNCHRO_TAG,SERVER_OUTPUT_WAIT_RESULT_TAG,SERVER_MAIN_THREAD_TAG,
						SERVER_LOG_DIR_NAME,SERVER_LOG_FILE_NAME;
	GetLogPathProc getLogPathProc;
	GetLogFnameProc getLogFnameProc;
	HANDLE requestMutex;
	int fileUtilsInited;
};

extern "C" __declspec(dllexport) void  startServer();
extern "C" __declspec(dllexport) void  stopServer();
#endif /* SERVER_SERVER_H_ */
