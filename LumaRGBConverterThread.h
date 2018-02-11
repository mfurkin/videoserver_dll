/*
 * LumaRGBConverterThread.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef LUMARGBCONVERTERTHREAD_H_
#define LUMARGBCONVERTERTHREAD_H_

#include "RGBConverterThread.h"
enum {LUMA_COEFF_R = 66,LUMA_COEFF_G = 129, LUMA_COEFF_B = 25, LUMA_ADDING = 16};
class LumaRGBConverterThread: public RGBConverterThread {
public:
	LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, int aDestDelta);
	virtual ~LumaRGBConverterThread();
private:
	int dest_delta;
		int getMaxHeight();
		void incPtrs(uint8_t** dest_ptr, uint8_t** source_ptr);
		void updatePtrs(uint8_t** dptr, uint8_t* dest_ptr, uint8_t** sptr, uint8_t* source_ptr);
};

#endif /* LUMARGBCONVERTERTHREAD_H_ */
