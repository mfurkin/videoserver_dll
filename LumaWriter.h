/*
 * LumaWriter.h
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#ifndef LUMAWRITER_H_
#define LUMAWRITER_H_
#include <stdint.h>
#include <windef.h>
#include <wtypes.h>
class LumaWriter {
public:
	LumaWriter(uint8_t* aSource, uint8_t* aDest, unsigned aSize,unsigned short aDelta, unsigned short anOffset,HANDLE aCompleted);
	static unsigned WINAPI writeLuma(void* ptr);
	unsigned writeThisLuma();
	virtual ~LumaWriter();
private:
	uint8_t* source_ptr,*dest_ptr;
	unsigned size;
	unsigned short delta,offset;
	HANDLE completed;
};

#endif /* LUMAWRITER_H_ */
