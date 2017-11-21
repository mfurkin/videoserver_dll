

#include <wtypes.h>
#include <winbase.h>
#include "Server.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD     fdwReason, LPVOID    lpvReserved) {
	BOOL res2 = TRUE;
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			Server::startServer();
			std::wcout<<L"Server have been started successfully\n";
			break;
		case DLL_PROCESS_DETACH:
			Server::stopServer();
			std::wcout<<L"Server have been stopped successfully\n";
			break;
	}
	return res2;
}
