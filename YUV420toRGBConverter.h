/*
 * YUV420toRGBConverter.h
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#ifndef YUV420TORGBCONVERTER_H_
#define YUV420TORGBCONVERTER_H_

#include "YUV420toSomethingConverter.h"
#include "server_names.h"

class YUV420toRGBConverter: public YUV420toSomethingConverter {
public:
	YUV420toRGBConverter();
	virtual ~YUV420toRGBConverter();
private:
	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
	unsigned long getDestSize(unsigned short aWidth, unsigned short aHeight);
};

#endif /* YUV420TORGBCONVERTER_H_ */
