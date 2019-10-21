// Simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "RFID.h"
#include "Detector.h"
#include "Reader.h"
#include "Database.h"
#include "Tester.h"

int main()
{
	Tester tester;
	tester.runTest();	
}

