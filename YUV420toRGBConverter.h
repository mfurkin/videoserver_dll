/*
 * YUV420toRGBConverter.h
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#ifndef YUV420TORGBCONVERTER_H_
#define YUV420TORGBCONVERTER_H_

#include "YUVtoRGBConverter.h"
#include "YUV420Format.h"
#include "YUV420LumaUpSampler.h"
#include "YUV420ChromaUpSampler.h"
// #include "server_names.h"

class YUV420toRGBConverter: public YUVtoRGBConverter {
public:
	YUV420toRGBConverter(LoggerEngine* aLoggerPtr);
	virtual ~YUV420toRGBConverter();
	unsigned long  getSourceSize(unsigned short width, unsigned short height);
private:
	uint8_t* getUAreaBegin(uint8_t* source, int width,int height);
	uint8_t* getVAreaBegin(uint8_t* source, int width,int height);
	/*
	int getLumaDelta();
	int getChromaDelta();

	uint8_t getLuma(uint8_t* source, int width, int row, int col);
	uint8_t getChromaU(uint8_t* source, int width, int height, int row, int col);
	uint8_t getChromaV(uint8_t* source, int width, int height, int row, int col);*/
	int getMaxHeight(int height);
/*
	uint8_t chromaSampleU(uint8_t* source, int width, int height, int row, int col);
	uint8_t chromaSampleV(uint8_t* source, int width, int height, int row, int col);
	uint8_t sampleByIndices(uint8_t* frame, int width, int row, int col);
*/
	int getMaxWidth(int width);
	ChromaUpSampler* getChromaUpSampler(uint8_t* source, int width, int height, int row);
	LumaUpSampler* getLumaUpSampler(uint8_t* source, int width, int height, int row);
};

#endif /* YUV420TORGBCONVERTER_H_ */
