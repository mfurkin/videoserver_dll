/*
 * Converter.cpp
 *
 *  Created on: 19 нояб. 2017 г.
 *      Author: Алёна
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
