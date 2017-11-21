/*
 * LumaWriter.cpp
 *
 *  Created on: 21 нояб. 2017 г.
 *      Author: Алёна
 */

#include "LumaWriter.h"

LumaWriter::LumaWriter(uint8_t* aSourcePtr, uint8_t* aDestPtr, unsigned aSize, unsigned short aDelta, unsigned short anOffset,HANDLE aCompleted): source_ptr(aSourcePtr), dest_ptr(aDestPtr),size(aSize),delta(aDelta),offset(anOffset), completed(aCompleted) {
}

unsigned __attribute__((__stdcall__)) LumaWriter::writeLuma(void* p) {
	LumaWriter* ptr = (LumaWriter*) p;
	return (*ptr).writeThisLuma();
}

unsigned LumaWriter::writeThisLuma() {
	int i;
	uint8_t*d_ptr,*s_ptr;
	d_ptr = dest_ptr+offset;
	for (i=0;i++<size;) {
		*d_ptr = *s_ptr++;
		d_ptr+=delta;
	}
	SetEvent(completed);
	return 0;
}

LumaWriter::~LumaWriter() {
	CloseHandle(completed);
}

