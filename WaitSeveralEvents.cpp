/*
 * WaitSeveralEvents.cpp
 *
 *  Created on: 12 февр. 2018 г.
 *      Author: Алёна
 */

#include "WaitSeveralEvents.h"


WaitSeveralEvents::WaitSeveralEvents(int anEventsQty, HANDLE anEndFlag):events(NULL),first_free(0),events_qty(anEventsQty) {
	int i;
	*events = new HANDLE [events_qty+1];
	*(events+events_qty) = anEndFlag;
	for(i=0;i<events_qty;)
		*(events+i++) = NULL;
}

WaitSeveralEvents::~WaitSeveralEvents() {
	int i;
	for (i=0;i<events_qty;)
		CloseHandle(*(events+i++));
	delete [] events;
}

int WaitSeveralEvents::registerMe() {
	int result = first_free++;
	events[result] = CreateEvent(NULL,TRUE,FALSE,NULL);
	return result;
}

void WaitSeveralEvents::setEvent(int index) {
	SetEvent(*(events+index));
}

int WaitSeveralEvents::waitAllThreads() {
	unsigned result,in_progress = events_qty;
	for(;(result=WaitForMultipleObjectsEx(events_qty+1,events,FALSE,INFINITE,TRUE)<WAIT_ABANDONED_0) && (in_progress--);) {
		ResetEvent(*(events+result-WAIT_OBJECT_0));
	}
	return (!(in_progress));
}
