/*
 * LumaLine.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef LUMALINE_H_
#define LUMALINE_H_

#include <wtypes.h>
#include "Line.h"
#include "LumaUpSampler.h"

class LumaLine: public Line {
public:
	LumaLine(LumaUpSampler* aLumaUpSamplerPtr);
	virtual ~LumaLine();
	uint8_t getSample(int index);
protected:
	uint8_t horizontalUpSampling(int index);
};

#endif /* LUMALINE_H_ */
