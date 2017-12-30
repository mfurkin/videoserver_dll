/*
 * Request.h
 *
 *  Created on: 12 но€б. 2017 г.
 *
 */

#ifndef SERVER_REQUEST_H_
#define SERVER_REQUEST_H_
#include <algorithm>
#include <iostream>
#include <map>
#include <process.h>
#include <stdint.h>
#include <stdlib.h>
#include <wtypes.h>
#include <winbase.h>
#include "YUV420toRGBConverter.h"
#include "YUV420toYUV422Converter.h"
#include "server_names.h"
#include "Server.h"
class Server;
enum {REQ_FLAG_INDEX = 0, END_FLAG_INDEX };

class Request {

	public:
		Request(std::string aPingName);
		static unsigned WINAPI runRequest(void*  p);
		static unsigned WINAPI pingChannelThread(void* p);
		static void initConverters();
		static void deinitConverters();
		static void releaseConverter(std::pair<unsigned short, Converter*> pair);
		static void setEndFlag(HANDLE anEndFlag);
		static void setServerPtr(Server* aServerPtr);
		virtual ~Request();
	private:
		void logPtr(std::string msg, unsigned ptr);
		int waitDataRequest(volatile int& req_rcvd);
		void sendHeaderData(unsigned frameSize, unsigned framesQty);
		void runThisRequest();
		unsigned pingChannelProc();
		static HANDLE endFlag;
		static std::string REQUEST_MAIN_THREAD;
		static std::map<unsigned short,Converter*> converters;
		static Server* server_ptr;
		std::string  pingName;													// pingName - им€ shared memory дл€ пинга сервера
		BOOL inited;															// успешно проинициализировано
//		HANDLE hMapSource,pingReq,pingNotify,writeCompleted,writeEnabled;		// hSourceFile - исходный файл
		LARGE_INTEGER fileSize;
//		HANDLE hMapSource,pingReq,pingNotify,destFileAccess,writeRequest;
		HANDLE hMapSource,pingReq,pingNotify,writeCompleted,writeEnabled;		// hSourceFile - исходный файл
		unsigned short conv_type,width,height;									// conv_type - вид преобразовани€
																				// hPingChannel - shared memory дл€ пинга
																				// hDestFile - shared memory дл€ рез.файла
		uint8_t* pingChannel,*destFile;
		volatile unsigned  progress;
		volatile uint8_t status;
//	HANDLE headerDataWritten;
	int in_progress;
	int timeout_count;


};

#endif /* SERVER_REQUEST_H_ */
