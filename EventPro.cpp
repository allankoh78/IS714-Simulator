#include "EventPro.h"

EventPro::EventPro() {
	clear();
}

void EventPro::clear() {
	rfidPro.reset();
	msOccurrenceTime = 0;
	iProcess = PROCESS::intothechain;
	bCloneAttacked = false;
}

EventPro::EventPro(const EventPro& a_EventPro) {
	rfidPro = a_EventPro.rfidPro;
	msOccurrenceTime = a_EventPro.msOccurrenceTime;
	iProcess = a_EventPro.iProcess;
	bCloneAttacked = a_EventPro.bCloneAttacked;
}

bool EventPro::getCloneAttacked() {
	return bCloneAttacked;
}

void EventPro::setValue(RFIDPro a_RFIDPro, PROCESS a_iProcess, bool a_bCloneAttacked) {
	rfidPro = a_RFIDPro;
	msOccurrenceTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	iProcess = a_iProcess;
	bCloneAttacked = a_bCloneAttacked;
}

RFIDPro EventPro::getRFIDPro() {
	return rfidPro;
}

unsigned __int64 EventPro::getOccurrenceTime(){
	return msOccurrenceTime;
}

PROCESS EventPro::getProcess() {
	return iProcess;
}

string EventPro::print() {
	ostringstream strbufMessage;
	strbufMessage << getRFIDPro().print();
	strbufMessage << "," << getOccurrenceTime() << "," << static_cast<underlying_type<PROCESS>::type>(getProcess());
	strbufMessage << (getCloneAttacked()==true ? ",Clone Attacked" : "");
	return strbufMessage.str();
}

bool EventPro::operator < (const EventPro& a_eventPro) const
{
	return (msOccurrenceTime < a_eventPro.msOccurrenceTime);
}