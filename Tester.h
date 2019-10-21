#ifndef __TESTER_H_INCLUDED__   
#define __TESTER_H_INCLUDED__   
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "RFID.h"
#include "Event.h"
#include "Constant.h"
#include "Detector.h"
#include "Reader.h"
#include "Database.h"

class Tester
{
private:
	
public:
	void runTest();
	bool ComparisionTest();
	bool InitializationTest(Detector a_Detector, RFID a_pRFID[]);
	bool InitializationTest(Reader a_pReader[]);
	bool ReaderRFIDTest(Reader a_pReader[], RFID a_pRFID[], Detector a_Detector);
};

#endif