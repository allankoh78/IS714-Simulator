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
#include "RFIDPro.h"
#include "DetectorPro.h"
#include "ReaderPro.h"
#include "DatabasePro.h"

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
void rfidPromover(RFIDPro& a_RFIDPro, ReaderPro a_aReaderPro[NUMOFREADER], DatabasePro& dbPro, ReaderPro& a_ReaderProMalicious, Logfile& a_logfile);

void rfidmover(RFID& a_RFID, Reader a_aReader[NUMOFREADER], Database& db, Reader& a_ReaderMalicious, Logfile& a_logfile) {
	int iCloneCount = 0;
	int iClone = 0;
	RFID cloneRFID;
	bool bCloneAttack = false;
	a_aReader[a_RFID.getReaderSequence(0)-1].read(a_RFID, PROCESS::intothechain, db, false);
	for (int iReaderSequenceIndex = 1; iReaderSequenceIndex < NUMOFREADER - 1; iReaderSequenceIndex++) {
		cloneRFID.reset();
		bCloneAttack = false;
		do {			
			iClone = a_ReaderMalicious.toCloneRfid(a_RFID, cloneRFID, db);
			iCloneCount += iClone;
			if ( iClone == 1) bCloneAttack = true;
		} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);
		a_aReader[a_RFID.getReaderSequence(iReaderSequenceIndex)-1].read(a_RFID, PROCESS::shipping, db, bCloneAttack);
	}
	cloneRFID.reset();
	/*do {
		iClone = a_ReaderMalicious.toCloneRfid(a_RFID, cloneRFID, db);
		iCloneCount += iClone;
	} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);*/
	a_aReader[a_RFID.getReaderSequence(NUMOFREADER - 1)-1].read(a_RFID, PROCESS::outofthechain, db, false);
	if (iCloneCount > 0) {
		ostringstream strbufMessage;
		strbufMessage << "RFID " << a_RFID.getRFIDID() << " produced " << iCloneCount << " clone(s).\n";
		a_logfile.write(strbufMessage.str());
		strbufMessage.clear();
		strbufMessage.str("");
	}
}

void rfidPromover(RFIDPro& a_RFIDPro, ReaderPro a_aReaderPro[NUMOFREADER], DatabasePro& dbPro, ReaderPro& a_ReaderProMalicious, Logfile& a_logfile) {
	int iCloneCount = 0;
	int iClone = 0;
	RFIDPro cloneRFIDPro;
	bool bCloneAttack = false;
	a_aReaderPro[a_RFIDPro.getReaderSequence(0) - 1].read(a_RFIDPro, PROCESS::intothechain, dbPro, false);
	for (int iReaderProSequenceIndex = 1; iReaderProSequenceIndex < NUMOFREADER - 1; iReaderProSequenceIndex++) {
		cloneRFIDPro.reset();
		bCloneAttack = false;
		do {
			iClone = a_ReaderProMalicious.toCloneRfidPro(a_RFIDPro, cloneRFIDPro, dbPro);
			iCloneCount += iClone;
			if (iClone == 1) bCloneAttack = true;
		} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);
		a_aReaderPro[a_RFIDPro.getReaderSequence(iReaderProSequenceIndex) - 1].read(a_RFIDPro, PROCESS::shipping, dbPro, bCloneAttack);
	}
	cloneRFIDPro.reset();
	/*do {
		iClone = a_ReaderProMalicious.toCloneRfidPro(a_RFIDPro, cloneRFIDPro, dbPro);
		iCloneCount += iClone;
	} while (iClone == 1 && iCloneCount < MAXCLONEPERCYCLE);*/
	a_aReaderPro[a_RFIDPro.getReaderSequence(NUMOFREADER - 1) - 1].read(a_RFIDPro, PROCESS::outofthechain, dbPro, false);
	if (iCloneCount > 0) {
		ostringstream strbufMessage;
		strbufMessage << "RFIDPro " << a_RFIDPro.getRFIDProID() << " produced " << iCloneCount << " clone(s).\n";
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
	#endif

	#ifdef ORIGINAL
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
		db.getRfidEvents(vResult, rfids[iIndex].getRFIDID());
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
		strbufMessage << "RFID (" << rfids[iIndex].getRFIDID() << ") - Clone detected: " << (bCloneRFIDTail[iIndex] ? "False" : "True") << "(" << vCloneRfid.size() << ")\n";
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
	#endif

	#ifdef PROPOSAL
	// Prepare detector and RFIDs. Detector generate the random values for RFIDs.
	RFIDPro* rfidPros = new RFIDPro[NUMOFRFID];
	DetectorPro detectorPro;
	detectorPro.setSeed(RANDOMSEED);
	detectorPro.initiateRandomSymbol();
	detectorPro.initiateRFIDProArray(NUMOFRFID, rfidPros);

	// Prepare readers and malicious reader.
	ReaderPro* readerPros = new ReaderPro[NUMOFREADER];
	ReaderPro readerProMalicious;
	for (int iIndex = 0; iIndex < NUMOFREADER; iIndex++) {
		readerPros[iIndex].setReaderProID(iIndex + 1);
		readerPros[iIndex].setSeed(RANDOMSEED + iIndex + 1);
		readerPros[iIndex].initiateRandomSymbol();
	}
	readerProMalicious.setReaderProID(0);
	readerProMalicious.setSeed(RANDOMSEED);
	readerProMalicious.initiateRandomSymbol();

	// Start the simulation of RFID travelling across different readers.
	DatabasePro dbPro;
	vector<thread> threads;
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++)
		threads.push_back(thread(rfidPromover, ref(rfidPros[iIndex]), ref(readerPros), ref(dbPro), ref(readerProMalicious), ref(logfile)));

	// Wait for all RFID to complete their travelling.
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++)
		threads[iIndex].join();

	// Check the results.
	vector<EventPro> vResultPro;
	vector<RFIDPro> vCloneRfidPro;
	size_t iTotalPossibleCloneEventPros = 0;
	bool bCloneRFIDProTail[NUMOFRFID];
	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) {
		dbPro.getRfidProEvents(vResultPro, rfidPros[iIndex].getRFIDProID());
		bCloneRFIDProTail[iIndex] = detectorPro.isValidRFIDProTailEvents(vResultPro, vCloneRfidPro, logfile);
		iTotalPossibleCloneEventPros += vCloneRfidPro.size();
		if (bCloneRFIDProTail[iIndex] == false) { // To print the RFID results.
			EventPro currentEventPro;
			logfile.write("RFID, Tail Value, Tail Pointer, Timestamp, Operation\n");
			for (vector<EventPro>::iterator it = vResultPro.begin(); it != vResultPro.end(); ++it) {
				currentEventPro = *it;
				logfile.write(currentEventPro.print() + "\n");
			}
		}
		strbufMessage << "RFID (" << rfidPros[iIndex].getRFIDProID() << ") - Clone detected: " << (bCloneRFIDProTail[iIndex] ? "False" : "True") << "(" << vCloneRfidPro.size() << ")\n";
		logfile.write(strbufMessage.str());
		strbufMessage.clear();
		strbufMessage.str("");
		vResultPro.clear();
		vCloneRfidPro.clear();
	}
	strbufMessage << "Total possible clone events : " << iTotalPossibleCloneEventPros << "\n";
	strbufMessage << "Total truth clone events : " << detectorPro.getTruthNumberofClone() << "\n";
	logfile.write(strbufMessage.str());
	strbufMessage.clear();
	strbufMessage.str("");
	#endif

	logfile.close();
	printf("Output file is at %s\n", strLogFilePathName.c_str());
	return 1;
}

