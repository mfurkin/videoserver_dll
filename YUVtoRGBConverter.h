/*
 * YUVtoRGBConverter.h
 *
 *  Created on: 25 џэт. 2018 у.
 *
 */

#ifndef YUVTORGBCONVERTER_H_
#define YUVTORGBCONVERTER_H_
#include "ChromaLine.h"
#include "LumaLine.h"
#include "Converter.h"
#include "server_names.h"

enum {C_COEFF=298, E_COEFF_R=409, D_COEFF_G= -100, E_COEFF_G=-208, D_COEFF_B=516, ADDING=128, SHIFT_BITS=8};
enum{LUMA_CONSTANT=16,CHROMA_CONSTANT=128};
class YUVtoRGBConverter: public Converter {
public:
	YUVtoRGBConverter(LoggerEngine* aLoggerPtr);
	virtual ~YUVtoRGBConverter();
	void convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight);
	unsigned long getDestSize(unsigned short aWidth, unsigned short aHeight);
private:
	uint8_t* buildIndices(int max_num);
	uint8_t at(uint8_t* frame, int index);
	virtual uint8_t* getLumaAreaBegin(uint8_t* source, int widdth,int height);
	virtual ChromaUpSampler* getChromaUpSampler(uint8_t* source, int width, int height, int row) = 0;
	virtual LumaUpSampler* getLumaUpSampler(uint8_t* source, int width, int height, int row) = 0;
	uint8_t horizontalUpSampling(uint8_t* source,int index);
	uint8_t sampleByIndices(uint8_t* frame, int width, int row, int col);
	uint8_t getLuma(uint8_t* source, int width, int height, int row, int col);
	virtual uint8_t* getUAreaBegin(uint8_t* source, int width,int height) = 0;
	virtual uint8_t* getVAreaBegin(uint8_t* source, int width,int height) = 0;
	virtual int getMaxHeight(int height) = 0;
	virtual int getMaxWidth(int width) = 0;
	uint8_t getRed(uint8_t c, uint8_t e);
	uint8_t getGreen(uint8_t c, uint8_t d, uint8_t e);
	uint8_t getBlue(uint8_t c, uint8_t d);
};

#endif /* YUVTORGBCONVERTER_H_ */
