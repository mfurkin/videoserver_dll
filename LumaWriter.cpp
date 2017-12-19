/*
 * LumaWriter.cpp
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#include "LumaWriter.h"

#include <winbase.h>
#include <winnt.h>

LumaWriter::LumaWriter(uint8_t* aSourcePtr, uint8_t* aDestPtr, unsigned aSize, unsigned short aDelta, unsigned short anOffset,HANDLE aCompleted, int aFlag): source_ptr(aSourcePtr), dest_ptr(aDestPtr),size(aSize),delta(aDelta),offset(anOffset), completed(aCompleted), flag(aFlag) {
}

unsigned __attribute__((__stdcall__)) LumaWriter::writeLuma(void* p) {
	LumaWriter* ptr = (LumaWriter*) p;
	return (*ptr).writeThisLuma();
}

unsigned LumaWriter::writeThisLuma() {
	std::cerr<<"writeThisLuma enter\n";
	if (flag) {
	unsigned i;
		uint8_t*d_ptr,*s_ptr;
		d_ptr = dest_ptr+offset;
		s_ptr = source_ptr;
		for (i=0;i<size;) {
			std::cerr<<"writeThisLuma pt1 i="<<i<<"\n";
			Server::logPtr("writeThisLuma pt2",(unsigned) d_ptr);
			*d_ptr = 0;
			std::cerr<<"writeThisLuma pt3 i="<<i<<"\n";
			*d_ptr = *s_ptr++;
			std::cerr<<"writeThisLuma pt4 i="<<i++<<"\n";
			d_ptr+=delta;
		}
	}
	SetEvent(completed);
	std::cerr<<"writeThisLuma exit\n";
	return 0;
}

LumaWriter::~LumaWriter() {
	CloseHandle(completed);
}

