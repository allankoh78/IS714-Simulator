#ifndef __RFID_H_INCLUDED__   
#define __RFID_H_INCLUDED__   

#include <string>
#include <iostream>
#include <mutex>
#include <sstream>
#include "Constant.h"

class RFID
{
private:
	int iRfid;
	int iTailPointer;
	unsigned char pucTail[TAILSIZE];	
	mutex mTail;
	int aReaderSequence[NUMOFREADER];
	
public:
	RFID();
	RFID(const RFID& a_RFID);
	RFID(int a_iRfid, unsigned char* a_pucTail);
	void incTailPointer();
	void setTailPointer(int a_iTailPointer);
	void setRFID(int a_iRfid);
	int getRFID();
	int getTailPointer();
	void initiateTailPointer();
	unsigned char getTail(int a_iIndex);
	unsigned char* getTail();
	bool setTail(unsigned char a_ucTail);
	void setTail(unsigned char* a_pucTail);
	string print();
	void reset();
	RFID& operator = (const RFID& a_RFID);
	void setReaderSequence(int iSeed); // https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
	int getReaderSequence(int iIndex);
};

#endif 