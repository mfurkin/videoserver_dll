/*
 * RGBConverterThread.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef RGBCONVERTERTHREAD_H_
#define RGBCONVERTERTHREAD_H_
#include <stdint.h>
#include <windef.h>
enum {RGB_SIZE=3,ADDING_FIRST=128,SHIFT_BITS=8 };
class RGBConverterThread {
public:
	RGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight, uint8_t aCoeffR, uint8_t aCoeffG, uint8_t aCoeffB,
						uint8_t anAdding);
	virtual ~RGBConverterThread();
	static unsigned WINAPI convertComponent(void* p);
	unsigned convertThisComponent();
	void registerThread(WaitSeveralEvents* wait);
protected:
	int getLumaMaxHeight();
private:
	virtual int getMaxHeight() = 0;
	virtual void incPtrs(uint8_t** dest_ptr, uint8_t** source_ptr) = 0;
	void convertLine(uint8_t** source, uint8_t** dest, int w_max);
	virtual void updatePtrs(uint8_t** dptr, uint8_t* dest_ptr, uint8_t** sptr, uint8_t* source_ptr) = 0;
	void setEvent();
	uint8_t*  source, *dest;
	int width, height;
	int evIndex;
	uint8_t coeff_r,coeff_g, coeff_b, adding;
	WaitSeveralEvents* wait;
};

#endif /* RGBCONVERTERTHREAD_H_ */
