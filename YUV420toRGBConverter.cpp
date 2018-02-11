/*
 * YUV420toRGBConverter.cpp
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#include "YUV420toRGBConverter.h"

YUV420toRGBConverter::YUV420toRGBConverter (LoggerEngine* aLoggerPtr):YUVtoRGBConverter(aLoggerPtr) {
}

YUV420toRGBConverter::~YUV420toRGBConverter() {
}





unsigned long YUV420toRGBConverter::getSourceSize(unsigned short width,	unsigned short height) {
	return width*height*2;
}


int YUV420toRGBConverter::getMaxHeight(int height) {
	return height >> 1;
}

uint8_t* YUV420toRGBConverter::getUAreaBegin(uint8_t* source, int width, int height) {
	return source + YUV420format::getSecondChromaticAreaOffset(width,height);
}

uint8_t* YUV420toRGBConverter::getVAreaBegin(uint8_t* source, int width, int height) {
	return source + YUV420format::getFirstChromaticAreaOffset(width,height);
}

int YUV420toRGBConverter::getMaxWidth(int width) {
	return width >> 1;
}

ChromaUpSampler* YUV420toRGBConverter::getChromaUpSampler(uint8_t* source, int width, int height, int row) {
	return new YUV420ChromaUpSampler(source,width,height,row);
}

LumaUpSampler* YUV420toRGBConverter::getLumaUpSampler(uint8_t* source, int width, int height, int row) {
	return new YUV420LumaUpSampler(source,width,height,row);
}
