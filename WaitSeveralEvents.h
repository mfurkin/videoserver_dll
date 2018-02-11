/*
 * WaitSeveralEvents.h
 *
 *  Created on: 12 ����. 2018 �.
 *      Author: ����
 */

#ifndef WAITSEVERALEVENTS_H_
#define WAITSEVERALEVENTS_H_
#include <winbase.h>
#include <windef.h>
class WaitSeveralEvents {
public:
	WaitSeveralEvents(int anEvenetsQty, HANDLE anEndFlag);
	virtual ~WaitSeveralEvents();
	int registerMe();
	void setEvent(int index);
	int waitAllThreads();
private:
	HANDLE* events;
	int first_free,events_qty;
};

#endif /* WAITSEVERALEVENTS_H_ */
