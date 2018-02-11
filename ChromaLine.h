/*
 * ChromaLine.h
 *
 *  Created on: 5 февр. 2018 г.
 *
 */

#ifndef CHROMALINE_H_
#define CHROMALINE_H_
#include "ChromaUpSampler.h"
#include "Line.h"

class ChromaLine: public Line {
public:
	ChromaLine(ChromaUpSampler* anUpsamplePtr);
	virtual ~ChromaLine();
	uint8_t getSample(int index);
};

#endif /* CHROMALINE_H_ */
