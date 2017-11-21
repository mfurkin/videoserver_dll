/*
 * Converter.h
 *
 *  Created on: 19 нояб. 2017 г.
 *      Author: Алёна
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_
#include <wtypes.h>
#include <stdint.h>
class Converter {
public:
	Converter();
	virtual void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) = 0;
	static void setEndFlag(HANDLE anEndFlag);
	virtual ~Converter();
protected:
	static HANDLE endFlag;
};

#endif /* CONVERTER_H_ */
