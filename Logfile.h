#ifndef __LOGFILE_H_INCLUDED__   
#define __LOGFILE_H_INCLUDED__
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include "Constant.h"

class Logfile
{
private:
	ofstream myfile;
	mutex mLock;

public:
	Logfile();
	int setFilePath(string const& a_strFilePath);
	void write(string const& a_strMessage);
	void close();	
};
#endif