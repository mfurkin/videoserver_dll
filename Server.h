/*
 * server.h
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */

#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_
#include <iostream>
#include <map>
#include <string>
#include <process.h>
#include <winsock.h>
#include "Request.h"
enum {WSA_OK=0,VERSION_LOW = 2,VERSION_HIGH = 2};
class Server {
public:
	Server();
	virtual ~Server();
	void startServer();
	static unsigned WINAPI runServer(void* p);
	static unsigned WINAPI runRequest(void* ptr);
	static std::map<int,std::wstring> makeInitMsgMap();
	static std::map<int,std::wstring> makeDeinitMsgMap();
	static void outputErrorResult(int code, std::map<int,std::wstring> msgMap);
//	static bool inited;
private:

	unsigned runThisServer();
	std::map<int,std::wstring> makeBindMsgMap();

	std::map<int,std::wstring> makeListenMsgMap();
	std::map<int,std::wstring> makeAcceptMsgMap();
	SOCKET listen_sock;
};
void __declspec(dllexport) startServer();
#endif /* SERVER_SERVER_H_ */
