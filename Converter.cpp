/*
 * Converter.cpp
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#include "Converter.h"
HANDLE Converter::endFlag;
// Converter::Converter(uint8_t* aSourcePtr, uint8_t* aDestPtr, unsigned short aWidth, unsigned short aHeight):source_ptr(aSourcePtr), dest_ptr(aDestPtr),width(aWidth),height(aHeight) {
Converter::Converter() {
}


Converter::~Converter() {
}

void Converter::setEndFlag(HANDLE anEndFlag) {
	endFlag = anEndFlag;
}
/*
void Converter::convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) {
	unsigned long size = getDestSize(aWidth,aHeight);
	memcpy(aDest,&size,sizeof(unsigned long));
	convertThis(aSource,aDest+sizeof(unsigned long),aWidth,aHeight);
}
*/
