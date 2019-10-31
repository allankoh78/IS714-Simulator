#include "DatabasePro.h"

/// <summary>
/// insertEvent.
/// Add an event from a RFID into the event database (eventdb).
/// </summary>
void DatabasePro::insertEventPro(EventPro a_EventPro) {
	mdb.lock();
	eventProdb.insert(pair<int, EventPro>(a_EventPro.getRFIDPro().getRFIDProID(), a_EventPro));
	mdb.unlock();
}

/// <summary>
/// getRfidEvents.
/// Retrieve all the events belonging to a RFID (a_iRfidPro). Events are sorted in ascending msOccurrenceTime and stored in a_vResult.
/// </summary>
void DatabasePro::getRfidProEvents(vector<EventPro>& a_vResult, int a_iRfidPro) {
	a_vResult.clear();
	pair<dbProItr, dbProItr> itrResult = eventProdb.equal_range(a_iRfidPro);

	// Iterate over the range
	for (dbProItr it = itrResult.first; it != itrResult.second; it++)
		a_vResult.push_back((EventPro)it->second);

	// Sort them in msOccurrenceTime (ascending)
	sort(a_vResult.begin(), a_vResult.end());

	return;
}

/// <summary>
/// print.
/// Print all events in the event database.
/// </summary>
string DatabasePro::print() {
	ostringstream strbufMessage;
	for (dbProItr it = eventProdb.begin(); it != eventProdb.end(); ++it) {
		strbufMessage << it->first << ": ";
		strbufMessage << ((EventPro)(it->second)).print();
		strbufMessage << "\n";
	}
	return strbufMessage.str();
}
