#include "Event.h"

Event::Event() {
	rfid.setRFID(0);
	rfid.setTail((unsigned char*)NULL);
	rfid.initiateTailPointer();
	msOccurrenceTime = 0;
	iProcess = PROCESS::intothechain;
}

void Event::clear() {
	rfid.setRFID(0);
	rfid.setTail((unsigned char*)NULL);
	rfid.initiateTailPointer();
	msOccurrenceTime = 0;
	iProcess = PROCESS::intothechain;
}

Event::Event(const Event& a_Event) {
	rfid = a_Event.rfid;
	msOccurrenceTime = a_Event.msOccurrenceTime;
	iProcess = a_Event.iProcess;
};

void Event::setValue(RFID a_RFID, PROCESS a_iProcess) {
	rfid = a_RFID;
	msOccurrenceTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	iProcess = a_iProcess;
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

string Event::print() {
	ostringstream strbufMessage;
	strbufMessage << getRFID().print();
	strbufMessage << "," << getOccurrenceTime() << "," << static_cast<underlying_type<PROCESS>::type>(getProcess());
	return strbufMessage.str();
}

bool Event::operator < (const Event& a_event) const
{
	return (msOccurrenceTime < a_event.msOccurrenceTime);
}