/*
 * ChromaWriter.h
 *
 *  Created on: 21 но€б. 2017 г.
 *
 */

#ifndef CHROMAWRITER_H_
#define CHROMAWRITER_H_
#include <iostream>
#include <stdint.h>
#include <wtypes.h>
#include <winbase.h>
#include <windef.h>
#include "LoggerEngine.h"

class ChromaWriter {

public:

	ChromaWriter(uint8_t* aSourcePtr,uint8_t* aDestPtr, unsigned short aWidth, unsigned short aHeight,unsigned short anOffset,
				 HANDLE aCompleted, std::string aColorName, int aFlag = 1, LoggerEngine* aLoggerPtr = NULL);
	virtual ~ChromaWriter();
	static unsigned WINAPI writeChroma(void* p);
private:
	void log(std::string msg);
	void logPtr(std::string msg, unsigned ptr);
	void logString(std::string msg, std::string& msg2);
	unsigned writeThisChroma();
	unsigned short getChromaticDelta();
	void writeChromaSample(uint8_t*& ptr,  uint8_t data);
	uint8_t getUpSamplingChroma(unsigned short row,unsigned short col);
	uint8_t clip8(unsigned short data);
	uint8_t getUpSamplingLastLine(unsigned short col);
	uint8_t* source_ptr, *dest_ptr;
	unsigned short width,height,offset;
	std::string colorName,tag;
	HANDLE completed;
	int flag;
	LoggerEngine* logger_ptr;
};

#endif /* CHROMAWRITER_H_ */
