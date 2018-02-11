/*
 * ChromaWriter.cpp
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#include "ChromaWriter.h"

ChromaWriter::ChromaWriter(uint8_t* aSourcePtr,uint8_t* aDestPtr, unsigned short aWidth, unsigned short aHeight,unsigned short anOffset,
						   HANDLE aCompleted, std::string aColorName,int aFlag, LoggerEngine* aLoggerPtr):source_ptr(aSourcePtr),
						   dest_ptr(aDestPtr),width(aWidth),height(aHeight),offset(anOffset),colorName(aColorName),completed(aCompleted),
						   flag(aFlag),logger_ptr(aLoggerPtr) {
	tag = std::string("ChromaWriter::").append(colorName);
}

ChromaWriter::~ChromaWriter() {
	CloseHandle(completed);
}

unsigned __attribute__((__stdcall__)) ChromaWriter::writeChroma(void* p) {
	ChromaWriter* ptr = (ChromaWriter*) p;
	return (*ptr).writeThisChroma();
}

unsigned ChromaWriter::writeThisChroma() {
	logString("writeThisChroma enter ",colorName);
	if (flag) {
		unsigned short i,j;
		unsigned width2 = width >> 1,  height1 = height - 1;
		uint8_t* d_ptr = dest_ptr+offset;

		for(i=0;i++<height1;)
			for (j=0;j++<width2;)
				writeChromaSample(d_ptr,getUpSamplingChroma(i,j));
		for (j=0;j++<width2;)
			writeChromaSample(d_ptr,getUpSamplingLastLine(j));
	}
	SetEvent(completed);
	logString("writeThisChroma exit ",colorName);
	return 0;
}

unsigned short ChromaWriter::getChromaticDelta() {
	return 4;
}

void ChromaWriter::writeChromaSample(uint8_t*& ptr, uint8_t data) {
	*ptr = data;
	ptr += getChromaticDelta();
}

uint8_t ChromaWriter::getUpSamplingChroma(unsigned short row, unsigned short col) {
	uint8_t result;
	unsigned short k = row >> 1;
	result =  (row & 0x1) ? (9*(*(source_ptr+k*width+col)+*(source_ptr+(k+1)*width+col)) - (*(source_ptr+(k-1)*width+col)+*(source_ptr+(k+2)*width+col))+8) >> 4 :
						*(source_ptr+k*width+col);
	return clip8(result);
}

uint8_t ChromaWriter::clip8(unsigned short data) {
	return (data > 255) ? 255 : data;
}

void ChromaWriter::log(std::string msg) {
	if (logger_ptr)
		(*logger_ptr).log(tag,msg);
}

void ChromaWriter::logPtr(std::string msg, unsigned ptr) {
	if (logger_ptr)
		(*logger_ptr).logPtr(tag,msg,ptr);
}

void ChromaWriter::logString(std::string msg,	std::string& msg2) {
	if (logger_ptr)
		(*logger_ptr).logString(tag, msg, msg2);
}

uint8_t ChromaWriter::getUpSamplingLastLine(unsigned short col) {
	uint8_t result;
	unsigned short height21 = (height >> 1) - 1;
	result = (9*(*(source_ptr+height21*width+col)+*(source_ptr+height21*width+col)) - (*(source_ptr+(height21-1)*width+col)+*(source_ptr+height21*width+col))+8) >> 4 ;
	return clip8(result);
}
