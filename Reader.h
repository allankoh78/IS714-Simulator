#ifndef __READER_H_INCLUDED__   
#define __READER_H_INCLUDED__   

#include <string>
#include <thread>
#include <mutex>
#include "RFID.h"
#include "Database.h"
#include "Event.h"
#include "Constant.h"

class Reader
{
private:
	int iReaderID;
	int iSeed;
	int iRandomCharIndex;
	unsigned char pucRandomChar[NUMBEROFRANDOMCHAR];
	std::mutex mRandomChar;
public:
	Reader();
	Reader(const Reader& a_Reader);
	void setReaderID(int a_iReaderID);
	int getReaderID();
	void setRandomChar(int a_iIndex, unsigned char a_ucRandom);
	unsigned char getRandomChar(int a_iIndex);
	void setSeed(int a_iSeedValue);
	int getSeed();
	void initiateRandomChar();
	Reader& operator = (const Reader& a_Reader);
	void read(RFID& a_RFID, PROCESS a_iProcess, Database &a_db);
	int toCloneRfid(RFID a_rfid, RFID& a_cloneRfid, Database& a_db); // Used by malicious reader.
};

#endif