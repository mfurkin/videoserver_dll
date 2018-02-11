/*
 * YUV422LumaRGBConverterThread.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef YUV422LUMARGBCONVERTERTHREAD_H_
#define YUV422LUMARGBCONVERTERTHREAD_H_

#include "LumaRGBConverterThread.h"
enum {LUMA_YUV422_DELTA=2};
class YUV422LumaRGBConverterThread: public LumaRGBConverterThread {
public:
	YUV422LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight);
	virtual ~YUV422LumaRGBConverterThread();
};

#endif /* YUV422LUMARGBCONVERTERTHREAD_H_ */
