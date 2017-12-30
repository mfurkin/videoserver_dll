/*
 * server_names.h
 *
 *  Created on: 16 нояб. 2017 г.
 *
 */

#ifndef SERVER_NAMES_H_
#define SERVER_NAMES_H_
#include <stdint.h>
#include <string>
#include <windef.h>
const std::string REQ_FLAG_NAME = "videoserver_request";
const std::string REQ_FILE_NAME = "server_input";
const std::string REQ_ENABLED_FLAG = "request_enabled_flag";
const std::string REQ_MUTEX_NAME = "request_mutex";
const std::string END_FLAG_NAME = "end_flag";
const std::string REQ_FLAG_CREATING_MSG = "ReqFlag could not be created";
const std::string END_FLAG_CREATING_MSG = "EndFlag could not be created";
const std::string REQ_ENABLED_FLAG_CREATING_MSG = "ReqEnabledFlag could not be created";
const std::string REQ_MUTEX_CREATING_MSG = "RequestMutex could not be created";

const std::string YUV420toRGB24_st="YUV420toRGB24";
const std::string YUV422toRGB24_st="YUV422toRGB24";
const std::string RGB24toYUV420_st = "RGB24toYUV420";
const std::string RGB24toYUV422_st = "RGB24toYUV422";
const std::string YUV420toYUV422_st = "YUV420toYUV422";

enum { NO_CONV_TYPE = 0, YUV420toRGB24,  YUV422toRGB24, RGB24toYUV420,  RGB24toYUV422, YUV420toYUV422};
enum { COLORS_NUM=3 };
enum { REQ_RECEIVED = 1, REQ_CONV_IN_PROGRESS, REQ_COMPLETED, REQ_ABORTED};
enum {REQ_TIMEOUT = 10000, REQ_TOUT_COUNT=10};

#pragma pack(push,1)
typedef struct {

	unsigned short width,height,conv_type;						//	width - ширина исходной картинки,
																//  height - высота исходной картинки,
																// 	conv_type - тип преобразования в соответствии с CONV_TYPE
	char source_name[MAX_PATH],pingReqName[MAX_PATH],				// sourceName - путь к исходному файлу
		 pingNotifyName[MAX_PATH],dataName[MAX_PATH],				// pingReqName - имя ивента для оповещения о пинге
		 destFileAccessName[MAX_PATH],
		 /*
		 headerDataWrittenName[MAX_PATH],
		 writeRequestName[MAX_PATH];
		 */
		 writeCompletedName[MAX_PATH],writeEnabledName[MAX_PATH];		// pingNotifyName - имя ивента для ответа на пинг
																// dataName - имя shared memory для рез. файла
																// writeEnabledName - имя ивента для разрешения записи в рез. shared memory
																// writeCompletedName - имя ивента оповещеня о записи в shared memory
} RequestDataStruct;

typedef struct {
	unsigned short progress; 			// в процентах
	uint8_t req_status;					// статус в соответствии с REQ_STATUS
}ResponseDataStruct;

typedef struct {
	unsigned long frame_size;
	unsigned frames_qty;				// frame_size - размер рез. кадра, frame_qty - количество кадров
} HeaderDataStruct;

#pragma pack(pop)
#endif /* SERVER_NAMES_H_ */
