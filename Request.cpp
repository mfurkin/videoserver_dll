/*
 * Request.cpp
 *
 *  Created on: 12 но€б. 2017 г.
 *
 */

#include "Request.h"

HANDLE Request::endFlag=NULL;
std::map<unsigned short, Converter*> Request::converters;

Request::Request(std::string aPingName):pingName(aPingName),inited(FALSE) {
	HANDLE hPingChannel = OpenFileMapping(FILE_MAP_READ,FALSE,pingName.c_str());
	if (hPingChannel  == INVALID_HANDLE_VALUE)
		std::cerr<<"Request could not be  executed: source could not be open, error code="<<GetLastError()<<"\n";
	else {
		pingChannel = (uint8_t*) MapViewOfFile(hPingChannel,FILE_MAP_ALL_ACCESS,0,0,sizeof(RequestDataStruct));
		if (pingChannel == NULL)
			std::cerr<<"Request could not be executed: Ping channel could not be open error code="<<GetLastError()<<"\n";
		else {
			RequestDataStruct* req_ptr = (RequestDataStruct*) pingChannel;
			HANDLE hSourceFile = CreateFile((*req_ptr).source_name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hSourceFile == INVALID_HANDLE_VALUE)
				std::cerr<<"Request could  not be executed: Source file could not be open error code= "<<GetLastError()<<"\n";
			else {
				width = (*req_ptr).width;
				height = (*req_ptr).height;
				unsigned short size = width*height*COLORS_NUM;
				hMapSource = CreateFileMapping(hSourceFile,NULL,PAGE_READONLY,0,size,NULL);
				if (hMapSource == INVALID_HANDLE_VALUE)
					std::cerr<<"Request could not be executed: file mapping for source file could not be created error code="<<GetLastError()<<"\n";
				else {
					HANDLE hDestFile = OpenFileMapping(FILE_MAP_WRITE,FALSE,(*req_ptr).dataName);
					if (hDestFile == INVALID_HANDLE_VALUE)
						std::cerr<<"Request could not be executed: dest file mappping could not be open error code="<<GetLastError()<<"\n";
					conv_type = (*req_ptr).conv_type;
					destFile = (uint8_t*) MapViewOfFile(hDestFile,FILE_MAP_WRITE,0,0,size+sizeof(unsigned long));
					pingReq = OpenEvent(SYNCHRONIZE,FALSE,(*req_ptr).pingReqName);
					pingNotify = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).pingNotifyName);
					writeCompleted = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).writeCompletedName);
					writeEnabled = OpenEvent(SYNCHRONIZE,FALSE,(*req_ptr).writeEnabledName);
					inited = TRUE;
					std::cerr<<"Request ctor have been completed  successfully.\n";
				}
			}
		}
	}
}

Request::~Request() {
}

void Request::runThisRequest() {
	status = REQ_RECEIVED;
	progress = 0;
	LARGE_INTEGER size,cur_ptr;
	BOOL res2;
	_beginthreadex(NULL,0,pingChannelThread,this,0,NULL);
	res2 = GetFileSizeEx(hMapSource,&size);
	if (res2) {
				HANDLE events[2];
				uint8_t* source_ptr;
				unsigned result;
				cur_ptr.QuadPart = 0;
				status = REQ_CONV_IN_PROGRESS;
				Converter* conv_ptr = converters[conv_type];
				unsigned picSize = (*conv_ptr).getSourceSize(width,height);
				events[0] = writeEnabled;
				events[1] = endFlag;
				unsigned count = 0,frames;
				frames = size.QuadPart / picSize;
				for (sendHeaderData(events,2,picSize,frames);(cur_ptr.QuadPart < size.QuadPart) && (result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE)) && (result == WAIT_OBJECT_0);) {
					source_ptr = (uint8_t*)MapViewOfFile(hMapSource,FILE_MAP_READ,cur_ptr.HighPart,cur_ptr.LowPart,picSize);
					(*conv_ptr).convert(source_ptr,destFile,width,height);
					progress = ++count*100.0 / frames;
					SetEvent(writeCompleted);
				}
				status = (result == WAIT_OBJECT_0) ? REQ_COMPLETED : REQ_ABORTED;
	}
}

unsigned Request::runRequest(void* p) {
	Request* ptr = (Request*) p;
	(*ptr).runThisRequest();
	delete ptr;
	return 0;
}

void Request::setEndFlag(HANDLE anEndFlag) {
	endFlag = anEndFlag;
}

void Request::initConverters() {
	Converter* ptr;
	Converter::setEndFlag(endFlag);
	ptr = new YUV420toRGBConverter();
	converters[YUV420toRGB24] = ptr;
}
void Request::releaseConverter(std::pair<unsigned short, Converter*> pair) {
	delete pair.second;
}
void Request::deinitConverters() {
	std::map<unsigned short,Converter*>::iterator begin = converters.begin(), end = converters.end();
	for_each(begin,end,Request::releaseConverter);
}

void Request::sendHeaderData(HANDLE* events, int evNum, unsigned frameSize, unsigned framesQty) {
//	unsigned result;
	HeaderDataStruct* header_ptr;
	header_ptr = (HeaderDataStruct*) destFile;
	(*header_ptr).frame_size = frameSize;
	(*header_ptr).frames_qty = framesQty;
	SetEvent(writeCompleted);
}

unsigned Request::pingChannelProc() {
	HANDLE events[2];
	ResponseDataStruct* resp_ptr;
	events[REQ_FLAG_INDEX] = pingReq;
	events[END_FLAG_INDEX] = endFlag;
	int index;
	unsigned result;
	for (;(result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE));) {
		index = result - WAIT_OBJECT_0;
		if (index == END_FLAG_INDEX) {
			status = REQ_ABORTED;
			break;
		}
		resp_ptr =(ResponseDataStruct*) pingChannel;
		(*resp_ptr).progress = progress;
		(*resp_ptr).req_status = status;
		SetEvent(pingNotify);
	}
	return 0;
}

unsigned Request::pingChannelThread(void* p) {
	Request* ptr = (Request*) p;
	return (*ptr).pingChannelProc();
}
