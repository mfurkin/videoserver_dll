/*
 * Request.h
 *
 *  Created on: 12 нояб. 2017 г.
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
#include "server_names.h"

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
		virtual ~Request();
	private:
		static HANDLE endFlag;
		static std::map<unsigned short,Converter*> converters;
		std::string  pingName;													// pingName - имя shared memory для пинга сервера
		BOOL inited;															// успешно проинициализировано
		HANDLE hMapSource,pingReq,pingNotify,writeCompleted,writeEnabled;		// hSourceFile - исходный файл
		unsigned short conv_type,width,height;												// conv_type - вид преобразования
																				// hPingChannel - shared memory для пинга
																				// hDestFile - shared memory для рез.файла
		uint8_t* pingChannel,*destFile;
		volatile unsigned  progress;
		volatile uint8_t status;
		void sendHeaderData(HANDLE* events, int evNum, unsigned frameSize, unsigned framesQty);
		void runThisRequest();
		unsigned pingChannelProc();
};

#endif /* SERVER_REQUEST_H_ */
