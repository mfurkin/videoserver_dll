/*
 * YUV420LumaRGBConverterThread.h
 *
 *  Created on: 7 февр. 2018 г.
 *
 */

#ifndef YUV420LUMARGBCONVERTERTHREAD_H_
#define YUV420LUMARGBCONVERTERTHREAD_H_

#include "LumaRGBConverterThread.h"
enum { LUMA_YUV420_DELTA=1 };
class YUV420LumaRGBConverterThread: public LumaRGBConverterThread {
public:
	YUV420LumaRGBConverterThread(uint8_t* aSource, uint8_t* aDest, int aWidth, int aHeight);
	virtual ~YUV420LumaRGBConverterThread();
};

#endif /* YUV420LUMARGBCONVERTERTHREAD_H_ */
