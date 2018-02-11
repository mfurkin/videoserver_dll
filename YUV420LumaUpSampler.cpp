/*
 * YUV420LumaUpSampler.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "YUV420LumaUpSampler.h"

YUV420LumaUpSampler::YUV420LumaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow):LumaUpSampler(aSource,aWidth,aHeight,aRow,1) {

}

YUV420LumaUpSampler::~YUV420LumaUpSampler() {
}

int YUV420LumaUpSampler::getIndex(int anIndex) {
	return anIndex;
}
