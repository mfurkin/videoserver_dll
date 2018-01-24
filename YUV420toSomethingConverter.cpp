/*
 * YUV420toSomethingConverter.cpp
 *
 *  Created on: 30 ����. 2017 �.
 *
 */

#include "YUV420toSomethingConverter.h"

YUV420toSomethingConverter::YUV420toSomethingConverter(LoggerEngine* aLoggerPtr):Converter(aLoggerPtr) {
}

YUV420toSomethingConverter::~YUV420toSomethingConverter() {
}

unsigned long YUV420toSomethingConverter::getSourceSize(unsigned short width, unsigned short height) {
	return YUV420format::getSecondChromaticAreaOffset(width,height)+width*height;
}


