#include "Reader.h"

Reader::Reader() {
	iReaderID = 0;
	iSeed = 0;
	iRandomCharIndex = 0;
	memset(pucRandomChar, 0x0, NUMBEROFRANDOMCHAR);
}

Reader::Reader(const Reader& a_Reader) {
	iReaderID = a_Reader.iReaderID;
	iSeed = a_Reader.iSeed;
	iRandomCharIndex = a_Reader.iRandomCharIndex;
	memcpy(pucRandomChar, a_Reader.pucRandomChar, NUMBEROFRANDOMCHAR);
}

void Reader::setReaderID(int a_iReaderID){
	iReaderID = a_iReaderID;
}

int Reader::getReaderID() {
	return iReaderID;
}

void Reader::setSeed(int a_iSeedValue) {
	iSeed = a_iSeedValue;
}

int Reader::getSeed() {
	return iSeed;
}

void Reader::setRandomChar(int a_iIndex, unsigned char a_ucRandom) {	
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMCHAR)
		pucRandomChar[a_iIndex] = a_ucRandom;	
}

unsigned char Reader::getRandomChar(int a_iIndex) {
	mRandomChar.lock();
	unsigned char cResult = NULL;
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMCHAR)
		cResult = pucRandomChar[a_iIndex];	
	mRandomChar.unlock();
	return cResult;
}

Reader& Reader::operator = (const Reader& a_Reader) {
	iReaderID = a_Reader.iReaderID;
	iSeed = a_Reader.iSeed;
	iRandomCharIndex = a_Reader.iRandomCharIndex;
	memcpy(pucRandomChar, a_Reader.pucRandomChar, NUMBEROFRANDOMCHAR);
	return *this;
}

void Reader::initiateRandomChar() {	
	mRandomChar.lock();
	srand(getSeed());
	unsigned char ucTemp = 0x0;
	unsigned char ucPreviousTemp = 0x0;
	for (int iIndex = 0; iIndex < NUMBEROFRANDOMCHAR; iIndex++) {
		do {
			ucTemp = rand() % 256 + 1;
		} while (ucPreviousTemp == ucTemp || ucTemp == 0x0);
		ucPreviousTemp = ucTemp;
		setRandomChar(iIndex, static_cast<unsigned char> (ucTemp));
	}	
	mRandomChar.unlock();
}

void Reader::read(RFID& a_RFID, PROCESS a_Process, Database& a_db) {
	if (iRandomCharIndex >= NUMBEROFRANDOMCHAR)
		iRandomCharIndex = 0;
	bool bSuccess = false;
	unsigned char ucNewchar = 0x0;
	do {
		ucNewchar = getRandomChar(iRandomCharIndex++);
		bSuccess = a_RFID.setTail(ucNewchar);
	} while (bSuccess == false);
	Event newEvent;
	newEvent.setValue(a_RFID, a_Process);
	a_db.insertEvent(newEvent);
	this_thread::sleep_for(chrono::milliseconds((int)ucNewchar));
}

int Reader::toCloneRfid(RFID a_rfid, RFID& a_cloneRfid, Database& a_db) {	
	srand((int)getRandomChar(iRandomCharIndex++));
	if (rand() % CLONEAGRESSIVE == 0) {
		if (a_cloneRfid.getRFID() == 0)
			a_cloneRfid = a_rfid;
		read(a_cloneRfid, PROCESS::cloning, a_db);
		return 1;
	}
	return 0;
}