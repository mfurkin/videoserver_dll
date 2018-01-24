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
#include "LoggerEngine.h"
class Converter {
public:
	Converter(LoggerEngine* aLoggerPtr);
	virtual void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) = 0;
	virtual unsigned long  getSourceSize(unsigned short width, unsigned short height) = 0;
	virtual unsigned long getDestSize(unsigned short aWidth, unsigned short aHeight) = 0;
	static void setEndFlag(HANDLE anEndFlag);
	virtual ~Converter();
protected:
	unsigned waitForEvents(HANDLE* events,int threadsNum);
	virtual void log(std::string msg);
	virtual void logPtr(std::string msg, unsigned ptr);
private:
	static HANDLE endFlag;
	static std::string CONVERTER_TAG;
	LoggerEngine* logger_ptr;
};

#endif /* CONVERTER_H_ */
