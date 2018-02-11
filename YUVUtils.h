/*
 * YUVUtils.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef YUVUTILS_H_
#define YUVUTILS_H_

#include <stdint.h>
enum {MAX_BYTE=255};
class YUVUtils {
public:
static uint8_t clip8(short data);
};

#endif /* YUVUTILS_H_ */
