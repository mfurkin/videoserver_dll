/*
 * Request.cpp
 *
 *  Created on: 12 но€б. 2017 г.
 *
 */

#include "Request.h"

HANDLE Request::endFlag=NULL;
std::map<unsigned short, Converter*> Request::converters;
Request::Request(std::string aPingName):pingName(aPingName),inited(FALSE),in_progress(0) {
	std::cerr<<"Request ctor pingName="<<pingName<<"\n";
	HANDLE hPingChannel = OpenFileMapping(FILE_MAP_WRITE,FALSE,pingName.c_str());
	if (!(hPingChannel))
		std::cerr<<"Request could not be  executed: ping channel could not be open, error code="<<GetLastError()<<"\n";
	else {
		pingChannel = (uint8_t*) MapViewOfFile(hPingChannel,FILE_MAP_ALL_ACCESS,0,0,sizeof(RequestDataStruct));
		if (!(pingChannel))
			std::cerr<<"Request could not be executed: Ping channel could not be mapped error code="<<GetLastError()<<"\n";
		else {
			RequestDataStruct* req_ptr = (RequestDataStruct*) pingChannel;
			HANDLE hSourceFile = CreateFile((*req_ptr).source_name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hSourceFile == INVALID_HANDLE_VALUE)
				std::cerr<<"Request could  not be executed: Source file could not be open error code= "<<GetLastError()<<"\n";
			else {
				width = (*req_ptr).width;
				height = (*req_ptr).height;
				unsigned res3  = GetFileSizeEx(hSourceFile,&fileSize);
				if (!(res3))
					std::cerr<<"Source file size could not be got error code="<<GetLastError()<<"\n";
				else {
					unsigned size = width*height*COLORS_NUM;
//					hMapSource = CreateFileMapping(hSourceFile,NULL,PAGE_READONLY,0,size,NULL);
					hMapSource = CreateFileMapping(hSourceFile,NULL,PAGE_READONLY,0,0,NULL);
					if (hMapSource == INVALID_HANDLE_VALUE)
						std::cerr<<"Request could not be executed: file mapping for source file could not be created error code="<<GetLastError()<<"\n";
					else {
						HANDLE hDestFile = OpenFileMapping(FILE_MAP_WRITE,FALSE,(*req_ptr).dataName);
						if (hDestFile == INVALID_HANDLE_VALUE)
							std::cerr<<"Request could not be executed: dest file mappping could not be open error code="<<GetLastError()<<"\n";
						else {
							conv_type = (*req_ptr).conv_type;
							destFile = (uint8_t*) MapViewOfFile(hDestFile,FILE_MAP_WRITE,0,0,size+sizeof(unsigned long));
							if(!(destFile))
								std::cerr<<"Request ctor: dest sharedmemory could not be got error code:"<<GetLastError()<<"\n";
							pingReq = OpenEvent(SYNCHRONIZE,FALSE,(*req_ptr).pingReqName);
							pingNotify = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).pingNotifyName);
							std::cerr<<"Request ctor destFileAccessName="<<(*req_ptr).destFileAccessName<<"\n";
							destFileAccess = OpenMutex(SYNCHRONIZE,FALSE,(*req_ptr).destFileAccessName);
							if (!(destFileAccess))
								std::cerr<<"Request ctor error during destFileAccessMutex opening error code="<<GetLastError()<<"\n";
							headerDataWritten = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).headerDataWrittenName);
							inited = destFile && pingReq && pingNotify && destFileAccess && headerDataWritten;
							if (inited)
								std::cerr<<"Request ctor have been completed  successfully.\n";
							else {
								std::cerr<<"Some errors have been got during Request ctor\n";
								Server::logPtr("Request ctor destFile=",(unsigned)destFile);
								Server::logPtr("Request ctor pingReq=",(unsigned)pingReq);
								Server::logPtr("Request ctor  pingNotify=",(unsigned)pingNotify);
								Server::logPtr("Request ctor destFileAccess=",(unsigned)destFileAccess);
								Server::logPtr("Request ctor headerDataWritten=",(unsigned)headerDataWritten);
							}
						}
					}
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
	LARGE_INTEGER cur_ptr;
	std::cerr<<"Request::runThisRequest enter inited="<<inited<<"\n";
	if (inited) {
		in_progress = 1;
		_beginthreadex(NULL,0,pingChannelThread,this,0,NULL);
		std::cerr<<"Request::runThisRequest pt1 \n";
				HANDLE events[2];
				uint8_t* source_ptr;
				unsigned result;
				cur_ptr.QuadPart = 0;
				status = REQ_CONV_IN_PROGRESS;
				std::cerr<<"Request::runThisRequest pt2 convtype="<<conv_type<<"\n";
				Converter* conv_ptr = converters[conv_type];
				if (!(conv_ptr))
					std::cerr<<"Request::runThisRequest unknown converion type!\n";
				else {
						unsigned picSize = (*conv_ptr).getSourceSize(width,height);
						events[0] = destFileAccess;
						events[1] = endFlag;
						unsigned count = 0,frames;
						unsigned long out_size = (*conv_ptr).getDestSize(width,height);
//						frames = fileSize.QuadPart / picSize;
						frames = fileSize.QuadPart / out_size;
						std::cerr<<"Request::runThisRequest before loop size="<<fileSize.QuadPart<<" cur_ptr="<<cur_ptr.QuadPart<<" picSize="<<picSize<<"\n";

//						for (sendHeaderData(events,2,picSize,frames);(in_progress) && (cur_ptr.QuadPart < fileSize.QuadPart) && (!(result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE)));) {
						for (sendHeaderData(events,2,out_size,frames);(in_progress) && (cur_ptr.QuadPart < fileSize.QuadPart) && (!(result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE)));) {
							std::cerr<<"Request::runThisRequest loop pt1\n";
							source_ptr = (uint8_t*)MapViewOfFile(hMapSource,FILE_MAP_READ,cur_ptr.HighPart,cur_ptr.LowPart,picSize);
							Server::logPtr("Request::runThisRequest loop pt2 source_ptr=",(unsigned)source_ptr);
							if (!(source_ptr)) {
								std::cerr<<"Request::runThisRequest error during MapViewOfFile error code="<<GetLastError()<<"\n";
								in_progress = 0;
							} else {
								(*conv_ptr).convert(source_ptr,destFile,width,height);
								std::cerr<<"Request::runThisRequest loop pt3\n";
								progress = ++count*100.0 / frames;
								ReleaseMutex(destFileAccess);
								UnmapViewOfFile(source_ptr);
								std::cerr<<"Request::runThisRequest loop pt4\n";
							}
						}
						status = (result == WAIT_OBJECT_0) ? REQ_COMPLETED : REQ_ABORTED;
				}
	}
	std::cerr<<"Request::runThisRequest exit\n";
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
	ptr = new YUV420toYUV422Converter();
	converters[YUV420toYUV422] = ptr;
}
void Request::releaseConverter(std::pair<unsigned short, Converter*> pair) {
	delete pair.second;
}
void Request::deinitConverters() {
	std::map<unsigned short,Converter*>::iterator begin = converters.begin(), end = converters.end();
	for_each(begin,end,Request::releaseConverter);
}

void Request::sendHeaderData(HANDLE* events, int evNum, unsigned frameSize, unsigned framesQty) {
	std::cerr<<"Request::sendHeaderData enter\n";
	HeaderDataStruct* header_ptr;
	unsigned result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE);
	std::cerr<<"Request::sendHeaderData pt1 result="<<result<<"\n";
	if (result == WAIT_OBJECT_0) {
		std::cerr<<"Request::sendHeaderData pt2\n";
		header_ptr = (HeaderDataStruct*) destFile;
		(*header_ptr).frame_size = frameSize;
		std::cerr<<"Request::sendHeaderData pt3\n";
		(*header_ptr).frames_qty = framesQty;
		std::cerr<<"Request::sendHeaderData pt4\n";
		SetEvent(headerDataWritten);
		ReleaseMutex(destFileAccess);
	}
	std::cerr<<"Request::sendHeaderData exit\n";
}

unsigned Request::pingChannelProc() {
	HANDLE events[2];
	std::cerr<<"Request::PingChannelProc enter\n";
	ResponseDataStruct* resp_ptr;
	events[REQ_FLAG_INDEX] = pingReq;
	events[END_FLAG_INDEX] = endFlag;
	unsigned result;

	std::cerr<<"Request::PingChannelProc pt1\n";
	for (;in_progress && (!(result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE)));) {
		std::cerr<<"Request::PingChannelProc pt2\n";
		resp_ptr =(ResponseDataStruct*) pingChannel;
		(*resp_ptr).progress = progress;
		(*resp_ptr).req_status = status;
		std::cerr<<"Request::PingChannelProc pt3\n";
		SetEvent(pingNotify);
		std::cerr<<"Request::PingChannelProc pt4\n";
	}
	std::cerr<<"Request::PingChannelProc exit\n";
	return 0;
}

unsigned Request::pingChannelThread(void* p) {
	Request* ptr = (Request*) p;
	return (*ptr).pingChannelProc();
}
