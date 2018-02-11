/*
 * ChromaUpSampler.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "ChromaUpSampler.h"

ChromaUpSampler::ChromaUpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow):UpSampler(aSource, aWidth, aHeight, aRow), row_indices(NULL),col_indices(NULL) {
	row_indices = buildRowIndices();
	col_indices = buildColIndices();
}

ChromaUpSampler::~ChromaUpSampler() {
	deleteIndices(row_indices);
	deleteIndices(col_indices);
}


int ChromaUpSampler::getRowIndex(int index) {
	return row_indices[index];
}

void ChromaUpSampler::deleteIndices(uint8_t* indices) {
	if (indices)
		delete [] indices;
}

uint8_t ChromaUpSampler::horizontalUpSampling(uint8_t* buf,int index) {
	return YUVUtils::clip8((9*(buf[col_indices[index]] + buf[col_indices[index+1]]) - (buf[col_indices[index-1]] + buf[col_indices[index+2]]) + 8) >> 4);
}

int ChromaUpSampler::getLength() {
	return getChromaLength();
//	return width >> 1;
}
