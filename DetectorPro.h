#ifndef __DETECTORPRO_H_INCLUDED__   
#define __DETECTORPRO_H_INCLUDED__   
#include <vector>
#include <sstream> 

#include "RFIDPro.h"
#include "Constant.h"
#include "EventPro.h"
#include "Logfile.h"
class DetectorPro
{
private:
	int iSeed;
	bitset<SYMBOLSIZE> bsRandomSymbol[NUMBEROFRANDOMSYMBOL];
	int iTruthNumberofClone;
public:
	DetectorPro();
	void setRandomSymbol(int a_iIndex, bitset<SYMBOLSIZE> a_bsRandom);
	void setRandomSymbol(int a_iIndex, int a_iRandom);
	bitset<SYMBOLSIZE> getRandomSymbol(int a_iIndex);
	void setSeed(int a_iSeedValue);
	int getSeed();
	int getTruthNumberofClone();
	void initiateRandomSymbol();
	bool isValidRFIDPro(RFIDPro a_PreviousRFIDPro, RFIDPro a_CurrentRFIDPro);
	void initiateRFIDProArray(int a_iNumOfRFIDPros, RFIDPro a_pRFIDPro[]);
	bool isValidRFIDProTailEvents(vector<EventPro>& a_vRFIDProObservations, vector<RFIDPro>& a_vCloneRfidProFound, Logfile& a_logfile);
};

#endif