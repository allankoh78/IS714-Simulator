#ifndef __EVENT_H_INCLUDED__   
#define __EVENT_H_INCLUDED__   

#include <chrono>
#include <sstream>
#include "Constant.h"
#include "RFID.h"
// A basic event comprise of (ID, T, L, S, TT, TP, TF) 
//	ID: Tag ID, T: Occurrence Time, L: Location, S: Process (receiving, stocking, shipping),
//	TT: Tag Tail, TP: Tail Pointer, TF = Tail Flag
// Two special events are created when tags enter the supply chain(an into-the-chain event,
// created at the manufacturer when tags and IDs are assigned to products) and when tags leave the chain(out-of-the-chain event, created at the retailer).
// * We will ignore the location and tail flag.
class Event
{
private:
	RFID rfid; // ID, TT, TP
	unsigned __int64 msOccurrenceTime; // T
	PROCESS iProcess; // S
	bool bCloneAttacked;
public:
	Event();
	Event(const Event& a_Event);
	void setValue(RFID a_Rfid, PROCESS a_iProcess, bool a_bCloneAttacked);
	RFID getRFID();
	unsigned __int64 getOccurrenceTime();
	PROCESS getProcess();
	bool getCloneAttacked();
	string print();
	bool operator<(const Event& a_event) const;
	void clear();
};
#endif