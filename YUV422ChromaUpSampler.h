/*
 * YUV422ChromaUpSampler.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef YUV422CHROMAUPSAMPLER_H_
#define YUV422CHROMAUPSAMPLER_H_

#include "ChromaUpSampler.h"

class YUV422ChromaUpSampler: public ChromaUpSampler {
public:
	YUV422ChromaUpSampler(uint8_t* source, int width, int height, int row);
	virtual ~YUV422ChromaUpSampler();
private:
	uint8_t verticalUpSampling(int col);
};

#endif /* YUV422CHROMAUPSAMPLER_H_ */
