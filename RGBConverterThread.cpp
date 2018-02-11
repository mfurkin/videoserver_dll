/*
 * RGBConverterThread.cpp
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#include "RGBConverterThread.h"

RGBConverterThread::RGBConverterThread (uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, uint8_t aCoeffR, uint8_t aCoeffG,
										uint8_t aCoeffB, uint8_t anAddingSecond): source(aSource), dest(aDest),
										width(aWidth), height(aHeight), evIndex(-1),coeff_r(aCoeffR), coeff_g(aCoeffG), coeff_b(aCoeffB),
										adding(anAddingSecond),wait(NULL) {
}

RGBConverterThread::~RGBConverterThread() {
}

unsigned __attribute__((__stdcall__)) RGBConverterThread::convertComponent(void* p) {
	RGBConverterThread* ptr = (RGBConverterThread*) p;
	return  (*ptr).convertThisComponent();
}

unsigned RGBConverterThread::convertThisComponent() {
	int i;
//	,j;
	int h_max = getMaxHeight();
	uint8_t* ptr,*p2;
	int w_max = width >> 1;
	ptr = dest;
	p2 = source;
	for (i=h_max;i--;) {
		convertLine(&p2,&ptr,w_max);
	}
	setEvent();
	return 0;
}

void RGBConverterThread::registerThread(WaitSeveralEvents* aWait) {
	wait = aWait;
	evIndex = (*wait).registerMe();
}

void RGBConverterThread::convertLine(uint8_t** source, uint8_t** dest, int w_max) {
	int i;
	uint8_t*  ptr, *p2;
	uint8_t r,g,b;
	ptr = *dest;
	p2 = *source;
	for (i=w_max;i--;incPtrs(&ptr,&p2)) {
		r = *p2++;
		g = *p2++;
		b = *p2++;
		*ptr = ((r*coeff_r + g*coeff_g + b*coeff_b + ADDING_FIRST) >> SHIFT_BITS) + adding;
	}
	updatePtrs(dest,ptr,source,p2);
}

int RGBConverterThread::getLumaMaxHeight() {
	return height;
}

void RGBConverterThread::setEvent() {
	(*wait).setEvent(evIndex);
}
