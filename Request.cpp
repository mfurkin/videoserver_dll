/*
 * Request.cpp
 *
 *  Created on: 12 но€б. 2017 г.
 *
 */

#include "Request.h"

HANDLE Request::endFlag=NULL;
Server* Request::server_ptr = NULL;
std::string Request::REQUEST_MAIN_THREAD="Request_main_thread";
std::map<unsigned short, Converter*> Request::converters;
Request::Request(std::string aPingName):pingName(aPingName),inited(FALSE),in_progress(0),timeout_count(10) {
	logString("Request ctor pingName=",pingName);
	HANDLE hPingChannel = OpenFileMapping(FILE_MAP_WRITE,FALSE,pingName.c_str());
	if (!(hPingChannel))
		logPtr("Request could not be  executed: ping channel could not be open, error code=",GetLastError());
	else {
		pingChannel = (uint8_t*) MapViewOfFile(hPingChannel,FILE_MAP_ALL_ACCESS,0,0,sizeof(RequestDataStruct));
		if (!(pingChannel))
			logPtr("Request could not be executed: Ping channel could not be mapped error code=",GetLastError());
		else {
			RequestDataStruct* req_ptr = (RequestDataStruct*) pingChannel;
			HANDLE hSourceFile = CreateFile((*req_ptr).source_name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hSourceFile == INVALID_HANDLE_VALUE)
				logPtr("Request could  not be executed: Source file could not be open error code= ",GetLastError());
			else {
				width = (*req_ptr).width;
				height = (*req_ptr).height;
				unsigned res3  = GetFileSizeEx(hSourceFile,&fileSize);
				if (!(res3))
					logPtr("Source file size could not be got error code=",GetLastError());
				else {
					unsigned size = width*height*COLORS_NUM;
					hMapSource = CreateFileMapping(hSourceFile,NULL,PAGE_READONLY,0,0,NULL);
					if (hMapSource == INVALID_HANDLE_VALUE)
						logPtr("Request could not be executed: file mapping for source file could not be created error code=",GetLastError());
					else {
						HANDLE hDestFile = OpenFileMapping(FILE_MAP_WRITE,FALSE,(*req_ptr).dataName);
						if (hDestFile == INVALID_HANDLE_VALUE)
							logPtr("Request could not be executed: dest file mappping could not be open error code=",GetLastError());
						else {
							conv_type = (*req_ptr).conv_type;
							destFile = (uint8_t*) MapViewOfFile(hDestFile,FILE_MAP_WRITE,0,0,size+sizeof(unsigned long));
							if(!(destFile))
								logPtr("Request ctor: dest sharedmemory could not be got error code:",GetLastError());
							pingReq = OpenEvent(SYNCHRONIZE,FALSE,(*req_ptr).pingReqName);
							pingNotify = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).pingNotifyName);
							logString("Request ctor destFileAccessName=",(*req_ptr).destFileAccessName);
							writeEnabled = OpenEvent(SYNCHRONIZE,FALSE,(*req_ptr).writeEnabledName);
							writeCompleted = OpenEvent(EVENT_MODIFY_STATE,FALSE,(*req_ptr).writeCompletedName);
							inited = destFile && pingReq && pingNotify && writeEnabled && writeCompleted;
							if (inited)
								log("Request ctor have been completed  successfully.");
							else {
								log("Some errors have been got during Request ctor");
								logPtr("Request ctor destFile=",(unsigned)destFile);
								logPtr("Request ctor pingReq=",(unsigned)pingReq);
								logPtr("Request ctor  pingNotify=",(unsigned)pingNotify);
								logPtr("Request ctor writeCompleted=",(unsigned)writeCompleted);
								logPtr("Request ctor writeEnabled=",(unsigned)writeEnabled);
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
	logPtr("Request::runThisRequest enter inited=",inited);
	if (inited) {
		in_progress = 1;
		_beginthreadex(NULL,0,pingChannelThread,this,0,NULL);
		log("Request::runThisRequest pt1");
				uint8_t* source_ptr;
				cur_ptr.QuadPart = 0;
				status = REQ_CONV_IN_PROGRESS;
				logPtr("Request::runThisRequest pt2 convtype=",conv_type);
				Converter* conv_ptr = converters[conv_type];
				if (!(conv_ptr))
					log("Request::runThisRequest unknown converion type!");
				else {
						unsigned picSize = (*conv_ptr).getSourceSize(width,height);
						unsigned count = 0,frames;
						unsigned long out_size = (*conv_ptr).getDestSize(width,height);
						volatile int req_rcvd;
						frames = fileSize.QuadPart / out_size;
						logPtr("Request::runThisRequest before loop size=",fileSize.QuadPart);
						logPtr("Request::runThisRequest before loop cur_ptr=",cur_ptr.QuadPart);
						logPtr("Request::runThisRequest before loop picSize=",picSize);
						req_rcvd = 0;
						for (sendHeaderData(out_size,frames);(in_progress) && (cur_ptr.QuadPart < fileSize.QuadPart) && waitDataRequest(req_rcvd);) {
							log("Request::runThisRequest loop pt1");
							source_ptr = (uint8_t*)MapViewOfFile(hMapSource,FILE_MAP_READ,cur_ptr.HighPart,cur_ptr.LowPart,picSize);
							if (req_rcvd) {
								if (!(source_ptr)) {
									logPtr("Request::runThisRequest error during MapViewOfFile error code=",GetLastError());
									in_progress = 0;
								} else {
									(*conv_ptr).convert(source_ptr,destFile,width,height);
									log("Request::runThisRequest loop pt3");
									progress = ++count*100.0 / frames;
									SetEvent(writeCompleted);
									req_rcvd = 0;
									UnmapViewOfFile(source_ptr);
									log("Request::runThisRequest loop pt4");
								}
							}
						}
						in_progress = 0;
						if (status != REQ_ABORTED)
							status = REQ_COMPLETED;
				}
	}
	log("Request::runThisRequest exit");
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
	ptr = new YUV420toRGBConverter(server_ptr);
	converters[YUV420toRGB24] = ptr;
	ptr = new YUV420toYUV422Converter(server_ptr);
	converters[YUV420toYUV422] = ptr;
}
void Request::releaseConverter(std::pair<unsigned short, Converter*> pair) {
	delete pair.second;
}
void Request::deinitConverters() {
	std::map<unsigned short,Converter*>::iterator begin = converters.begin(), end = converters.end();
	for_each(begin,end,Request::releaseConverter);
}

void Request::sendHeaderData(unsigned frameSize, unsigned framesQty) {
	log("Request::sendHeaderData enter");
	HeaderDataStruct* header_ptr;
	HANDLE events[2];
	events[0] = writeCompleted;
	events[1] = endFlag;
	unsigned result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE);
	logPtr("Request::sendHeaderData pt1 result=",result);
	if (result == WAIT_OBJECT_0) {
		log("Request::sendHeaderData pt2");
		header_ptr = (HeaderDataStruct*) destFile;
		(*header_ptr).frame_size = frameSize;
		log("Request::sendHeaderData pt3");
		(*header_ptr).frames_qty = framesQty;
		log("Request::sendHeaderData pt4");
		SetEvent(writeCompleted);
	}
	log("Request::sendHeaderData exit");
}

int Request::waitDataRequest(volatile int& req_rcvd) {
	static HANDLE events[2] = {writeEnabled,endFlag};
	unsigned result;
	int res;
	result = WaitForMultipleObjectsEx(3,events,FALSE,REQ_TIMEOUT,TRUE);
	if (result == WAIT_FAILED) {
			status = REQ_ABORTED;
			res = 0;
	} else
		if (result == WAIT_TIMEOUT) {
				status = REQ_ABORTED;
				res = timeout_count--;
		} else
				if (result >= WAIT_ABANDONED) {
					res = FALSE;
					status = REQ_ABORTED;
				} else {
					unsigned index = result - WAIT_OBJECT_0;
					if (index == END_INDEX)
						res = 0;
					else {
						res = 1;
						req_rcvd = 1;
					}
				}
	return res;
}
void Request::log(std::string msg) {
	if (server_ptr)
		(*server_ptr).log(REQUEST_MAIN_THREAD,msg);
}

void Request::logPtr(std::string msg, unsigned ptr) {
	if (server_ptr)
		(*server_ptr).logPtr(REQUEST_MAIN_THREAD,msg,ptr);
}

void Request::logString(std::string msg, std::string msg2) {
	if (server_ptr)
		(*server_ptr).logString(REQUEST_MAIN_THREAD,msg,msg2);
}

unsigned Request::pingChannelProc() {
	HANDLE events[2];
	log("Request::PingChannelProc enter");
	ResponseDataStruct* resp_ptr;
	events[REQ_FLAG_INDEX] = pingReq;
	events[END_FLAG_INDEX] = endFlag;
	unsigned result;

	log("Request::PingChannelProc pt1");
	for (;in_progress && (!(result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE)));) {
		log("Request::PingChannelProc pt2");
		resp_ptr =(ResponseDataStruct*) pingChannel;
		(*resp_ptr).progress = progress;
		(*resp_ptr).req_status = status;
		log("Request::PingChannelProc pt3");
		SetEvent(pingNotify);
		log("Request::PingChannelProc pt4");
	}
	log("Request::PingChannelProc exit");
	return 0;
}

unsigned Request::pingChannelThread(void* p) {
	Request* ptr = (Request*) p;
	return (*ptr).pingChannelProc();
}

void Request::setServerPtr(Server* aServerPtr) {
	server_ptr = aServerPtr;
}


