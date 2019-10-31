#ifndef __DATABASEPRO_H_INCLUDED__   
#define __DATABASEPRO_H_INCLUDED__   
#include <map>
#include <iterator> 
#include <vector>
#include <algorithm>
#include <sstream>
#include <mutex>

#include "EventPro.h"

typedef map<int, EventPro>::iterator dbProItr;

class DatabasePro
{
private:
	multimap<int, EventPro> eventProdb; // Store all the events by all RFID.
	mutex mdb;

public:
	void insertEventPro(EventPro a_EventPro);
	void getRfidProEvents(vector<EventPro>& a_vResult, int a_iRfidPro);
	string print();
};
#endif