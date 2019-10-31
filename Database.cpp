#include "Database.h"

/// <summary>
/// insertEvent.
/// Add an event from a RFID into the event database (eventdb).
/// </summary>
void Database::insertEvent(Event a_Event) {
	mdb.lock();
	eventdb.insert(pair<int, Event>(a_Event.getRFID().getRFID(), a_Event));
	mdb.unlock();
}

/// <summary>
/// getRfidEvents.
/// Retrieve all the events belonging to a RFID (a_iRfid). Events are sorted in ascending msOccurrenceTime and stored in a_vResult.
/// </summary>
void Database::getRfidEvents(vector<Event> &a_vResult, int a_iRfid) {
	a_vResult.clear();
	pair<dbItr, dbItr> itrResult = eventdb.equal_range(a_iRfid);

	// Iterate over the range
	for (dbItr it = itrResult.first; it != itrResult.second; it++)
		a_vResult.push_back((Event)it->second);
		
	// Sort them in msOccurrenceTime (ascending)
	sort(a_vResult.begin(), a_vResult.end());

	return;
}

/// <summary>
/// print.
/// Print all events in the event database.
/// </summary>
string Database::print() {
	ostringstream strbufMessage;
	for (dbItr it = eventdb.begin(); it != eventdb.end(); ++it) {
		strbufMessage << it->first << ": ";
		strbufMessage << ((Event)(it->second)).print();
		strbufMessage << "\n";
	}
	return strbufMessage.str();
}
