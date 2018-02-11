/*
 * YUV422toRGBConverter.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef YUV422TORGBCONVERTER_H_
#define YUV422TORGBCONVERTER_H_

#include "YUVtoRGBConverter.h"
#include "YUV422LumaUpSampler.h"
#include "YUV422ChromaUpSampler.h"

class YUV422toRGBConverter: public YUVtoRGBConverter {
public:
	YUV422toRGBConverter(LoggerEngine* aLoggerPtr);
	virtual ~YUV422toRGBConverter();
private:
	virtual int getUOffset();
	virtual int getVOffset();
	uint8_t* getUAreaBegin(uint8_t* source, int width,int height);
	uint8_t* getVAreaBegin(uint8_t* source, int width,int height);
	int getMaxHeight(int height);
	int getMaxWidth(int width);
	ChromaUpSampler* getChromaUpSampler(uint8_t* source, int width, int height, int row);
	LumaUpSampler* getLumaUpSampler(uint8_t* source, int width, int height, int row);
};

#endif /* YUV422TORGBCONVERTER_H_ */
