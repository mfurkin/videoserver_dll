/*
 * Request.h
 *
 *  Created on: 12 ����. 2017 �.
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
		std::string  pingName;													// pingName - ��� shared memory ��� ����� �������
		BOOL inited;															// ������� �������������������
//		HANDLE hMapSource,pingReq,pingNotify,writeCompleted,writeEnabled;		// hSourceFile - �������� ����
		LARGE_INTEGER fileSize;
		HANDLE hMapSource,pingReq,pingNotify,destFileAccess;					// hSourceFile - �������� ����
		unsigned short conv_type,width,height;												// conv_type - ��� ��������������
																				// hPingChannel - shared memory ��� �����
																				// hDestFile - shared memory ��� ���.�����
		uint8_t* pingChannel,*destFile;
		volatile unsigned  progress;
		volatile uint8_t status;
	HANDLE headerDataWritten;
	int in_progress;

		void sendHeaderData(HANDLE* events, int evNum, unsigned frameSize, unsigned framesQty);
		void runThisRequest();
		unsigned pingChannelProc();
};

#endif /* SERVER_REQUEST_H_ */
