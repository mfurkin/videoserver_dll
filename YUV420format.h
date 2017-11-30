/*
 * YUV420format.h
 *
 *  Created on: 30 но€б. 2017 г.
 *
 */

#ifndef YUV420FORMAT_H_
#define YUV420FORMAT_H_

class YUV420format {
public:
	YUV420format();
	virtual ~YUV420format();
	static unsigned getFirstChromaticAreaOffset(unsigned short width,  unsigned short height);
	static unsigned getSecondChromaticAreaOffset(unsigned short width,  unsigned short height);
};

#endif /* YUV420FORMAT_H_ */
