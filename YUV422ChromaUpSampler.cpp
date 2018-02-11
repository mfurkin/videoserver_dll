/*
 * YUV422ChromaUpSampler.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "YUV422ChromaUpSampler.h"

YUV422ChromaUpSampler::YUV422ChromaUpSampler(uint8_t* source, int width, int height, int row):ChromaUpSampler(source,width,height,row) {
}

YUV422ChromaUpSampler::~YUV422ChromaUpSampler() {
}

uint8_t YUV422ChromaUpSampler::verticalUpSampling(int col) {
//	return source[row*(width << 1) + col];
	return at(getIndexFromRowAndCol422(col));
}
