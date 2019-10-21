#ifndef __RFID_H_INCLUDED__   
#define __RFID_H_INCLUDED__   

#include <string>
#include <iostream>
#include <mutex>
#include "Constant.h"

class RFID
{
private:
	int iRfid;
	int iTailPointer;
	unsigned char pucTail[TAILSIZE];	
	std::mutex mTail;
	
public:
	RFID();
	RFID(const RFID& a_RFID);
	RFID(int a_iRfid, unsigned char* a_pucTail);
	void incTailPointer();
	void setRFID(int a_iRfid);
	int getRFID();
	int getTailPointer();
	void initiateTailPointer();
	unsigned char getTail(int a_iIndex);
	unsigned char* getTail();
	void setTail(unsigned char a_ucTail);
	void setTail(unsigned char* a_pucTail);
	void print();
	void reset();
	RFID& operator = (const RFID& a_RFID);
};

#endif 