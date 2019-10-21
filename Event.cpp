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
	msOccurrenceTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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

void Event::print() {
	getRFID().print();
	std::cout << "," << getOccurrenceTime() << "," << static_cast<std::underlying_type<PROCESS>::type>(getProcess());
}

bool Event::operator < (const Event& a_event) const
{
	return (msOccurrenceTime < a_event.msOccurrenceTime);
}