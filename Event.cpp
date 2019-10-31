#include "Event.h"

Event::Event() {
	clear();
}

void Event::clear() {
	rfid.reset();
	msOccurrenceTime = 0;
	iProcess = PROCESS::intothechain;
	bCloneAttacked = false;
}

Event::Event(const Event& a_Event) {
	rfid = a_Event.rfid;
	msOccurrenceTime = a_Event.msOccurrenceTime;
	iProcess = a_Event.iProcess;
	bCloneAttacked = a_Event.bCloneAttacked;
};

void Event::setValue(RFID a_RFID, PROCESS a_iProcess, bool a_bCloneAttacked) {
	rfid = a_RFID;
	msOccurrenceTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	iProcess = a_iProcess;
	bCloneAttacked = a_bCloneAttacked;
}

RFID Event::getRFID() {
	return rfid;
}

unsigned __int64 Event::getOccurrenceTime(){
	return msOccurrenceTime;
}

PROCESS Event::getProcess() {
	return iProcess;
}

bool Event::getCloneAttacked() {
	return bCloneAttacked;
}

string Event::print() {
	ostringstream strbufMessage;
	strbufMessage << getRFID().print();
	strbufMessage << "," << getOccurrenceTime() << "," << static_cast<underlying_type<PROCESS>::type>(getProcess());
	strbufMessage << (getCloneAttacked()==true ? ",Clone Attacked" : "");
	return strbufMessage.str();
}

bool Event::operator < (const Event& a_event) const
{
	return (msOccurrenceTime < a_event.msOccurrenceTime);
}