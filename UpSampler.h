/*
 * UpSampler.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef UPSAMPLER_H_
#define UPSAMPLER_H_

#include <stdint.h>

class UpSampler {
public:
	UpSampler(uint8_t* aSource,int aWidth, int aHeight, int aRow);
	virtual ~UpSampler();
	virtual uint8_t horizontalUpSampling(uint8_t* source, int index) = 0;
	virtual uint8_t verticalUpSampling(int col) = 0;
	virtual int getLength() = 0;
protected:
	uint8_t at(int col);
	int getIndexWithOffset(int delta);
	int getIndexFromRowAndCol(int row, int col);
	int getIndexFromRowAndCol422(int col);
	int getChromaLength();
	int getLumaLength();
	uint8_t* buildIndices(int qty);
	uint8_t* buildRowIndices();
	uint8_t* buildColIndices();
private:
	uint8_t* source;

	int width,height,row;
};

#endif /* UPSAMPLER_H_ */
