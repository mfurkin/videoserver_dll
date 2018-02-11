/*
 * YUVUtils.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "YUVUtils.h"


uint8_t YUVUtils::clip8(short data) {
	return (data<0) ? 0 : (data>MAX_BYTE) ? MAX_BYTE : (uint8_t) data;
}


