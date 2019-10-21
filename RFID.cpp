#include "RFID.h"

RFID::RFID() {
	iRfid = 0;
	iTailPointer = 1;
	memset(pucTail, 0x0, TAILSIZE);
}

void RFID::reset() {
	iRfid = 0;
	iTailPointer = 1;
	memset(pucTail, 0x0, TAILSIZE);
}

RFID::RFID(int a_iTailPointer, unsigned char* a_pucTail) {
	iRfid = a_iTailPointer;
	iTailPointer = 1;
	setTail(a_pucTail);
}

RFID::RFID(const RFID& a_RFID) {
	iRfid = a_RFID.iRfid;
	iTailPointer = a_RFID.iTailPointer;
	memcpy(pucTail, a_RFID.pucTail, TAILSIZE);
}

RFID& RFID::operator = (const RFID& a_RFID) {
	iRfid = a_RFID.iRfid;
	iTailPointer = a_RFID.iTailPointer;
	memcpy(pucTail, a_RFID.pucTail, TAILSIZE);
	return *this;
}

int RFID::getTailPointer() {
	return iTailPointer;
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

void RFID::setTail(unsigned char a_ucTail) {
	mTail.lock();
	pucTail[iTailPointer - 1] = a_ucTail;
	incTailPointer();	
	mTail.unlock();
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

void RFID::print() {
	//std::cout << getRFID() << "," << std::string((char *) getTail(), TAILSIZE) << "," << getTailPointer();
	std::cout << getRFID() << ",";
	for ( int iIndex = 1 ; iIndex <= TAILSIZE ; iIndex++ )
		std::cout << (int)getTail(iIndex) << "-";
	std::cout << "," << getTailPointer();
}
