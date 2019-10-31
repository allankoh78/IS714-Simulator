#include "Logfile.h"

Logfile::Logfile() {
}

int Logfile::setFilePath(string const& a_strFilePath) {
	myfile.open(a_strFilePath.c_str());
	if (myfile.is_open()) {
		return 0;
	}
	return -1;
}

void Logfile::write(string const& a_strMessage) {
	mLock.lock();
	myfile.write(a_strMessage.c_str(), a_strMessage.size());
	mLock.unlock();
}

void Logfile::close() {
	if (myfile.is_open()){
		myfile.close();
	}	
}