#include "Database.h"

/// <summary>
/// insertEvent.
/// Add an event from a RFID into the event database (eventdb).
/// </summary>
void Database::insertEvent(Event a_Event) {
	mdb.lock();
	eventdb.insert(std::pair<int, Event>(a_Event.getRFID().getRFID(), a_Event));
	mdb.unlock();
}

/// <summary>
/// getRfidEvents.
/// Retrieve all the events belonging to a RFID (a_iRfid). Events are sorted in ascending msOccurrenceTime and stored in a_vResult.
/// </summary>
void Database::getRfidEvents(std::vector<Event> &a_vResult, int a_iRfid) {
	a_vResult.clear();
	std::pair<dbItr, dbItr> itrResult = eventdb.equal_range(a_iRfid);

	// Iterate over the range
	for (dbItr it = itrResult.first; it != itrResult.second; it++)
		a_vResult.push_back((Event)it->second);
		
	// Sort them in msOccurrenceTime (ascending)
	std::sort(a_vResult.begin(), a_vResult.end());

	return;
}

/// <summary>
/// print.
/// Print all events in the event database.
/// </summary>
void Database::print() {
	for (dbItr it = eventdb.begin(); it != eventdb.end(); ++it) {
		std::cout << it->first << ": ";
		((Event)(it->second)).print();
		std::cout << "\n";
	}
}
