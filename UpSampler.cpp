/*
 * UpSampler.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "UpSampler.h"


UpSampler::UpSampler(uint8_t* aSource, int aWidth, int aHeight, int aRow):source(aSource),width(aWidth),height(aHeight),row(aRow) {
}

UpSampler::~UpSampler() {
}



uint8_t UpSampler::at(int col) {
	return *(source+col);
}

int UpSampler::getIndexWithOffset(int delta) {
	return row+delta;;
}

int UpSampler::getIndexFromRowAndCol(int row, int col) {
	return row*width+col;
}

int UpSampler::getIndexFromRowAndCol422(int col) {
	return getIndexFromRowAndCol(row<<1,col);
}

int UpSampler::getChromaLength() {
	return width >> 1;
}

int UpSampler::getLumaLength() {
	return width;
}

uint8_t* UpSampler::buildIndices(int qty) {
	uint8_t* result =  new uint8_t[qty+2];
	int i,max_num1= qty - 1;
	for(i=0;i<max_num1;i++)
		*(result+i) = i;
	*(result+i++) = max_num1;
	*(result+i++) = max_num1;
	return result;
}

uint8_t* UpSampler::buildRowIndices() {
	return buildIndices(height);
}

uint8_t* UpSampler::buildColIndices() {
	return buildIndices(width);
}
