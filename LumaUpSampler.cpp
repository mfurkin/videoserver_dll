/*
 * LumaUpSampler.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "LumaUpSampler.h"

LumaUpSampler::LumaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow, int aDelta):UpSampler(aSource,aWidth,aHeight,aRow),delta(aDelta) {
}

LumaUpSampler::~LumaUpSampler() {
}

uint8_t LumaUpSampler::horizontalUpSampling(uint8_t* buf, int index) {
	return *(buf + index);
}

uint8_t LumaUpSampler::verticalUpSampling(int col) {
	return at(getIndex(col));
}

int LumaUpSampler::getLength() {
	return getLumaLength();
//	return width;
}
