#ifndef __RFIDPro_H_INCLUDED__   
#define __RFIDPro_H_INCLUDED__   

#include <string>
#include <iostream>
#include <mutex>
#include <sstream>
#include <bitset>
#include "Constant.h"

class RFIDPro
{
private:
	int iRfid;
	int iTailPointer;
	bitset<SYMBOLSIZE> bsTail[TAILSIZEPRO];
	mutex mTail;
	int aReaderSequence[NUMOFREADER];
	
public:
	RFIDPro();
	RFIDPro(const RFIDPro& a_RFIDPro);
	RFIDPro(int a_iRfid, bitset<SYMBOLSIZE>* a_pbsTail);
	void incTailPointer();
	void setTailPointer(int a_iTailPointer);
	void setRFIDProID(int a_iRfidProID);
	int getRFIDProID();
	int getTailPointer();
	void initiateTailPointer();
	bitset<SYMBOLSIZE> getTail(int a_iIndex);
	bitset<SYMBOLSIZE>* getTail();
	void setTail(bitset<SYMBOLSIZE>* a_pbsTail);
	string print();
	void reset();
	RFIDPro& operator = (const RFIDPro& a_RFIDPro);
	void setReaderSequence(int iSeed); // https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
	int getReaderSequence(int iIndex);
};

#endif 