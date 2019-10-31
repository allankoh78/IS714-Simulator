#include "RFID.h"

RFID::RFID() {
	iRfid = 0;
	iTailPointer = 1;
	memset(pucTail, 0x0, sizeof(unsigned char) * TAILSIZE);
	memset(aReaderSequence, 0x0, sizeof(int) * NUMOFREADER);
}

void RFID::reset() {
	iRfid = 0;
	iTailPointer = 1;
	memset(pucTail, 0x0, sizeof(unsigned char) * TAILSIZE);
	memset(aReaderSequence, 0x0, sizeof(int) * NUMOFREADER);
}

RFID::RFID(int a_iTailPointer, unsigned char* a_pucTail) {
	iRfid = a_iTailPointer;
	iTailPointer = 1;
	setTail(a_pucTail);
}

RFID::RFID(const RFID& a_RFID) {
	iRfid = a_RFID.iRfid;
	iTailPointer = a_RFID.iTailPointer;
	memcpy(pucTail, a_RFID.pucTail, sizeof(unsigned char) * TAILSIZE);
	memcpy(aReaderSequence, a_RFID.aReaderSequence, sizeof(int) * NUMOFREADER);
}

RFID& RFID::operator = (const RFID& a_RFID) {
	iRfid = a_RFID.iRfid;
	iTailPointer = a_RFID.iTailPointer;
	memcpy(pucTail, a_RFID.pucTail, sizeof(unsigned char) * TAILSIZE);
	memcpy(aReaderSequence, a_RFID.aReaderSequence, sizeof(int) * NUMOFREADER);
	return *this;
}

int RFID::getTailPointer() {
	return iTailPointer;
}

void RFID::setTailPointer(int a_iTailPointer) {
	iTailPointer = a_iTailPointer;
}

void RFID::initiateTailPointer() {
	iTailPointer = 1;
}

void RFID::incTailPointer() {
	if (iTailPointer + 1 > TAILSIZE)
		iTailPointer = 1;
	else
		iTailPointer++;	
}

unsigned char RFID::getTail(int a_iIndex) {
	unsigned char ucResult = NULL;
	mTail.lock();
	if (a_iIndex > 0 && a_iIndex <= TAILSIZE)
		ucResult= pucTail[a_iIndex -1];
	mTail.unlock();
	return ucResult;
}

unsigned char* RFID::getTail() {
	return pucTail;
}

bool RFID::setTail(unsigned char a_ucTail) {
	bool bSuccess = true;
	mTail.lock();
	int iOriginalTailPointer = iTailPointer;
	incTailPointer();
	if (pucTail[iTailPointer - 1] != a_ucTail)
		pucTail[iTailPointer - 1] = a_ucTail;
	else {
		iTailPointer = iOriginalTailPointer;
		bSuccess = false;
	}
	mTail.unlock();
	return bSuccess;
}

void RFID::setTail(unsigned char* a_pucTail) {
	mTail.lock();
	if (a_pucTail != NULL)
		memcpy(pucTail, a_pucTail, TAILSIZE);
	else
		memset(pucTail, 0x0, TAILSIZE);
	mTail.unlock();
}

void RFID::setRFID(int a_iRfid) {
	iRfid = a_iRfid;
}

int RFID::getRFID() {
	return iRfid;
}

string RFID::print() {
	ostringstream strbufMessage;
	strbufMessage << getRFID() << ",";
	for ( int iIndex = 1 ; iIndex <= TAILSIZE ; iIndex++ )
		strbufMessage << (int)getTail(iIndex) << "-";
	strbufMessage << "," << getTailPointer();
	return strbufMessage.str();
}

void RFID::setReaderSequence(int iSeed) {
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

int RFID::getReaderSequence(int iIndex) {
	return aReaderSequence[iIndex];
}