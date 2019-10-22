// Simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Tester.h"

void Tester::runTest()
{
	// Test 01: To compare tail values and tail pointer, and to determine if the comparision test (detector.isValidRFID) is working.
	if (ComparisionTest() == true)
		std::cout << "ComparisionTest Passed.\n";
	else
		std::cout << "ComparisionTest Failed.\n";
		
	// Test 02: To prepare the pool of random strings for all RFIDs' tails (detector.initiateRandomChar), 
	// and to determine if this pool is properly assigned to the array of RFID (detector.initiateRFIDArray).
	RFID aRfid[NUMOFRFID];
	Detector detector;
	detector.setSeed(RANDOMSEED);
	detector.initiateRandomChar();
	// Initiate the RFID tail, tail pointer and RFID id.
	detector.initiateRFIDArray(NUMOFRFID, aRfid);
	if (InitializationTest(detector, aRfid) == true)
		std::cout << "InitializationTest (Detector) Passed.\n";
	else
		std::cout << "InitializationTest (Detector) Failed.\n";

	// Test 03: To prepare the pool of random string for readers (reader.initiateRandomChar), and to determine if the function works properly.
	Reader readers[6];
	for (int iIndex = 0; iIndex < 6; iIndex++) {
		readers[iIndex].setReaderID(iIndex + 1);
		readers[iIndex].setSeed(RANDOMSEED + iIndex + 1);
		readers[iIndex].initiateRandomChar();
	}
	if (InitializationTest(readers) == true)
		std::cout << "InitializationTest (Readers) Passed.\n";
	else
		std::cout << "InitializationTest (Readers) Failed.\n";
	   
	// Test 04: Test readers on rfid.
	if (ReaderRFIDTest(readers, aRfid, detector ) == true)
		std::cout << "ReaderRFIDTest Passed.\n";
	else
		std::cout << "ReaderRFIDTest Failed.\n";

	return;
}

bool Tester::ReaderRFIDTest(Reader a_pReader[], RFID a_pRFID[], Detector a_Detector) {
	bool bResult = true;
	Event event;
	Event cloneEvent;
	Database db;
	int iCloneCount = 0;
	RFID cloneRFID;
	/* You need to set these setting for the test to work correctly. 
	static const int NUMBEROFRANDOMCHAR = 1000;
	static const int TAILSIZE = 3;
	static const int NUMOFRFID = 1000; //10;
	static const int RANDOMSEED = 123456;
	static const int NUMOFREADER = 4; //10;
	static const int MAXCLONEPERCYCLE = 2;
	static const int CLONEAGRESSIVE = 2; // Must be non-zero ; 1 is the most agressive ; Higher the number, lesser the agressiveness. 
	*/

	for (int iRFIDIndex = 1; iRFIDIndex < 6; iRFIDIndex++) {
		std::cout << "Testing RFID (" << iRFIDIndex << "):\n";
		srand(iRFIDIndex);
		cloneRFID.reset();

		a_pReader[0].read(a_pRFID[iRFIDIndex], PROCESS::intothechain, db);
		//std::thread th1(&Reader::read, &a_pReader[0], std::ref(a_pRFID[iRFIDIndex]), PROCESS::intothechain, std::ref(db));
		//th1.join();
		iCloneCount += a_pReader[4].toCloneRfid(a_pRFID[iRFIDIndex], cloneRFID, db);
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));

		a_pReader[1].read(a_pRFID[iRFIDIndex], PROCESS::shipping, db);
		iCloneCount += a_pReader[4].toCloneRfid(a_pRFID[iRFIDIndex], cloneRFID, db);
		
		a_pReader[2].read(a_pRFID[iRFIDIndex], PROCESS::receiving, db);
		iCloneCount += a_pReader[4].toCloneRfid(a_pRFID[iRFIDIndex], cloneRFID, db);
		
		a_pReader[3].read(a_pRFID[iRFIDIndex], PROCESS::stocking, db);
		iCloneCount += a_pReader[4].toCloneRfid(a_pRFID[iRFIDIndex], cloneRFID, db);
		
		a_pReader[5].read(a_pRFID[iRFIDIndex], PROCESS::outofthechain, db);
		std::vector<Event> vResult;
		db.getRfidEvents(vResult, a_pRFID[iRFIDIndex].getRFID());
		//db.print();

		std::vector<RFID> vCloneRfid;
		a_Detector.isValidRFIDTailEvents(vResult, vCloneRfid);
		switch (iRFIDIndex) {
		case 1:
			if (vCloneRfid.size() != 5)
				bResult = false;
			else {
				RFID tempRfid = (RFID)vCloneRfid.at(0);
				if (tempRfid.getRFID() != 2 || tempRfid.getTail(1) != 146 || tempRfid.getTail(2) != 254 || tempRfid.getTail(3) != 1 || tempRfid.getTailPointer() != 3)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(1);
				if (tempRfid.getRFID() != 2 || tempRfid.getTail(1) != 143 || tempRfid.getTail(2) != 254 || tempRfid.getTail(3) != 17 || tempRfid.getTailPointer() != 1)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(2);
				if (tempRfid.getRFID() != 2 || tempRfid.getTail(1) != 4 || tempRfid.getTail(2) != 254 || tempRfid.getTail(3) != 1 || tempRfid.getTailPointer() != 1)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(3);
				if (tempRfid.getRFID() != 2 || tempRfid.getTail(1) != 143 || tempRfid.getTail(2) != 67 || tempRfid.getTail(3) != 17 || tempRfid.getTailPointer() != 2)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(4);
				if (tempRfid.getRFID() != 2 || tempRfid.getTail(1) != 4 || tempRfid.getTail(2) != 8 || tempRfid.getTail(3) != 1 || tempRfid.getTailPointer() != 2)
					bResult = false;
			}
			break;
		case 2:
			if (vCloneRfid.size() != 3)
				bResult = false;
			else {
				RFID tempRfid = (RFID)vCloneRfid.at(0);
				if (tempRfid.getRFID() != 3 || tempRfid.getTail(1) != 45 || tempRfid.getTail(2) != 31 || tempRfid.getTail(3) != 28 || tempRfid.getTailPointer() != 3)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(1);
				if (tempRfid.getRFID() != 3 || tempRfid.getTail(1) != 58 || tempRfid.getTail(2) != 31 || tempRfid.getTail(3) != 187 || tempRfid.getTailPointer() != 1)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(2);
				if (tempRfid.getRFID() != 3 || tempRfid.getTail(1) != 24 || tempRfid.getTail(2) != 31 || tempRfid.getTail(3) != 28 || tempRfid.getTailPointer() != 1)
					bResult = false;
			}			
			break;
		case 3:
			if (vCloneRfid.size() != 1)
				bResult = false;
			else {
				RFID tempRfid = (RFID)vCloneRfid.at(0);
				if (tempRfid.getRFID() != 4 || tempRfid.getTail(1) != 144 || tempRfid.getTail(2) != 200 || tempRfid.getTail(3) != 55 || tempRfid.getTailPointer() != 2)
					bResult = false;
			}
			break;
		case 4:
			if (vCloneRfid.size() != 1)
				bResult = false;
			else {
				RFID tempRfid = (RFID)vCloneRfid.at(0);
				if (tempRfid.getRFID() != 5 || tempRfid.getTail(1) != 251 || tempRfid.getTail(2) != 146 || tempRfid.getTail(3) != 145 || tempRfid.getTailPointer() != 1)
					bResult = false;
			}
			break;
		case 5:
			if (vCloneRfid.size() != 5)
				bResult = false;
			else {
				RFID tempRfid = (RFID)vCloneRfid.at(0);
				if (tempRfid.getRFID() != 6 || tempRfid.getTail(1) != 32 || tempRfid.getTail(2) != 102 || tempRfid.getTail(3) != 176 || tempRfid.getTailPointer() != 3)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(1);
				if (tempRfid.getRFID() != 6 || tempRfid.getTail(1) != 98 || tempRfid.getTail(2) != 102 || tempRfid.getTail(3) != 64 || tempRfid.getTailPointer() != 1)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(2);
				if (tempRfid.getRFID() != 6 || tempRfid.getTail(1) != 149 || tempRfid.getTail(2) != 69 || tempRfid.getTail(3) != 176 || tempRfid.getTailPointer() != 2)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(3);
				if (tempRfid.getRFID() != 6 || tempRfid.getTail(1) != 98 || tempRfid.getTail(2) != 187 || tempRfid.getTail(3) != 64 || tempRfid.getTailPointer() != 2)
					bResult = false;
				tempRfid = (RFID)vCloneRfid.at(4);
				if (tempRfid.getRFID() != 6 || tempRfid.getTail(1) != 149 || tempRfid.getTail(2) != 69 || tempRfid.getTail(3) != 218 || tempRfid.getTailPointer() != 3)
					bResult = false;
			}
			break;
		default:
			bResult = false;
			break;
		}

		for (std::vector<RFID>::iterator it = vCloneRfid.begin(); it != vCloneRfid.end(); ++it) {
			std::cout << "Detected clone RFID injection near : ";
			((RFID)(*it)).print();
			std::cout << "\n";
		}

		if (bResult == false)
			break;
	}
	return bResult;
}

bool Tester::InitializationTest(Reader a_Reader[]) {
	bool bResult = true;
	int iRandomCharIndex = 0;

	for (int iIndex = 0; iIndex < NUMOFREADER; iIndex++) {
		if (a_Reader[iIndex].getReaderID() != iIndex + 1) {
			bResult = false;
			break;
		}
		if (a_Reader[iIndex].getSeed() != RANDOMSEED + iIndex + 1) {
			bResult = false;
			break;
		}

		srand(a_Reader[iIndex].getSeed());
		unsigned char ucTemp;
		unsigned char ucPreviousTemp = 0x0;
		for (int iRandomIndex = 0; iRandomIndex < NUMBEROFRANDOMCHAR; iRandomIndex++) {
			do {
				ucTemp = rand() % 256 + 1;
			} while (ucPreviousTemp == ucTemp);
			ucPreviousTemp = ucTemp;
			if (a_Reader[iIndex].getRandomChar(iRandomIndex) != ucTemp) {
				bResult = false;
				break;
			}
		}
	}
	return bResult;
}

bool Tester::InitializationTest(Detector a_Detector, RFID a_pRFID[]) {
	bool bResult = true;
	int iRandomCharIndex = 0;

	for (int iIndex = 0; iIndex < NUMOFRFID; iIndex++) {
		if (a_pRFID[iIndex].getRFID() != iIndex + 1) {
			bResult = false;
			break;
		}
		if (a_pRFID[iIndex].getTailPointer() != 1) {
			bResult = false;
			break;
		}
		for (int iTailIndex = 0; iTailIndex < TAILSIZE; iTailIndex++) {
			if (a_pRFID[iIndex].getTail(iTailIndex + 1) != a_Detector.getRandomChar(iRandomCharIndex + iTailIndex)) {
				bResult = false;
				break;
			}
		}
		iRandomCharIndex += TAILSIZE;
		if (iRandomCharIndex + TAILSIZE >= NUMBEROFRANDOMCHAR) {
			iRandomCharIndex = 0;
		}
	}
	return bResult;
}

bool Tester::ComparisionTest() {
	RFID rfid1, rfid2;
	Detector detector;
	bool bFlag;
	bool bResult = true;

	rfid1.setTail((unsigned char*)"ABC");
	rfid2 = rfid1;

	if ( rfid1.setTail('X') == false)
		bResult = false;
	//std::cout << "Test1: " << std::string((char*)rfid2.getTail(), TAILSIZE) << "(" << rfid2.getTailPointer() << ") ---> " << std::string((char*)rfid1.getTail(), TAILSIZE) << "(" << rfid1.getTailPointer() << ")\n";
	bFlag = detector.isValidRFID(rfid2, rfid1);
	if (bFlag == false)
		bResult = false;
	//std::cout << (bFlag == true ? "Valid" : "Invalid") << "\n";

	rfid2 = rfid1;
	if ( rfid1.setTail('C') != false) // Same value cannot be set.
		bResult = false;

	rfid2 = rfid1;
	if (rfid1.setTail('Z') == false)
		bResult = false;
	rfid2.setTail((unsigned char*)"QBA");
	bFlag = detector.isValidRFID(rfid2, rfid1);
	if (bFlag == true)
		bResult = false;
	
	return bResult;
}
