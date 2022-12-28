#ifndef _LOGS_H_
#define _LOGS_H_
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

// ������� ��� ������������� �����������
// ������� ��� ��������� ������� ���� � �������
string currentDataTime();
// ������� ��� ��������� ������� ����
string currentData();
// ������� ��� ������ � ���� � ������ �� ����� ��������� �� ������
void writeError(string message);
// ������� ��� ������ � ���� � ������ �� ����� ��������� � ������� ���������� ���������
void writeStatus(string message);

#endif