#ifndef __READERPRO_H_INCLUDED__   
#define __READERPRO_H_INCLUDED__   

#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <bitset>
#include "RFIDPro.h"
#include "DatabasePro.h"
#include "EventPro.h"
#include "Constant.h"
#include <math.h> 

class ReaderPro
{
private:
	int iReaderProID;
	int iSeed;
	int iRandomSymbolIndex;
	bitset<SYMBOLSIZE>  pbsRandomSymbol[NUMBEROFRANDOMSYMBOL];
	mutex mRandomSymbol;
public:
	ReaderPro();
	ReaderPro(const ReaderPro& a_ReaderPro);
	void setReaderProID(int a_iReaderProID);
	int getReaderProID();
	void setRandomSymbol(int a_iIndex, bitset<SYMBOLSIZE> a_bsRandom);
	void setRandomSymbol(int a_iIndex, int a_iRandom);
	bitset<SYMBOLSIZE> getRandomSymbol(int a_iIndex);
	void setSeed(int a_iSeedValue);
	int getSeed();
	void initiateRandomSymbol();
	ReaderPro& operator = (const ReaderPro& a_ReaderPro);
	void read(RFIDPro& a_RFIDPro, PROCESS a_iProcess, DatabasePro& a_dbPro, bool a_bCloneAttacked);
	int toCloneRfidPro(RFIDPro a_rfidPro, RFIDPro& a_cloneRfidPro, DatabasePro& a_dbPro); // Used by malicious reader.
};

#endif