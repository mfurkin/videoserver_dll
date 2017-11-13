/*
 * Request.h
 *
 *  Created on: 12 нояб. 2017 г.
 *      Author: Алёна
 */

#ifndef SERVER_REQUEST_H_
#define SERVER_REQUEST_H_
#include <winsock.h>
class Request {
private:
	SOCKET socket;
public:
	Request(SOCKET aSocket);
	void runRequest();
	virtual ~Request();
};

#endif /* SERVER_REQUEST_H_ */
