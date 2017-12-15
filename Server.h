/*
 * server.h
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */

#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_
#include <iostream>
#include <string>
#include <process.h>
#include <wtypes.h>
#include <winbase.h>
#include "server_names.h"
#include "Request.h"
enum {MAX_REQ_NUM=10};


typedef struct {
	uint32_t width, height;
	uint8_t type;
	char fname_in[MAX_PATH],fname_out[MAX_PATH];
}RequestData;

class Server {
public:
	virtual ~Server();
	static unsigned WINAPI runServer(void* p);
	unsigned runThisServer();
	static Server& getServer();
	void static startServer();
	void static stopServer();
	void stopThisServer();
//	static unsigned WINAPI runRequest(void* ptr);
private:
	Server();
	Server(const Server&) = delete;
	Server& operator =(const Server& ) = delete;
	BOOL inited;
	void controlSynchro(std::string correct, std::string error,  unsigned long result);
	static void logPtr(std::string msg,  unsigned ptr);
	void checkSynchro(HANDLE synchro);
	void checkMutex(HANDLE mutex);
	void checkEvent(HANDLE event);
	BOOL createEvent(HANDLE* event_ptr, BOOL manualReset, BOOL initialState, const std::string& name, const std::string msg);
	BOOL createMutex(HANDLE* mutex_ptr, BOOL initialOwner, const std::string& name, const std::string& msg);
	void outputMsg(const std::string&msg, BOOL result);
	void outputWaitResult(unsigned result, unsigned num, std::string msg);
	LPSTR buf;
	HANDLE fileInput,reqFlag,endFlag,reqEnabledFlag;
	HANDLE requestMutex;
};

extern "C" __declspec(dllexport) void  startServer();
extern "C" __declspec(dllexport) void  stopServer();
#endif /* SERVER_SERVER_H_ */
