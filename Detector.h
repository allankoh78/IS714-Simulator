#ifndef __DETECTOR_H_INCLUDED__   
#define __DETECTOR_H_INCLUDED__   

#include "RFID.h"
#include "Constant.h"
#include "Event.h"
#include <vector>

class Detector
{
private:
	int iSeed;
	unsigned char pucRandomChar[NUMBEROFRANDOMCHAR];
public:
	Detector();
	void setRandomChar(int a_iIndex, unsigned char a_ucRandom);
	unsigned char getRandomChar(int a_iIndex);
	void setSeed(int a_iSeedValue);
	int getSeed();	
	void initiateRandomChar();
	bool isValidRFID(RFID a_PreviousRFID, RFID a_CurrentRFID);
	void initiateRFIDArray(int a_iNumOfRFIDs, RFID a_pRFID[]);
	bool isValidRFIDTailEvents(std::vector<Event>& a_vRFIDObservations, std::vector<RFID>& a_vCloneRfidFound);
};

#endif