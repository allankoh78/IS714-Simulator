#ifndef __DATABASE_H_INCLUDED__   
#define __DATABASE_H_INCLUDED__   
#include <map>
#include <iterator> 
#include <vector>
#include <algorithm>
#include <mutex>

#include "Event.h"

typedef std::map<int, Event>::iterator dbItr;

class Database
{
private:
	std::multimap<int, Event> eventdb; // Store all the events by all RFID.
	std::mutex mdb;

public:
	void insertEvent(Event a_Event);
	void getRfidEvents(std::vector<Event>& a_vResult, int a_iRfid);
	void print();
};
#endif