/*
 * YUV420toYUV422Converter.cpp
 *
 *  Created on: 21 нояб. 2017 г.
 *      Author: Алёна
 */

#include "YUV420toYUV422Converter.h"

YUV420toYUV422Converter::YUV420toYUV422Converter() {
}

void YUV420toYUV422Converter::convert(uint8_t* source, uint8_t* dest, unsigned short width, unsigned short height) {
	int size = width*height;
//	int newSize = 2*size;
	unsigned result;
	int thread_count;
	HANDLE LumaWriteCompleted = CreateEvent(NULL,FALSE,FALSE,NULL),events[4],ChromaticRedCompleted = CreateEvent(NULL,FALSE,FALSE,NULL),
		   ChromaticBlueCompleted = CreateEvent(NULL,FALSE,FALSE,NULL);
	LumaWriter* lumaWriter_ptr = new LumaWriter(source,dest,size,getLumaDelta(),getLumaOffset(),LumaWriteCompleted);
	ChromaWriter* chromaBlueWriter_ptr = new ChromaWriter(getBluePtr(source,width,height),dest,width,height,getBlueOffset(),
														  ChromaticBlueCompleted);
	ChromaWriter* chromaRedWriter_ptr = new ChromaWriter(getRedPtr(source,width,height),dest,width,height,getRedOffset(),
			  	  	  	  	  	  	  	  	  	  	  	 ChromaticRedCompleted);
	_beginthreadex(NULL,0,LumaWriter::writeLuma,lumaWriter_ptr,0,NULL);
	_beginthreadex(NULL,0,ChromaWriter::writeChroma,chromaBlueWriter_ptr,0,NULL);
	_beginthreadex(NULL,0,ChromaWriter::writeChroma,chromaRedWriter_ptr,0,NULL);
	thread_count = 3;
	events[0] = LumaWriteCompleted;
	events[1] = ChromaticRedCompleted;
	events[2] = ChromaticBlueCompleted;
	events[3] = endFlag;
	for (;(result = WaitForMultipleObjectsEx(numOfThreads,events,FALSE,INFINITE,TRUE)) && ((result == END_INDEX+WAIT_OBJECT_0) || (--thread_count));){
	}
	delete lumaWriter_ptr;
	delete chromaBlueWriter_ptr;
	delete chromaRedWriter_ptr;
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
	unsigned size = width*height;
	return pic + (((height + 15) & ~15) * size);
}

uint8_t* YUV420toYUV422Converter::getSecondChromaticArea(uint8_t* pic, unsigned short width, unsigned short height) {
	unsigned size = width*height;
	return pic + (((((height * 3) / 2) + 15) & ~15) * size);
}
