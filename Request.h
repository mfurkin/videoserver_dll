/*
 * Request.h
 *
 *  Created on: 12 нояб. 2017 г.
 *      Author: Алёна
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
enum CONV_TYPE { YUV420toRGB24 = 0,  YUV422toRGB24, RGB24toYUV420,  RGB24toYUV422, YUV420toYUV422};
enum REQ_STATUS { REQ_RECEIVED = 1, REQ_CONV_IN_PROGRESS, REQ_COMPLETED, REQ_ABORTED};
enum {REQ_FLAG_INDEX = 0, END_FLAG_INDEX };
enum { COLORS_NUM=3 };
#pragma pack(push,1)
typedef struct {
	unsigned short width,height,conv_type;						//	width - ширина исходной картинки,
																//  height - высота исходной картинки,
																// 	conv_type - тип преобразования в соответствии с CONV_TYPE
	char fName[MAX_PATH],respName[MAX_PATH],					// fName - путь к исходному файлу
		 respPingName[MAX_PATH],destName[MAX_PATH],				// respName - имя ивента для оповещения о пинге
		 writeCompleted[MAX_PATH],writeEnabled[MAX_PATH];		// respPingName - имя ивента для ответа на пинг
																// destName - имя shared memory для рез. файла
																// writeEnabled - имя ивента для разрешения записи в рез. shared memory
} RequestDataStruct;
typedef struct {
	unsigned short progress; 			// в процентах
	uint8_t req_status;					// статус в соответствии с REQ_STATUS
}ResponseDataStruct;
#pragma pack(pop)
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
		void runThisRequest();
		unsigned pingChannelProc();
};

#endif /* SERVER_REQUEST_H_ */
