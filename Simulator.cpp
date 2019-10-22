// Simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "RFID.h"
#include "Detector.h"
#include "Reader.h"
#include "Database.h"
#include "Tester.h"

void rfidmover(RFID& a_RFID, Reader a_aReader[NUMOFREADER], Database& db, Reader& a_ReaderMalicious);

void rfidmover(RFID& a_RFID, Reader a_aReader[NUMOFREADER], Database& db, Reader& a_ReaderMalicious) {
	int iCloneCount = 0;
	int iClone = 0;
	RFID cloneRFID;
	a_aReader[a_RFID.getReaderSequence(0)-1].read(a_RFID, PROCESS::intothechain, db);
	for (int iReaderSequenceIndex = 1; iReaderSequenceIndex < NUMOFREADER - 1; iReaderSequenceIndex++) {
		cloneRFID.reset();
		do {			
			iClone = a_ReaderMalicious.toCloneRfid(a_RFID, cloneRFID, db);
			iCloneCount += iClone;
		} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);
		a_aReader[a_RFID.getReaderSequence(iReaderSequenceIndex)-1].read(a_RFID, PROCESS::shipping, db);	
	}
	cloneRFID.reset();
	do {
		iClone = a_ReaderMalicious.toCloneRfid(a_RFID, cloneRFID, db);
		iCloneCount += iClone;
	} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);
	a_aReader[a_RFID.getReaderSequence(NUMOFREADER - 1)-1].read(a_RFID, PROCESS::outofthechain, db);
	if (iCloneCount > 0)
		printf("RFID %d produced %d clone(s).\n", a_RFID.getRFID(), iCloneCount);
}

int main()
{
	//Tester tester;
	//tester.runTest();
	//return 1;
	
	// Prepare detector and RFIDs. Detector generate the random values for RFIDs.
	RFID rfids[NUMOFRFID];
	Detector detector;
	detector.setSeed(RANDOMSEED);
	detector.initiateRandomChar();
	detector.initiateRFIDArray(NUMOFRFID, rfids);

	// Prepare readers and malicious reader.
	Reader readers[NUMOFREADER];
	Reader readerMalicious;
	for (int iIndex = 0; iIndex < NUMOFREADER; iIndex++) {
		readers[iIndex].setReaderID(iIndex + 1);
		readers[iIndex].setSeed(RANDOMSEED + iIndex + 1);
		readers[iIndex].initiateRandomChar();
	}
	readerMalicious.setReaderID(0);
	readerMalicious.setSeed(RANDOMSEED);
	readerMalicious.initiateRandomChar();

	// Start the simulation of RFID travelling across different readers.
	Database db;
	std::vector<std::thread> threads;
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) 		
		threads.push_back(std::thread(rfidmover, std::ref(rfids[iIndex]), std::ref(readers), std::ref(db), std::ref(readerMalicious)));
		
	// Wait for all RFID to complete their travelling.
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++)
		threads[iIndex].join();
	
	// Check the results.
	std::vector<Event> vResult;
	std::vector<RFID> vCloneRfid;
	size_t iTotalPossibleCloneEvents = 0;
	bool bCloneRFIDTail[NUMOFRFID];
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) {
		db.getRfidEvents(vResult, rfids[iIndex].getRFID());
		bCloneRFIDTail[iIndex] = detector.isValidRFIDTailEvents(vResult, vCloneRfid);
		iTotalPossibleCloneEvents += vCloneRfid.size();
		/*if (bCloneRFIDTail[iIndex] == false) { // To print the RFID results.
			Event currentEvent;
			for (std::vector<Event>::iterator it = vResult.begin(); it != vResult.end(); ++it) {
				currentEvent = *it;
				currentEvent.print();
				printf("\n");
			}
		}*/
		printf("RFID (%d) - Clone detected: %s (%zd) \n", rfids[iIndex].getRFID(), bCloneRFIDTail[iIndex] ? "False" : "True", vCloneRfid.size());
		vResult.clear();
		vCloneRfid.clear();
	}
	printf("Total possible clone events: %zd\n", iTotalPossibleCloneEvents);
	printf("Total truth clone events: %i\n", detector.getTruthNumberofClone());
}

