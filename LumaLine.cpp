/*
 * LumaLine.cpp
 *
 *  Created on: 5 ����. 2018 �.
 *
 */

#include "LumaLine.h"

LumaLine::LumaLine(LumaUpSampler* aLumaUpSamplerPtr):Line(aLumaUpSamplerPtr) {
}

LumaLine::~LumaLine() {
}

uint8_t LumaLine::getSample(int index) {
	return at(index);
}

uint8_t LumaLine::horizontalUpSampling(int index) {
	return at(index);
}


