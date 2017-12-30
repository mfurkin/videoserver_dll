/*
 * LumaWriter.cpp
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#include "LumaWriter.h"

#include <winbase.h>
#include <winnt.h>
std::string LumaWriter::LUMAWRITER_TAG = "LumaWriter";
LumaWriter::LumaWriter(uint8_t* aSourcePtr, uint8_t* aDestPtr, unsigned aSize, unsigned short aDelta, unsigned short anOffset,
					   HANDLE aCompleted, int aFlag, LoggerEngine* aLoggerPtr): source_ptr(aSourcePtr), dest_ptr(aDestPtr),size(aSize),delta(aDelta),offset(anOffset), completed(aCompleted), flag(aFlag), logger_ptr(aLoggerPtr) {
}

unsigned __attribute__((__stdcall__)) LumaWriter::writeLuma(void* p) {
	LumaWriter* ptr = (LumaWriter*) p;
	return (*ptr).writeThisLuma();
}

unsigned LumaWriter::writeThisLuma() {
	log("writeThisLuma enter");
	if (flag) {
	unsigned i;
		uint8_t* d_ptr,*s_ptr;
		d_ptr = dest_ptr+offset;
		s_ptr = source_ptr;
		for (i=0;i<size;) {
			logPtr("writeThisLuma pt1 i=",i);
			logPtr("writeThisLuma pt2",(unsigned) d_ptr);
			*d_ptr = 0;
			logPtr("writeThisLuma pt3 i=",i);
			*d_ptr = *s_ptr++;
			logPtr("writeThisLuma pt4 i=",i++);
			d_ptr+=delta;
		}
	}
	SetEvent(completed);
	log("writeThisLuma exit");
	return 0;
}

LumaWriter::~LumaWriter() {
	CloseHandle(completed);
}

void LumaWriter::log(std::string msg) {
	if (logger_ptr)
		(*logger_ptr).log(LUMAWRITER_TAG,msg);
}

void LumaWriter::logPtr(std::string msg, unsigned ptr) {
	if (logger_ptr)
		(*logger_ptr).logPtr(LUMAWRITER_TAG,msg,ptr);
}

void LumaWriter::logString(std::string msg, std::string& msg2) {
	if (logger_ptr)
		(*logger_ptr).logString(LUMAWRITER_TAG,msg,msg2);
}
