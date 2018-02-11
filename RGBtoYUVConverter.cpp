/*
 * RGBtoYUVConverter.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "RGBtoYUVConverter.h"

RGBtoYUVConverter::RGBtoYUVConverter(LoggerEngine* aLoggerPtr):Converter(aLoggerPtr) {
}

RGBtoYUVConverter::~RGBtoYUVConverter() {
}

void RGBtoYUVConverter::convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) {
	int i,j;
	uint8_t* ptr,*p2;
	LumaRGBConverterThread* luma_ptr = getLumaRGBConverterThread(aSource,getLumaAreaBegin(aDest,aWidth,aHeight),aWidth,aHeight);
	ChromaRGBConverterThread* 	u_ptr = getChromaRGBConverterThread(aSource,getChromaUAreaBegin(aDest,aWidth,aHeight),aWidth,aHeight),
								*v_ptr = getChromaRGBConverterThread(aSource,getChromaVAreaBegin(aDest,aWidth,aHeight),aWidth,aHeight);
	WaitSeveralEvents wait(3,endFlag);
	(*luma_ptr).registerThread(&wait);
	(*u_ptr).registerThread(&wait);
	(*v_ptr).registerThread(&wait);
	wait.waitAllThreads();
}
