#ifndef __TESTER_H_INCLUDED__   
#define __TESTER_H_INCLUDED__   
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include "RFID.h"
#include "Event.h"
#include "Constant.h"
#include "Detector.h"
#include "Reader.h"
#include "Database.h"
#include "Logfile.h"

class Tester
{
private:
	
public:
	void runTest(Logfile& a_logfile);
	bool ComparisionTest();
	bool InitializationTest(Detector& a_Detector, RFID a_pRFID[]);
	bool InitializationReaderTest(Reader a_pReader[]);
	bool ReaderRFIDTest(Reader a_pReader[], RFID a_pRFID[], Detector& a_Detector, Logfile& a_logfile);
};

#endif