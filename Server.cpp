/*
 * server.cpp
 *
 *  Created on: 10 нояб. 2017 г.
 *      Author: Михаил
 */
// #include <iostream>

#include "Server.h"
enum {PORT_NUM=49001,QUEUE_SIZE=5};
Server::Server() {


	std::map<int,std::wstring> bindMsgMap = makeBindMsgMap();

	listen_sock = socket(PF_INET,SOCK_STREAM,0);
	if (listen_sock == INVALID_SOCKET)
		std::wcout<<L"socket have not been created successfully.\n";
	else {
		SOCKADDR_IN addr;

		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		addr.sin_port = htons(PORT_NUM);
		addr.sin_family = AF_INET;
		int result = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));
		if (result !=0)
			outputErrorResult(result,bindMsgMap);
		else {
			std::wcout<<L"Socket have been binded successfully\n";
			_beginthreadex(NULL,0,runServer,this,0,NULL);
		}
	}
}

Server::~Server() {
	shutdown(listen_sock,2);
}

void Server::startServer() {

}

void Server::outputErrorResult(int code, std::map<int,std::wstring> msgMap) {

	std::wstring st = (msgMap.count(code)) ? msgMap[code] : L"Unknown issue";
	std::wcout<<st<<'\n';
}

std::map<int, std::wstring> Server::makeBindMsgMap() {
	std::map<int,std::wstring> result = std::map<int,std::wstring>();
	result[WSAEBADF] = std::wstring(L"The socket argument is not a valid file descriptor.");
	result[WSAENOTSOCK] = std::wstring(L"The descriptor socket is not a socket.");
	result[WSAEADDRNOTAVAIL] = std::wstring(L"The specified address is not available on this machine.");
	result[WSAEADDRINUSE] = std::wstring(L"Some other socket is already using the specified address.");
	result[WSAEINVAL] = std::wstring(L"The socket socket already has an address.");
	result[WSAEACCES] = std::wstring(L"You do not have permission to access the requested address.");
	return result;
}

unsigned Server::runServer(void* p) {
	Server* ptr = (Server*) p;
	return (*ptr).runThisServer();
}

unsigned Server::runThisServer() {
	std::map<int,std::wstring> listenMsgMap = makeListenMsgMap();
	int result = listen(listen_sock,QUEUE_SIZE),res2 = 0;
	if (result)
		outputErrorResult(errno,listenMsgMap);
	else {
		std::map<int,std::wstring> acceptMsgMap = makeAcceptMsgMap();
		struct sockaddr addrinfo;
		int addrlen;
		std::wcout<<"Listening completed successfully\n";
		for(;;) {
			SOCKET clSocket  = accept(listen_sock,&addrinfo,&addrlen);
			if (clSocket == INVALID_SOCKET)
				outputErrorResult(errno,acceptMsgMap);
			else{
				Request request = Request(clSocket);
				_beginthreadex(NULL,0,runRequest,&request,0,NULL);
			}
		}
	}
	return res2;
}

std::map<int, std::wstring> Server::makeInitMsgMap() {
	std::map<int,std::wstring> result = std::map<int,std::wstring>();
	result[WSASYSNOTREADY] = std::wstring(L"Init error: The underlying network subsystem is not ready for network communication.");
	result[WSAVERNOTSUPPORTED] = std::wstring(L"Init error: The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
	result[WSAEINPROGRESS] = std::wstring(L"Init error: A blocking Windows Sockets 1.1 operation is in progress.");
	result[WSAEPROCLIM] = std::wstring(L"Init error: A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
	result[WSAEFAULT] = std::wstring(L"Init error: The lpWSAData parameter is not a valid pointer.");
	return result;
}

std::map<int, std::wstring> Server::makeDeinitMsgMap() {
	std::map<int,std::wstring> result = std::map<int,std::wstring>();
	result[WSANOTINITIALISED] = L"Deinit error: A successful WSAStartup call must occur before using this function.";
	result[WSAENETDOWN] = L"The network subsystem has failed.";
	result[WSAEINPROGRESS] = L"A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
	return result;
}

std::map<int, std::wstring> Server::makeListenMsgMap() {
	std::map<int,std::wstring> result = std::map<int,std::wstring>();
	result[WSAEBADF] = L"Listening error: The argument socket is not a valid file descriptor.";
	result[WSAENOTSOCK] = L"Listening error: The argument socket is not a socket.";
	result[WSAEOPNOTSUPP]= L"Listening error: The socket socket does not support this operation.";
	return result;
}

unsigned Server::runRequest(void* p) {
	Request* ptr = (Request*) p;
	(*ptr).runRequest();
	return 0;
}



std::map<int, std::wstring> Server::makeAcceptMsgMap() {
	std::map<int,std::wstring> result = std::map<int,std::wstring>();
	result[WSAEBADF] = L"Accepting error: The socket argument is not a valid file descriptor.";
	result[WSAENOTSOCK] = L"Accepting error: The descriptor socket argument is not a socket.";
	result[WSAEOPNOTSUPP] = L"Accepting error: The descriptor socket does not support this operation.";
	result[WSAEWOULDBLOCK] = L"Accepting error: socket has nonblocking mode set, and there are no pending connections immediately available.";
	return result;
}
