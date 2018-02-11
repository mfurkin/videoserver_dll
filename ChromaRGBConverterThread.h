/*
 * ChromaURGBConverterThread.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef CHROMARGBCONVERTERTHREAD_H_
#define CHROMARGBCONVERTERTHREAD_H_

#include "RGBConverterThread.h"
enum { CHROMA_SOURCE_DELTA=2, CHROMA_U_COEFF_R=-38, CHROMA_U_COEFF_G=-74, CHROMA_U_COEFF_B=112, CHROMA_U_ADDING = 128};
class ChromaRGBConverterThread: public RGBConverterThread {
public:
	ChromaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, uint8_t aCoeffR, uint8_t aCoeffG, uint8_t aCoeffB,
								uint8_t anAdding, int aChromaDestDelta);
	virtual ~ChromaRGBConverterThread();
private:
	int dest_delta;
	int getMaxHeight();
	void incPtrs(uint8_t** dest_ptr, uint8_t** source_ptr);
	void updatePtrs(uint8_t** dptr, uint8_t* dest_ptr, uint8_t** sptr, uint8_t* source_ptr);
};

#endif /* CHROMARGBCONVERTERTHREAD_H_ */
