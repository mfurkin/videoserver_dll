/*
 * Line.cpp
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#include "Line.h"



Line::Line(UpSampler* anUpSamplerPtr):upsamplerPtr(anUpSamplerPtr) {
	int len = (*upsamplerPtr).getLength();
	startInit();
	array =  new uint8_t[len];
	if (array) {
		uint8_t* ptr;
//		, *p2;
		int i;
		ptr = array;
		for (i=0;i<len;) {
			*ptr++ = (*upsamplerPtr).verticalUpSampling(i++);
		}
		initCompleted();
	}
}

Line::~Line() {
	if (inited)
		delete [] array;
	if (upsamplerPtr)
		delete upsamplerPtr;
}

uint8_t Line::at(int index) {
	return *(array+index);
}

uint8_t Line::horizontalUpSampling(int index) {
	return (*upsamplerPtr).horizontalUpSampling(array,index);
}

void Line::startInit() {
	inited = 0;
}

void Line::initCompleted() {
	inited = 1;
}

uint8_t Line::lineSample(int anIndex) {
	return isInited() ? getSample(anIndex) : 0;
}

int Line::isInited() {
	return inited;
}
