

#include <wtypes.h>
#include <winbase.h>
#include "Server.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD     fdwReason, LPVOID    lpvReserved) {
	BOOL res2 = TRUE;
	int result;
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
		{
			WSADATA wsadata;
			result = WSAStartup(MAKEWORD(VERSION_HIGH,VERSION_LOW),&wsadata);
			std::map<int,std::wstring> initMsgMap = Server::makeInitMsgMap();
			if (result == WSA_OK){
				std::wcout<<L"WSAStartup have been run successfully\n";
				std::wcout<<"version is "<<HIBYTE(wsadata.wVersion)<<"."<<LOBYTE(wsadata.wVersion);
			}
			else {
				res2 = FALSE;
				Server::outputErrorResult(result,initMsgMap);
			}
		}
		break;
		case DLL_PROCESS_DETACH:
		{
			result = WSACleanup();
			std::map<int,std::wstring> deinitMsgMap = Server::makeDeinitMsgMap();
			if (result == WSA_OK)
				std::wcout<<L"WSACleanup have been run successfully\n";
			else {
				res2 = FALSE;
				Server::outputErrorResult(result,deinitMsgMap);
			}
		}
			break;
	}
	return res2;
}
