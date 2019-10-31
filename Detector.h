#ifndef __DETECTOR_H_INCLUDED__   
#define __DETECTOR_H_INCLUDED__   
#include <vector>
#include <sstream> 

#include "RFID.h"
#include "Constant.h"
#include "Event.h"
#include "Logfile.h"
class Detector
{
private:
	int iSeed;
	unsigned char pucRandomChar[NUMBEROFRANDOMCHAR];
	int iTruthNumberofClone;
public:
	Detector();
	void setRandomChar(int a_iIndex, unsigned char a_ucRandom);
	unsigned char getRandomChar(int a_iIndex);
	void setSeed(int a_iSeedValue);
	int getSeed();	
	int getTruthNumberofClone();
	void initiateRandomChar();
	bool isValidRFID(RFID a_PreviousRFID, RFID a_CurrentRFID);
	void initiateRFIDArray(int a_iNumOfRFIDs, RFID a_pRFID[]);
	bool isValidRFIDTailEvents(vector<Event>& a_vRFIDObservations, vector<RFID>& a_vCloneRfidFound, Logfile& a_logfile);
};

#endif