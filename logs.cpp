#include <string>
#include <fstream>
#include <ctime>
#include "logs.h"

using namespace std;

string currentDataTime() {
	__time64_t t;
	struct tm now;
	_time64(&t);
	errno_t e = _localtime64_s(&now, &t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%Y %X ", &now);
	return buffer;
}

string currentData() {
	__time64_t t;
	struct tm now;
	_time64(&t);
	errno_t e = _localtime64_s(&now, &t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%Y ", &now);
	return buffer;
}

void writeError(string message) {
	string mess = currentDataTime() + "Error: " + message + "\n";
	cout << mess;
	string addr = "log/logFile " + currentData() + ".txt";
	ofstream logFile(addr, ios::app);
	logFile << mess;
	logFile.close();
}

void writeStatus(string message) {
	string mess = currentDataTime() + "Status: " + message + "\n";
	cout << mess;
	string addr = "log/logFile " + currentData() + ".txt";
	ofstream logFile(addr, ios::app);
	logFile << mess;
	logFile.close();
}