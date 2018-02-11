/*
 * YUV422LumaRGBConverterThread.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "YUV422LumaRGBConverterThread.h"

YUV422LumaRGBConverterThread::YUV422LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight):LumaRGBConverterThread(aSource,aDest,aWidth,aHeight,LUMA_YUV422_DELTA) {
}

YUV422LumaRGBConverterThread::~YUV422LumaRGBConverterThread() {
}
