/*
 * YUV420toRGBConverter.h
 *
 *  Created on: 19 нояб. 2017 г.
 *      Author: Алёна
 */

#ifndef YUV420TORGBCONVERTER_H_
#define YUV420TORGBCONVERTER_H_

#include "Converter.h"

class YUV420toRGBConverter: public Converter {
public:
	YUV420toRGBConverter();
	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
	virtual ~YUV420toRGBConverter();
};

#endif /* YUV420TORGBCONVERTER_H_ */
