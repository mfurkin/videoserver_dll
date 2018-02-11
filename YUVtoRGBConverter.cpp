/*
 * YUVtoRGBConverter.cpp
 *
 *  Created on: 25 џэт. 2018 у.
 *
 */

#include "YUVtoRGBConverter.h"

YUVtoRGBConverter::YUVtoRGBConverter(LoggerEngine* aLoggerPtr):Converter(aLoggerPtr) {
//	,row_indices(NULL),col_indices(NULL) {
}

YUVtoRGBConverter::~YUVtoRGBConverter() {
}


void YUVtoRGBConverter::convert(uint8_t* aSource, uint8_t* aDest, unsigned short aWidth, unsigned short aHeight) {
	uint8_t* d_ptr;
		unsigned short c,d,e;
		int i,j;
	uint8_t* luma_area,*u_area,*v_area;
	luma_area = getLumaAreaBegin(aSource,aWidth,aHeight);
	u_area = getUAreaBegin(aSource,aWidth,aHeight);
	v_area = getVAreaBegin(aSource,aWidth,aHeight);
	for(i=0;i<aHeight;i++) {
		LumaLine y_line(getLumaUpSampler(luma_area,aWidth,aHeight,i));
		ChromaLine u_line(getChromaUpSampler(u_area,aWidth,aHeight,i)), v_line(getChromaUpSampler(v_area,aWidth,aHeight,i));
		for (j=0;j<aWidth;j++) {
			c = y_line.lineSample(j) - LUMA_CONSTANT;
			d = u_line.lineSample(j) - CHROMA_CONSTANT;
			e = v_line.lineSample(j) - CHROMA_CONSTANT;
			*d_ptr++ = getRed(c,e);
			*d_ptr++ = getGreen(c,d,e);
			*d_ptr++ = getBlue(c,d);
		}
	}
}

unsigned long YUVtoRGBConverter::getDestSize(unsigned short aWidth,	unsigned short aHeight) {
	return aWidth*aHeight*COLORS_NUM;
}

uint8_t YUVtoRGBConverter::getRed(uint8_t c, uint8_t e) {
	short result = (C_COEFF*c + E_COEFF_R*e + ADDING) >> SHIFT_BITS;
	return YUVUtils::clip8(result);
}

uint8_t YUVtoRGBConverter::getGreen(uint8_t c, uint8_t d, uint8_t e) {
	short result = (c*C_COEFF+d*D_COEFF_G+e*E_COEFF_G+ADDING) >> SHIFT_BITS;
	return YUVUtils::clip8(result);
}

uint8_t YUVtoRGBConverter::getBlue(uint8_t c, uint8_t d) {
	short result = (C_COEFF*c + D_COEFF_B*d + ADDING) >> SHIFT_BITS;
	return YUVUtils::clip8(result);
}

uint8_t* YUVtoRGBConverter::getLumaAreaBegin(uint8_t* source, int width, int height) {
	return source;
}

uint8_t YUVtoRGBConverter::at(uint8_t* frame, int index) {
	return *(frame+index);
}

