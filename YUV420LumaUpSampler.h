/*
 * YUV420LumaUpSampler.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef YUV420LUMAUPSAMPLER_H_
#define YUV420LUMAUPSAMPLER_H_

#include "LumaUpSampler.h"

class YUV420LumaUpSampler: public LumaUpSampler {
public:
	YUV420LumaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow);
	virtual ~YUV420LumaUpSampler();
private:
	int getIndex(int anIndex);
};

#endif /* YUV420LUMAUPSAMPLER_H_ */
