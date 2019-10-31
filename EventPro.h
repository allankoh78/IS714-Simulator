#ifndef __EVENTPRO_H_INCLUDED__   
#define __EVENTPRO_H_INCLUDED__   

#include <chrono>
#include <sstream>
#include "Constant.h"
#include "RFIDPro.h"
// A basic event comprise of (ID, T, L, S, TT, TP, TF) 
//	ID: Tag ID, T: Occurrence Time, L: Location, S: Process (receiving, stocking, shipping),
//	TT: Tag Tail, TP: Tail Pointer, TF = Tail Flag
// Two special events are created when tags enter the supply chain(an into-the-chain event,
// created at the manufacturer when tags and IDs are assigned to products) and when tags leave the chain(out-of-the-chain event, created at the retailer).
// * We will ignore the location and tail flag.
class EventPro
{
private:
	RFIDPro rfidPro; // ID, TT, TP
	unsigned __int64 msOccurrenceTime; // T
	PROCESS iProcess; // S
	bool bCloneAttacked;
public:
	EventPro();
	EventPro(const EventPro& a_EventPro);
	void setValue(RFIDPro a_RfidPro, PROCESS a_iProcess, bool a_bCloneAttacked);
	RFIDPro getRFIDPro();
	unsigned __int64 getOccurrenceTime();
	PROCESS getProcess();
	bool getCloneAttacked();
	string print();
	bool operator<(const EventPro& a_eventPro) const;
	void clear();
};
#endif