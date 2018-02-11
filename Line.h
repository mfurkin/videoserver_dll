/*
 * Line.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef LINE_H_
#define LINE_H_
#include <stdint.h>
#include "UpSampler.h"
class Line {
public:
	Line(UpSampler* anUpSamplerPtr);
	virtual ~Line();
	uint8_t lineSample(int anIndex);
protected:
	uint8_t at(int index);
	virtual uint8_t horizontalUpSampling(int index);
private:
	void startInit();
	void initCompleted();
	virtual int isInited();
	virtual uint8_t getSample(int index) = 0;
	UpSampler* upsamplerPtr;
	uint8_t* array;
	int inited;
};

#endif /* LINE_H_ */
