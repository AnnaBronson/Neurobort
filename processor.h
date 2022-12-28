// ������������ ��� ���������� ���������� (����������� ��� �������������) 
// � ����������� fps

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include <iostream>

#include "nmdl.h"

using namespace std;

class Processor {
    
public:
    // �������� FPS (������� ���������: ���������� ������ � �������)
    double fps;
    // �����������
    Processor();
    // ���������
    ~Processor();
    // ������� ���������� � ��������� ����������
    void waitForOutput(NMDL_HANDLE nmdl, uint32_t unit_num, float* outputs[]);
};

#endif