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
	unsigned char cResult = NULL;
	mRandomChar.lock();
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMCHAR)
		cResult = pucRandomChar[a_iIndex];	
	mRandomChar.unlock();
	return cResult;
}

void Reader::initiateRandomChar() {	
	mRandomChar.lock();
	srand(getSeed());
	unsigned char ucTemp = 0x0;
	unsigned char ucPreviousTemp = 0x0;
	for (int iIndex = 0; iIndex < NUMBEROFRANDOMCHAR; iIndex++) {
		do {
			ucTemp = rand() % 256 + 1;
		} while (ucPreviousTemp == ucTemp);
		ucPreviousTemp = ucTemp;
		setRandomChar(iIndex, static_cast<unsigned char> (ucTemp));
	}	
	mRandomChar.unlock();
}

void Reader::read(RFID& a_RFID, PROCESS a_Process, Event& a_Event) {
	if (iRandomCharIndex >= NUMBEROFRANDOMCHAR)
		iRandomCharIndex = 0;
	a_RFID.setTail(getRandomChar(iRandomCharIndex++));
	a_Event.setValue(a_RFID, a_Process);
}

int Reader::toCloneRfid(RFID a_rfid, RFID& a_cloneRfid, Event& a_cloneEvent) {
	if (rand() % CLONEAGRESSIVE == 0) {
		if (a_cloneRfid.getRFID() == 0)
			a_cloneRfid = a_rfid;
		read(a_cloneRfid, PROCESS::cloning, a_cloneEvent);
		return 1;
	}
	return 0;
}