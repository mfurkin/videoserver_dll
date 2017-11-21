/*
 * YUV420toYUV422Converter.h
 *
 *  Created on: 21 нояб. 2017 г.
 *      Author: Алёна
 */

#ifndef YUV420TOYUV422CONVERTER_H_
#define YUV420TOYUV422CONVERTER_H_
#include <process.h>
#include <windef.h>
#include "Converter.h"
#include "ChromaWriter.h"
#include "LumaWriter.h"
enum {numOfThreads=3,END_INDEX=4};
class YUV420toYUV422Converter: public Converter {
public:
	YUV420toYUV422Converter();
	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
	virtual ~YUV420toYUV422Converter();
private:
	virtual unsigned short getLumaDelta();
	virtual unsigned short getLumaOffset();
	virtual unsigned short getBlueOffset();
	virtual uint8_t* getBluePtr(uint8_t* pic, unsigned short width, unsigned short height);
	virtual unsigned short getRedOffset();
	virtual uint8_t* getRedPtr(uint8_t* pic, unsigned short width, unsigned short height);
	uint8_t* getFirstChromaticArea(uint8_t* pic, unsigned short width, unsigned short height);
	uint8_t* getSecondChromaticArea(uint8_t* pic, unsigned short width, unsigned short height);
};

#endif /* YUV420TOYUV422CONVERTER_H_ */
