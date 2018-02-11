/*
 * ChromaUpSampler.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef CHROMAUPSAMPLER_H_
#define CHROMAUPSAMPLER_H_

#include <wtypes.h>
#include "UpSampler.h"
#include "YUVUTils.h"
enum {YUV420_ROW_SIZE = 1};
class ChromaUpSampler: public UpSampler {
public:
	ChromaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow);
	virtual ~ChromaUpSampler();
	uint8_t horizontalUpSampling(uint8_t* buf, int index);
	int getLength();
protected:
	int getRowIndex(int index);
private:
//	int getIndex(int index);
//	uint8_t* buildIndices(int qty);
	void deleteIndices(uint8_t* indices);
	uint8_t* row_indices, *col_indices;
};

#endif /* CHROMAUPSAMPLER_H_ */
