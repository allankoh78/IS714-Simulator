#include "DetectorPro.h"

DetectorPro::DetectorPro() {
	iSeed = 0;
	iTruthNumberofClone = 0;
	for (int i = 0; i < NUMBEROFRANDOMSYMBOL; i++)
		bsRandomSymbol[i].reset();
}

void DetectorPro::setSeed(int a_iSeedValue) {
	iSeed = a_iSeedValue;
}

int DetectorPro::getSeed() {
	return iSeed;
}

int DetectorPro::getTruthNumberofClone() {
	return iTruthNumberofClone;
}

void DetectorPro::setRandomSymbol(int a_iIndex, bitset<SYMBOLSIZE> a_bsRandom) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		bsRandomSymbol[a_iIndex] = a_bsRandom;
}

void DetectorPro::setRandomSymbol(int a_iIndex, int a_iRandom) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		bsRandomSymbol[a_iIndex] = bitset<SYMBOLSIZE>(a_iRandom);
}

bitset<SYMBOLSIZE> DetectorPro::getRandomSymbol(int a_iIndex) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		return bsRandomSymbol[a_iIndex];
	return NULL;
}

/// <summary>
/// initiateRandomSymbol.
/// Generate random values (mod 2^s) in the pool so that the values can be assigned to RFID tail. The random seed is set in setSeed(). 
/// </summary>
void DetectorPro::initiateRandomSymbol() {
	srand(getSeed());
	int iTemp = 0;
	int iPreviousTemp = 0;
	int iMod = (int)pow(2, SYMBOLSIZE);
	for (int iIndex = 0; iIndex < NUMBEROFRANDOMSYMBOL; iIndex++) {
		do {
			iTemp = rand() % iMod;
		} while (iPreviousTemp == iTemp);
		iPreviousTemp = iTemp;
		setRandomSymbol(iIndex, iTemp);
	}
	setSeed(rand());
}

/// <summary>
/// isValidRFID.
/// Return true if 
///	1. the tail pointer is in +1 sequence from a_PreviousRFIDPro to a_CurrentRFID and,
/// 2. the number of tail values different is equal to the a_CurrentRFIDPro.TailPointer.
/// </summary>
bool DetectorPro::isValidRFIDPro(RFIDPro a_PreviousRFIDPro, RFIDPro a_CurrentRFIDPro) {
	int iTestingTailPointer = a_PreviousRFIDPro.getTailPointer();
	// Check if tail pointer is in sequence. 
	if (iTestingTailPointer + 1 > TAILSIZEPRO)
		iTestingTailPointer = 1;
	else
		iTestingTailPointer++;
	if (iTestingTailPointer != a_CurrentRFIDPro.getTailPointer())
		return false;

	// Get the number of different tail values.
	int iCounter = 0;
	for (int i = 1; i <= NUMBEROFRANDOMSYMBOL; i++) {
		if (a_PreviousRFIDPro.getTail(i) != a_CurrentRFIDPro.getTail(i))
			iCounter++;
	}
	if (iCounter != a_CurrentRFIDPro.getTailPointer()) {
		return false;
	}
	return true;
}

/// <summary>
/// initiateRFIDArray.
/// Generate RFID index, tail pointer and tail value for all the RFID in a_pRFID. 
/// </summary>
void DetectorPro::initiateRFIDProArray(int a_iNumOfRFIDPros, RFIDPro a_pRFIDPro[]) {
	int iRandomCharIndex = 0;
	bitset<SYMBOLSIZE> bsTail[TAILSIZEPRO];

	for (int iIndex = 0; iIndex < a_iNumOfRFIDPros; iIndex++) {
		a_pRFIDPro[iIndex].setRFIDProID(iIndex + 1);
		a_pRFIDPro[iIndex].setTailPointer(0); // Set the first element of the tail.
		for (int iTailIndex = 0; iTailIndex < TAILSIZEPRO; iTailIndex++) {
			bsTail[iTailIndex] = getRandomSymbol(iRandomCharIndex + iTailIndex); // No need to check as detector ensures neighbouring random chars are unique.
		}
		a_pRFIDPro[iIndex].setTail(bsTail);
		a_pRFIDPro[iIndex].initiateTailPointer(); // To perform after setTail to reset the tail pointer to 1.
		iRandomCharIndex += TAILSIZEPRO;
		if (iRandomCharIndex + TAILSIZEPRO >= NUMBEROFRANDOMSYMBOL) {
			iRandomCharIndex = 0;
		}
		a_pRFIDPro[iIndex].setReaderSequence(RANDOMSEED + iIndex);
	}

}

/// <summary>
/// isValidRFIDTailEvents.
/// Return true if all the event in a_vRFIDObservations have valid tail values (Only tail value, pointed by the previous event tail pointer, is different).
/// </summary>
bool DetectorPro::isValidRFIDProTailEvents(vector<EventPro>& a_vRFIDProObservations, vector<RFIDPro>& a_vCloneRfidProFound, Logfile& a_logfile){
	EventPro currentEventPro;
	EventPro previousEventPro;
	bool bFoundClone = false;
	ostringstream strbufMessage;

	a_vCloneRfidProFound.clear();
	for (vector<EventPro>::iterator it = a_vRFIDProObservations.begin(); it != a_vRFIDProObservations.end(); ++it) {
		if (previousEventPro.getRFIDPro().getRFIDProID() == 0) {
			previousEventPro = *it;
			continue;
		}
		// Compare the current event tail with the previous event tail.
		currentEventPro = *it;
		// Skip the checking if bCloneAttacked is true;
		if (currentEventPro.getCloneAttacked() == true) {
			continue;
		}
		if (isValidRFIDPro(previousEventPro.getRFIDPro(), currentEventPro.getRFIDPro()) != true) {
			a_vCloneRfidProFound.push_back(currentEventPro.getRFIDPro());
			bFoundClone = true;
		}

		// This is to compute the truth of the system - Number of clone in total.
		if (previousEventPro.getProcess() == PROCESS::cloning) {
			iTruthNumberofClone++;
			if (bFoundClone == false) {
				strbufMessage << "!Detected consecutive clone tag!\n";
				strbufMessage << "Previous RFID, Tail Value, Tail Pointer, Timestamp, Operation, Current RFID, Tail Value, Tail Pointer\n";
				strbufMessage << previousEventPro.print() << "," << currentEventPro.print() << "\n!Detection ends!\n";
				a_logfile.write(strbufMessage.str());
				strbufMessage.clear();
				strbufMessage.str("");
			}
		}

		previousEventPro = *it; // Since we cannot differentiate who is the clone, so have to let the current event replace the previous event.
	}
	return a_vCloneRfidProFound.size() == 0;
}