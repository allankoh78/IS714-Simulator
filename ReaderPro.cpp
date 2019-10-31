#include "ReaderPro.h"

ReaderPro::ReaderPro() {
	iReaderProID = 0;
	iSeed = 0;
	iRandomSymbolIndex = 0;
	for (int i = 0; i < NUMBEROFRANDOMSYMBOL; i++)
		pbsRandomSymbol[i].reset();
}

ReaderPro::ReaderPro(const ReaderPro& a_ReaderPro) {
	iReaderProID = a_ReaderPro.iReaderProID;
	iSeed = a_ReaderPro.iSeed;
	iRandomSymbolIndex = a_ReaderPro.iRandomSymbolIndex;
	for (int i = 0; i < NUMBEROFRANDOMSYMBOL; i++)
		pbsRandomSymbol[i] = a_ReaderPro.pbsRandomSymbol[i];
}

void ReaderPro::setReaderProID(int a_iReaderProID) {
	iReaderProID = a_iReaderProID;
}

int ReaderPro::getReaderProID() {
	return iReaderProID;
}

void ReaderPro::setSeed(int a_iSeedValue) {
	iSeed = a_iSeedValue;
}

int ReaderPro::getSeed() {
	return iSeed;
}

void ReaderPro::setRandomSymbol(int a_iIndex, bitset<SYMBOLSIZE> a_bsRandom) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		pbsRandomSymbol[a_iIndex] = a_bsRandom;
}

void ReaderPro::setRandomSymbol(int a_iIndex, int a_iRandom) {
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		pbsRandomSymbol[a_iIndex] = bitset<SYMBOLSIZE>(a_iRandom);
}

bitset<SYMBOLSIZE> ReaderPro::getRandomSymbol(int a_iIndex) {
	mRandomSymbol.lock();
	bitset<SYMBOLSIZE>  bsResult;
	if (a_iIndex > -1 && a_iIndex < NUMBEROFRANDOMSYMBOL)
		bsResult = pbsRandomSymbol[a_iIndex];
	mRandomSymbol.unlock();
	return bsResult;
}

ReaderPro& ReaderPro::operator = (const ReaderPro& a_ReaderPro) {
	iReaderProID = a_ReaderPro.iReaderProID;
	iSeed = a_ReaderPro.iSeed;
	iRandomSymbolIndex = a_ReaderPro.iRandomSymbolIndex;
	for (int i = 0; i < NUMBEROFRANDOMSYMBOL; i++)
		pbsRandomSymbol[i] = a_ReaderPro.pbsRandomSymbol[i];
	return *this;
}

/// <summary>
/// initiateRandomSymbol.
/// Generate random values (mod 2^s) in the pool so that the values can be assigned to RFID tail. The random seed is set in setSeed(). 
/// </summary>
void ReaderPro::initiateRandomSymbol() {
	mRandomSymbol.lock();
	srand(getSeed());
	int iTemp = 0;
	int iPreviousTemp = 0;
	int iMod = (int) pow(2, SYMBOLSIZE);
	for (int iIndex = 0; iIndex < NUMBEROFRANDOMSYMBOL; iIndex++) {
		do {
			iTemp = rand() % iMod;
		} while (iPreviousTemp == iTemp);
		iPreviousTemp = iTemp;
		setRandomSymbol(iIndex, iTemp);
	}
	setSeed(rand());
	mRandomSymbol.unlock();
}

void ReaderPro::read(RFIDPro& a_RFIDPro, PROCESS a_iProcess, DatabasePro& a_dbPro, bool a_bCloneAttacked) {
	if (iRandomSymbolIndex >= NUMBEROFRANDOMSYMBOL)
		iRandomSymbolIndex = 0;
	bool bSuccess = false;
	bitset<SYMBOLSIZE> bsNewSymbol;
	bitset<SYMBOLSIZE> pbsCurrentTail[TAILSIZEPRO];
	
	// Get the current tail value.
	for ( int i = 0 ; i < TAILSIZEPRO ; i++)
		pbsCurrentTail[i] = a_RFIDPro.getTail(i+1);	

	// Get the current tail pointer to compute the number of tail symbols to change.
	int iNewTailPointer = a_RFIDPro.getTailPointer();
	if (iNewTailPointer + 1 >= TAILSIZEPRO)
		iNewTailPointer = 1;
	else
		iNewTailPointer++;
	
	// Random select the tail symbols indexes to change.
	srand(getSeed());
	bool bSelectedTailIndex[TAILSIZEPRO] = { false };
	int iCounter = 0;
	int iSelectedTailIndex = 0;
	do {
		iSelectedTailIndex = rand() % TAILSIZEPRO;
		if (bSelectedTailIndex[iSelectedTailIndex] == true)
			continue;
		bSelectedTailIndex[iSelectedTailIndex] = true;
		iCounter++;
	} while (iCounter < iNewTailPointer);

	// Replace the selected tail indexes.
	for (int i = 0; i < TAILSIZEPRO; i++) {
		if (bSelectedTailIndex[i] == true) {
			do {
				bsNewSymbol = getRandomSymbol(iRandomSymbolIndex++);
				if (bsNewSymbol != pbsCurrentTail[i]) {
					pbsCurrentTail[i] = bsNewSymbol;
					break;
				}
			} while (bsNewSymbol == pbsCurrentTail[i]);
		}	
	}
	
	// Update seed.
	setSeed(rand());

	// Update the tail.
	a_RFIDPro.setTail(pbsCurrentTail);

	// Add the event into database.
	EventPro newEventPro;
	newEventPro.setValue(a_RFIDPro, a_iProcess, a_bCloneAttacked);
	a_dbPro.insertEventPro(newEventPro);
	this_thread::sleep_for(chrono::milliseconds(100));
}

int ReaderPro::toCloneRfidPro(RFIDPro a_rfidPro, RFIDPro& a_cloneRfidPro, DatabasePro& a_dbPro) {
	int iResult = 1;
	srand(getSeed());
	if (rand() % CLONEAGRESSIVE == 0) {
		if (a_cloneRfidPro.getRFIDProID() == 0)
			a_cloneRfidPro = a_rfidPro;
		read(a_cloneRfidPro, PROCESS::cloning, a_dbPro, false);
	}
	else {
		iResult = 0;
	}
	// Update seed.
	setSeed(rand());
	return iResult;
}