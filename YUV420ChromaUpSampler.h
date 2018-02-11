/*
 * YUV420ChromaUpSampler.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef YUV420CHROMAUPSAMPLER_H_
#define YUV420CHROMAUPSAMPLER_H_

#include "ChromaUpSampler.h"

class YUV420ChromaUpSampler: public ChromaUpSampler {
public:
	YUV420ChromaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow);
	virtual ~YUV420ChromaUpSampler();
	uint8_t verticalUpSampling(int col);
private:
	int getVerticalIndex(int col,int delta);
};

#endif /* YUV420CHROMAUPSAMPLER_H_ */
