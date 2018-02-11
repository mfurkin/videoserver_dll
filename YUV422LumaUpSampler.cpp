/*
 * YUV422LumaUpSampler.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "YUV422LumaUpSampler.h"


YUV422LumaUpSampler::YUV422LumaUpSampler(uint8_t* aSource, int aWidth, int aHeight, int aRow):LumaUpSampler(aSource,aWidth,aHeight,aRow,YUV422_LUMA_DELTA) {
}

YUV422LumaUpSampler::~YUV422LumaUpSampler() {
}

int YUV422LumaUpSampler::getIndex(int anIndex) {
	return (anIndex << 1) + getLumaOffset();
}

int YUV422LumaUpSampler::getLumaOffset() {
	return 0;
}
