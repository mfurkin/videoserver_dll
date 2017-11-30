/*
 * YUV420format.cpp
 *
 *  Created on: 30 но€б. 2017 г.
 *
 */

#include "YUV420format.h"

YUV420format::YUV420format() {
}

YUV420format::~YUV420format() {
}

unsigned YUV420format::getFirstChromaticAreaOffset(unsigned short width, unsigned short height) {
	return ((height + 15) & ~15) * width;
}

unsigned YUV420format::getSecondChromaticAreaOffset(unsigned short width, unsigned short height) {
	return ((((height * 3) / 2) + 15) & ~15) * width;
}
