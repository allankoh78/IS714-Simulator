#include "Detector.h"

Detector::Detector() {
	iSeed = 0;
	memset(pucRandomChar, 0x0, NUMBEROFRANDOMCHAR);
}

void Detector::setSeed(int a_iSeedValue) {
	iSeed = a_iSeedValue;
}

int Detector::getSeed() {
	return iSeed;
}

void Detector::setRandomChar(int a_iIndex, unsigned char a_ucRandom) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMCHAR)
		pucRandomChar[a_iIndex] = a_ucRandom;
}

unsigned char Detector::getRandomChar(int a_iIndex) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMCHAR)
		return pucRandomChar[a_iIndex];
	return NULL;
}

/// <summary>
/// initiateRandomChar.
/// Generate random values (mod 256) in the pool so that the values can be assigned to RFID tail. The random seed is set in setSeed(). 
/// </summary>
void Detector::initiateRandomChar() {
	srand(getSeed());
	unsigned char ucTemp = 0x0;
	unsigned char ucPreviousTemp = 0x0;
	for (int iIndex = 0; iIndex < NUMBEROFRANDOMCHAR; iIndex++) {
		do {
			ucTemp = rand() % 256 + 1;
		} while (ucPreviousTemp == ucTemp);
		ucPreviousTemp = ucTemp;
		setRandomChar(iIndex, static_cast<unsigned char> (ucTemp));
	}
}

/// <summary>
/// isValidRFID.
/// Return true if 
///	1. the tail pointer is in +1 sequence from a_PreviousRFID to a_CurrentRFID and,
/// 2. all tail values are the same, except for the tail value pointed by a_PreviousRFID.TailPointer.
/// </summary>
bool Detector::isValidRFID(RFID a_PreviousRFID, RFID a_CurrentRFID) {
	int iTestingTailPointer = a_PreviousRFID.getTailPointer();
	// Check if tail pointer is in sequence. 
	if (iTestingTailPointer + 1 > TAILSIZE)
		iTestingTailPointer = 1;
	else
		iTestingTailPointer++;
	if (iTestingTailPointer != a_CurrentRFID.getTailPointer())
		return false;
	
	// Reset the tail pointer to the previous RFID's one.
	iTestingTailPointer = a_PreviousRFID.getTailPointer();
	if (a_PreviousRFID.getTail(iTestingTailPointer) == a_CurrentRFID.getTail(iTestingTailPointer))
		return false;
	for (int iIndex = 1; iIndex <= TAILSIZE; iIndex++) {
		if (iIndex == iTestingTailPointer)
			continue;
		if (a_PreviousRFID.getTail(iIndex) != a_CurrentRFID.getTail(iIndex))
			return false;
	}
	return true;
}

/// <summary>
/// initiateRFIDArray.
/// Generate RFID index, tail pointer and tail value for all the RFID in a_pRFID. 
/// </summary>
void Detector::initiateRFIDArray(int a_iNumOfRFIDs, RFID a_pRFID[]) {
	int iRandomCharIndex = 0;

	for (int iIndex = 0; iIndex < a_iNumOfRFIDs; iIndex++) {
		a_pRFID[iIndex].setRFID(iIndex + 1);		
		a_pRFID[iIndex].initiateTailPointer();
		for (int iTailIndex = 0; iTailIndex < TAILSIZE; iTailIndex++) {
			a_pRFID[iIndex].setTail(getRandomChar(iRandomCharIndex+iTailIndex));
		}
		a_pRFID[iIndex].initiateTailPointer(); // To perform after setTail to reset the tail pointer to 1.
		iRandomCharIndex += TAILSIZE;
		if (iRandomCharIndex + TAILSIZE >= NUMBEROFRANDOMCHAR) {
			iRandomCharIndex = 0;
		}
	}

}

/// <summary>
/// isValidRFIDTailEvents.
/// Return true if all the event in a_vRFIDObservations have valid tail values (Only tail value, pointed by the previous event tail pointer, is different).
/// </summary>
bool Detector::isValidRFIDTailEvents(std::vector<Event> & a_vRFIDObservations, std::vector<RFID> & a_vCloneRfidFound) {
	Event currentEvent;
	Event previousEvent;

	a_vCloneRfidFound.clear();
	for (std::vector<Event>::iterator it = a_vRFIDObservations.begin(); it != a_vRFIDObservations.end(); ++it) {
		if (previousEvent.getRFID().getRFID() == 0) {
			previousEvent = *it;
			continue;
		}
		// Compare the current event tail with the previous event tail.
		currentEvent = *it;
		if (isValidRFID(previousEvent.getRFID(), currentEvent.getRFID()) != true)
			a_vCloneRfidFound.push_back(currentEvent.getRFID());
		previousEvent = *it; //Since we cannot differentiate who is the clone, so have to let the current event replace the previous event.
	}
	return a_vCloneRfidFound.size() == 0;
}