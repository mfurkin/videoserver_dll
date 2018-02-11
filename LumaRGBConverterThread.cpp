/*
 * LumaRGBConverterThread.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "LumaRGBConverterThread.h"

LumaRGBConverterThread::LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, int aDestDelta):RGBConverterThread(aSource,aDest,aWidth,aHeight,LUMA_COEFF_R,LUMA_COEFF_G,LUMA_COEFF_B,LUMA_ADDING),dest_delta(aDestDelta) {
}

LumaRGBConverterThread::~LumaRGBConverterThread() {
}

int LumaRGBConverterThread::getMaxHeight() {
//	return height;
	return getLumaMaxHeight();
}

void LumaRGBConverterThread::incPtrs(uint8_t** dest_ptr, uint8_t** source_ptr) {
	(*source_ptr)++;
	dest_ptr += dest_delta;
}

void LumaRGBConverterThread::updatePtrs(uint8_t** d_ptr, uint8_t* dest_ptr, uint8_t** s_ptr, uint8_t* source_ptr) {
	*s_ptr = source_ptr;
	*d_ptr = dest_ptr;
}
