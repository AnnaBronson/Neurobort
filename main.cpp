#include <iostream>

#include "script.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	// �������� ���������� ��������
	Script script;
	// ������ �������� ������������
	script.segmentationOneImage();
	return 0;
}

