/*
 *
 * ChromaLine.cpp
 *
 *  Created on: 5 ����. 2018 �.
 *
 */

#include "ChromaLine.h"


ChromaLine::ChromaLine(ChromaUpSampler* anUpsamplerPtr):Line(anUpsamplerPtr) {

}

ChromaLine::~ChromaLine() {
}

uint8_t ChromaLine::getSample(int index) {
	return (index & 0x1) ? horizontalUpSampling(index) : at(index>>1);
}




