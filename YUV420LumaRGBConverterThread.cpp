/*
 * YUV420LumaRGBConverterThread.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "YUV420LumaRGBConverterThread.h"

YUV420LumaRGBConverterThread::YUV420LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight):LumaRGBConverterThread(aSource,aDest,aWidth,aHeight,LUMA_YUV420_DELTA) {
}

YUV420LumaRGBConverterThread::~YUV420LumaRGBConverterThread() {
}
