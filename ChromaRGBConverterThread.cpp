/*
 * ChromaURGBConverterThread.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "ChromaRGBConverterThread.h"


ChromaRGBConverterThread::ChromaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, uint8_t aCoeffR,
		uint8_t aCoeffG, uint8_t aCoeffB, uint8_t anAdding, int aDestDelta):RGBConverterThread(aSource,aDest,aWidth,aHeight,
																									 CHROMA_U_COEFF_R,CHROMA_U_COEFF_G,
																									 CHROMA_U_COEFF_B,CHROMA_U_ADDING),
																									 dest_delta(aDestDelta) {
}

ChromaRGBConverterThread::~ChromaRGBConverterThread() {
}


void ChromaRGBConverterThread::incPtrs(uint8_t** dest_ptr,	uint8_t** source_ptr) {
	source_ptr +=  CHROMA_SOURCE_DELTA;
	dest_ptr += dest_delta;
}

void ChromaRGBConverterThread::updatePtrs(uint8_t** d_ptr, uint8_t* dest_ptr, uint8_t** s_ptr, uint8_t* source_ptr) {

}
