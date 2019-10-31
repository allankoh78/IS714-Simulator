// Simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream> 
#include "RFID.h"
#include "Detector.h"
#include "Reader.h"
#include "Database.h"
#include "Tester.h"
#include "Logfile.h"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	char *result = GetCurrentDir(buff, FILENAME_MAX);
	string current_working_dir(buff);
	return current_working_dir;
}

void rfidmover(RFID& a_RFID, Reader a_aReader[NUMOFREADER], Database& db, Reader& a_ReaderMalicious, Logfile& a_logfile);

void rfidmover(RFID& a_RFID, Reader a_aReader[NUMOFREADER], Database& db, Reader& a_ReaderMalicious, Logfile& a_logfile) {
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
	if (iCloneCount > 0) {
		ostringstream strbufMessage;
		strbufMessage << "RFID " << a_RFID.getRFID() << " produced " << iCloneCount << " clone(s).\n";
		a_logfile.write(strbufMessage.str());
		strbufMessage.clear();
		strbufMessage.str("");
	}
}

int main()
{
	Logfile logfile;
	ostringstream strbufMessage;
	string strLogFilePathName;
	#ifdef WINDOWS
		strLogFilePathName = GetCurrentWorkingDir() + "\\logfile.txt";
	#else
		strLogFilePathName = logfile.setFilePath(GetCurrentWorkingDir() + "//logfile.txt");
	#endif

	if ( logfile.setFilePath(strLogFilePathName) != 0){
		printf("Error opening log file %s\n", strLogFilePathName.c_str());
		return -1;
	}

	#ifdef TESTER
	Tester tester;
	tester.runTest(logfile);
	logfile.close();
	return 1;
	#else

	// Prepare detector and RFIDs. Detector generate the random values for RFIDs.
	RFID *rfids = new RFID[NUMOFRFID];
	Detector detector;
	detector.setSeed(RANDOMSEED);
	detector.initiateRandomChar();
	detector.initiateRFIDArray(NUMOFRFID, rfids);

	// Prepare readers and malicious reader.
	Reader* readers = new Reader[NUMOFREADER];
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
	vector<thread> threads;
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) 		
		threads.push_back(thread(rfidmover, ref(rfids[iIndex]), ref(readers), ref(db), ref(readerMalicious), ref(logfile)));
		
	// Wait for all RFID to complete their travelling.
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++)
		threads[iIndex].join();
	
	// Check the results.
	vector<Event> vResult;
	vector<RFID> vCloneRfid;
	size_t iTotalPossibleCloneEvents = 0;
	bool bCloneRFIDTail[NUMOFRFID];
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) {
		db.getRfidEvents(vResult, rfids[iIndex].getRFID());
		bCloneRFIDTail[iIndex] = detector.isValidRFIDTailEvents(vResult, vCloneRfid, logfile);
		iTotalPossibleCloneEvents += vCloneRfid.size();
		if (bCloneRFIDTail[iIndex] == false) { // To print the RFID results.
			Event currentEvent;
			logfile.write("RFID, Tail Value, Tail Pointer, Timestamp, Operation\n");
			for (vector<Event>::iterator it = vResult.begin(); it != vResult.end(); ++it) {
				currentEvent = *it;
				logfile.write(currentEvent.print()+"\n");
			}
		}
		strbufMessage << "RFID (" << rfids[iIndex].getRFID() << ") - Clone detected: " << (bCloneRFIDTail[iIndex] ? "False" : "True") << "(" << vCloneRfid.size() << ")\n";
		logfile.write(strbufMessage.str());
		strbufMessage.clear();
		strbufMessage.str("");
		vResult.clear();
		vCloneRfid.clear();
	}
	strbufMessage << "Total possible clone events : " << iTotalPossibleCloneEvents << "\n";
	strbufMessage << "Total truth clone events : " << detector.getTruthNumberofClone() << "\n";
	logfile.write(strbufMessage.str());
	strbufMessage.clear();
	strbufMessage.str("");
	logfile.close();

	printf("Output file is at %s\n", strLogFilePathName.c_str());
	return 1;
	#endif
}

