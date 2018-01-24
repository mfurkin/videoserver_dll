/*
 * Converter.cpp
 *
 *  Created on: 19 но€б. 2017 г.
 *
 */

#include "Converter.h"
HANDLE Converter::endFlag;
std::string Converter::CONVERTER_TAG="Converter";
Converter::Converter(LoggerEngine* aLoggerPtr):logger_ptr(aLoggerPtr) {
}

Converter::~Converter() {
}

void Converter::setEndFlag(HANDLE anEndFlag) {
	endFlag = anEndFlag;
}

unsigned Converter::waitForEvents(HANDLE* events, int threadsNum) {
	int i,numOfThreads = threadsNum+1;
	HANDLE* allEvents = new HANDLE [numOfThreads];
	unsigned result;
	for (i=0;i<threadsNum;i++)
		allEvents[i] = events[i];
	allEvents[threadsNum] = endFlag;
	result = WaitForMultipleObjectsEx(numOfThreads,allEvents,FALSE,INFINITE,TRUE);
	delete [] allEvents;
	return result;
}

void Converter::log(std::string msg) {
	if (logger_ptr)
		(*logger_ptr).log(CONVERTER_TAG,msg);
}

void Converter::logPtr(std::string msg, unsigned ptr) {
	if (logger_ptr)
		(*logger_ptr).logPtr(CONVERTER_TAG,msg,ptr);
}
