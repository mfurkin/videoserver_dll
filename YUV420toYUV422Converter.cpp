/*
 * YUV420toYUV422Converter.cpp
 *
 *  Created on: 21 ����. 2017 �.
 *
 */

#include "YUV420toYUV422Converter.h"

YUV420toYUV422Converter::YUV420toYUV422Converter(LoggerEngine* aLoggerPtr):Converter(aLoggerPtr) {
}

void YUV420toYUV422Converter::convert(uint8_t* source, uint8_t* dest, unsigned short width, unsigned short height) {
	log("YUV420toYUV422Converter::convert enter");
	int size = width*height;
	unsigned result,i=0;
	int thread_count;
	HANDLE LumaWriteCompleted = CreateEvent(NULL,FALSE,FALSE,NULL),events[3],ChromaticRedCompleted = CreateEvent(NULL,FALSE,FALSE,NULL),
		   ChromaticBlueCompleted = CreateEvent(NULL,FALSE,FALSE,NULL);
	LumaWriter* lumaWriter_ptr = new LumaWriter(source,dest,size,getLumaDelta(),getLumaOffset(),LumaWriteCompleted);
	ChromaWriter* chromaBlueWriter_ptr = new ChromaWriter(getBluePtr(source,width,height),dest,width,height,getBlueOffset(),
														  ChromaticBlueCompleted,"blue");
	ChromaWriter* chromaRedWriter_ptr = new ChromaWriter(getRedPtr(source,width,height),dest,width,height,getRedOffset(),
			  	  	  	  	  	  	  	  	  	  	  	 ChromaticRedCompleted,"red");
	_beginthreadex(NULL,0,LumaWriter::writeLuma,lumaWriter_ptr,0,NULL);
	_beginthreadex(NULL,0,ChromaWriter::writeChroma,chromaBlueWriter_ptr,0,NULL);
	_beginthreadex(NULL,0,ChromaWriter::writeChroma,chromaRedWriter_ptr,0,NULL);
	thread_count = 3;
	events[0] = LumaWriteCompleted;
	events[1] = ChromaticRedCompleted;
	events[2] = ChromaticBlueCompleted;
	log("YUV420toYUV422Converter::convert pt1");
	switch(i) {
		for (;((result != END_INDEX+WAIT_OBJECT_0) && (--thread_count));) {
			case 0:
				logPtr("YUV420toYUV422Converter::convert pt2 thread_count=",thread_count);
				result = waitForEvents(events,NUM_OF_THREADS);
				logPtr("YUV420toYUV422Converter::convert pt3 thread_count=",thread_count);
		}
	}
	log("YUV420toYUV422Converter::convert pt4");
	delete chromaBlueWriter_ptr;
	delete chromaRedWriter_ptr;
	log("YUV420toYUV422Converter::convert exit");
}

YUV420toYUV422Converter::~YUV420toYUV422Converter() {
}

unsigned short YUV420toYUV422Converter::getLumaDelta() {
	return 2;		// Y in YUV422  goes over 2 byte
}

unsigned short YUV420toYUV422Converter::getLumaOffset() {
	return 0;		// one variant Y is first byte. other - Y is second byte
}


unsigned short YUV420toYUV422Converter::getBlueOffset() {
	return 1;
}

uint8_t* YUV420toYUV422Converter::getBluePtr(uint8_t* pic, unsigned short width, unsigned short height) {
	return getSecondChromaticArea(pic,width,height);
}


unsigned short YUV420toYUV422Converter::getRedOffset() {
	return 3;
}

uint8_t* YUV420toYUV422Converter::getRedPtr(uint8_t* pic, unsigned short width,
		unsigned short height) {
	return getFirstChromaticArea(pic,width,height);
}

uint8_t* YUV420toYUV422Converter::getFirstChromaticArea(uint8_t* pic, unsigned short width, unsigned short height) {
	return pic + YUV420format::getFirstChromaticAreaOffset(width,height);
}

unsigned long YUV420toYUV422Converter::getSourceSize(unsigned short width, unsigned short height) {
	return YUV420format::getSecondChromaticAreaOffset(width,height)+width*height;
}

unsigned long YUV420toYUV422Converter::getDestSize(unsigned short aWidth, unsigned short aHeight) {
	return aWidth*aHeight*2;		// �� ��������� ������������� � ����� �������
}


uint8_t* YUV420toYUV422Converter::getSecondChromaticArea(uint8_t* pic, unsigned short width, unsigned short height) {
	return pic + YUV420format::getSecondChromaticAreaOffset(width,height);
}
