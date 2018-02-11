/*
 * YUV422LumaUpSampler.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef YUV422LUMAUPSAMPLER_H_
#define YUV422LUMAUPSAMPLER_H_

#include "LumaUpSampler.h"
enum {YUV422_LUMA_DELTA=2};
class YUV422LumaUpSampler: public LumaUpSampler {
public:
	YUV422LumaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow);
	virtual ~YUV422LumaUpSampler();
private:
	int getIndex(int anIndex);
	virtual int getLumaOffset();
};

#endif /* YUV422LUMAUPSAMPLER_H_ */
