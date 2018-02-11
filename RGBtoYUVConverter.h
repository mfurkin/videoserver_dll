/*
 * RGBtoYUVConverter.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef RGBTOYUVCONVERTER_H_
#define RGBTOYUVCONVERTER_H_

#include <stdint.h>
#include "Converter.h"

#include "ChromaRGBConverterThread.h"
#include "LumaRGBConverterThread.h"

class RGBtoYUVConverter : public Converter {
public:
	RGBtoYUVConverter(LoggerEngine* aLoggerPtr);
	virtual ~RGBtoYUVConverter();
	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
private:
	virtual LumaRGBConverterThread* getLumaRGBConverterThread(uint8_t* aSource,uint8_t* aDest,int aWidth, int aHeight) = 0;
	virtual ChromaRGBConverterThread* getChromaRGBConverterThread(uint8_t* aSource,uint8_t* aDest,int aWidth, int aHeight) = 0;
	virtual uint8_t* getLumaAreaBegin(uint8_t* dest, int width, int height) = 0;
	virtual uint8_t* getChromaUAreaBegin(uint8_t* dest, int width, int height) = 0;
	virtual uint8_t* getChromaVAreaBegin(uint8_t* dest, int width, int height) = 0;
};

#endif /* RGBTOYUVCONVERTER_H_ */
