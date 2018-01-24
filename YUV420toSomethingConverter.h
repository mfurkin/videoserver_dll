/*
 * YUV420toSomethingConverter.h
 *
 *  Created on: 30 но€б. 2017 г.
 *
 */

#ifndef YUV420TOSOMETHINGCONVERTER_H_
#define YUV420TOSOMETHINGCONVERTER_H_
#include "Converter.h"
#include "YUV420format.h"
class YUV420toSomethingConverter : public Converter{
public:
	YUV420toSomethingConverter(LoggerEngine* aLoggerPtr);
	unsigned long getSourceSize(unsigned short width, unsigned short height);
	virtual ~YUV420toSomethingConverter();
};

#endif /* YUV420TOSOMETHINGCONVERTER_H_ */
