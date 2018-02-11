/*
 * LumaUpSampler.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef LUMAUPSAMPLER_H_
#define LUMAUPSAMPLER_H_

#include "UpSampler.h"

class LumaUpSampler: public UpSampler {
public:
	LumaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow, int aDelta);
	virtual ~LumaUpSampler();
	uint8_t horizontalUpSampling(uint8_t* buf, int index);
	uint8_t verticalUpSampling(int col);
	int getLength();
private:
	virtual int getIndex(int anIndex) = 0;
	int delta;
};

#endif /* LUMAUPSAMPLER_H_ */
