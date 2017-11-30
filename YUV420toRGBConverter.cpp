/*
 * YUV420toRGBConverter.cpp
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#include "YUV420toRGBConverter.h"

YUV420toRGBConverter::YUV420toRGBConverter (){
}

YUV420toRGBConverter::~YUV420toRGBConverter() {
}

void YUV420toRGBConverter::convert(uint8_t* source, uint8_t* dest, unsigned short width, unsigned short height) {
}

unsigned long YUV420toRGBConverter::getDestSize(unsigned short width, unsigned short height) {
	return width*height*COLORS_NUM;
}
