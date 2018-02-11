/*
 * YUV422toRGBConverter.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "YUV422toRGBConverter.h"

YUV422toRGBConverter::YUV422toRGBConverter(LoggerEngine* aLoggerPtr):YUVtoRGBConverter(aLoggerPtr) {
}

YUV422toRGBConverter::~YUV422toRGBConverter() {
}

uint8_t* YUV422toRGBConverter::getUAreaBegin(uint8_t* source, int width, int height) {
	return source+getUOffset();
}

uint8_t* YUV422toRGBConverter::getVAreaBegin(uint8_t* source, int width, int height) {
	return source+getVOffset();
}

int YUV422toRGBConverter::getMaxHeight(int height) {
	return height;
}

int YUV422toRGBConverter::getMaxWidth(int width) {
	return width >> 1;
}

ChromaUpSampler* YUV422toRGBConverter::getChromaUpSampler(uint8_t* source, int width, int height, int row) {
	return new YUV422ChromaUpSampler(source,width,height,row);
}

int YUV422toRGBConverter::getUOffset() {
	return 1;
}

int YUV422toRGBConverter::getVOffset() {
	return 3;
}

LumaUpSampler* YUV422toRGBConverter::getLumaUpSampler(uint8_t* source, int width, int height, int row) {
	return new YUV422LumaUpSampler(source,width,height,row);
}
