#ifndef __DATABASE_H_INCLUDED__   
#define __DATABASE_H_INCLUDED__   
#include <map>
#include <iterator> 
#include <vector>
#include <algorithm>
#include <sstream>
#include <mutex>

#include "Event.h"

typedef map<int, Event>::iterator dbItr;

class Database
{
private:
	multimap<int, Event> eventdb; // Store all the events by all RFID.
	mutex mdb;

public:
	void insertEvent(Event a_Event);
	void getRfidEvents(vector<Event>& a_vResult, int a_iRfid);
	string print();
};
#endif