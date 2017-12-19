/*
 * LumaWriter.h
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#ifndef LUMAWRITER_H_
#define LUMAWRITER_H_
#include <iostream>
#include <stdint.h>
#include <windef.h>
#include <wtypes.h>
#include "Server.h"
class LumaWriter {
public:
	LumaWriter(uint8_t* aSource, uint8_t* aDest, unsigned aSize,unsigned short aDelta, unsigned short anOffset,HANDLE aCompleted, int aFlag = 1);
	static unsigned WINAPI writeLuma(void* ptr);
	unsigned writeThisLuma();
	virtual ~LumaWriter();
private:
	uint8_t* source_ptr,*dest_ptr;
	unsigned size;
	unsigned short delta,offset;
	HANDLE completed;
	int flag;
};

#endif /* LUMAWRITER_H_ */
