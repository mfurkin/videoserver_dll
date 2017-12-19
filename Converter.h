/*
 * Converter.h
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_
#include <iostream>
#include <wtypes.h>
#include <stdint.h>
class Converter {
public:
	Converter();
//	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
	virtual void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) = 0;
	virtual unsigned long  getSourceSize(unsigned short width, unsigned short height) = 0;
	virtual unsigned long getDestSize(unsigned short aWidth, unsigned short aHeight) = 0;
	static void setEndFlag(HANDLE anEndFlag);
	virtual ~Converter();
protected:
	static HANDLE endFlag;
	};

#endif /* CONVERTER_H_ */
