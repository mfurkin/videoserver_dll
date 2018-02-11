/*
 * YUV420ChromaUpSampler.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "YUV420ChromaUpSampler.h"



YUV420ChromaUpSampler::YUV420ChromaUpSampler(uint8_t* aSource, int aWidth, int aHeight, int aRow):ChromaUpSampler(aSource,aWidth,aHeight,aRow) {
}

YUV420ChromaUpSampler::~YUV420ChromaUpSampler() {
}

uint8_t YUV420ChromaUpSampler::verticalUpSampling(int col) {
	return YUVUtils::clip8(9*(at(getVerticalIndex(col,0))+at(getVerticalIndex(col,1))) - (at(getVerticalIndex(col,-2))+at(getVerticalIndex(col,2)))+8) >> 4;
}

int YUV420ChromaUpSampler::getVerticalIndex(int col, int delta) {
	return getIndexFromRowAndCol(getRowIndex(getIndexWithOffset(delta)),col);
}
