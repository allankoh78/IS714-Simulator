#include "RFIDPro.h"

RFIDPro::RFIDPro() {
	reset();
}

void RFIDPro::reset() {
	iRfid = 0;
	iTailPointer = 1;
	for (int i = 0; i < TAILSIZEPRO ; i++)
		bsTail[i].reset();
	memset(aReaderSequence, 0x0, sizeof(int) * NUMOFREADER);
}

RFIDPro::RFIDPro(int a_iRfid, bitset<SYMBOLSIZE>* a_pbsTail) {
	iRfid = a_iRfid;
	iTailPointer = 1;
	for (int i = 0; i < TAILSIZEPRO; i++)
		bsTail[i] = a_pbsTail[i];
	memset(aReaderSequence, 0x0, sizeof(int) * NUMOFREADER);
}

RFIDPro::RFIDPro(const RFIDPro& a_RFIDPro) {
	iRfid = a_RFIDPro.iRfid;
	iTailPointer = a_RFIDPro.iTailPointer;
	for (int i = 0; i < TAILSIZEPRO ; i++)
		bsTail[i] = a_RFIDPro.bsTail[i];
	memcpy(aReaderSequence, a_RFIDPro.aReaderSequence, sizeof(int) * NUMOFREADER);
}

RFIDPro& RFIDPro::operator = (const RFIDPro& a_RFIDPro) {
	iRfid = a_RFIDPro.iRfid;
	iTailPointer = a_RFIDPro.iTailPointer;
	for (int i = 0; i < TAILSIZEPRO ; i++)
		bsTail[i] = a_RFIDPro.bsTail[i];
	memcpy(aReaderSequence, a_RFIDPro.aReaderSequence, sizeof(int) * NUMOFREADER);
	return *this;
}

int RFIDPro::getTailPointer() {
	return iTailPointer;
}

void RFIDPro::setTailPointer(int a_iTailPointer) {
	iTailPointer = a_iTailPointer;
}

void RFIDPro::initiateTailPointer() {
	iTailPointer = 1;
}

void RFIDPro::incTailPointer() {
	if (iTailPointer + 1 >= TAILSIZEPRO)
		iTailPointer = 1;
	else
		iTailPointer++;	
}

bitset<SYMBOLSIZE> RFIDPro::getTail(int a_iIndex) {
	bitset<SYMBOLSIZE> bsResult; // All set to 0s.
	mTail.lock();
	if (a_iIndex > 0 && a_iIndex <= TAILSIZEPRO)
		bsResult = bsTail[a_iIndex-1];
	mTail.unlock();
	return bsResult;
}

bitset<SYMBOLSIZE>* RFIDPro::getTail() {
	return bsTail;
}

void RFIDPro::setTail(bitset<SYMBOLSIZE>* a_pbsTail) {
	mTail.lock();
	if (a_pbsTail != NULL) {
		incTailPointer();
		for (int i = 0; i < TAILSIZEPRO; i++)
			bsTail[i] = a_pbsTail[i];
	}
	else
		for (int i = 0; i < TAILSIZEPRO ; i++)
			bsTail[i].reset();
	mTail.unlock();
}

void RFIDPro::setRFIDProID(int a_iRfidProID) {
	iRfid = a_iRfidProID;
}

int RFIDPro::getRFIDProID() {
	return iRfid;
}

string RFIDPro::print() {
	ostringstream strbufMessage;
	strbufMessage << getRFIDProID() << ",";
	for ( int iIndex = 1 ; iIndex <= TAILSIZEPRO; iIndex++ )
		strbufMessage << getTail(iIndex) << ":";
	strbufMessage << "," << getTailPointer();
	return strbufMessage.str();
}

void RFIDPro::setReaderSequence(int iSeed) {
	for (int iIndex = 0; iIndex < NUMOFREADER; iIndex++)
		aReaderSequence[iIndex] = iIndex + 1;

	srand(iSeed);
	for (int i = NUMOFREADER - 1; i > 0; i--)
	{
		// Pick a random index from 0 to i  
		int j = rand() % (i + 1);

		// Swap array elements with another element at random index.
		int iTemp = aReaderSequence[i];
		aReaderSequence[i] = aReaderSequence[j];
		aReaderSequence[j] = iTemp;	
	}//end for
}

int RFIDPro::getReaderSequence(int iIndex) {
	return aReaderSequence[iIndex];
}