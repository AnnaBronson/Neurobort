#ifndef _TENSORS_CONTROLLER_H_
#define _TENSORS_CONTROLLER_H_
#include <iostream>
#include <vector>

#include "nmdl.h"

using namespace std;

class TensorsController {

public:
    // �������� ������� 
    vector<vector<float>>outputs;
    // ������� �������
    vector<vector<float>>inputs;
    // �����������
    TensorsController();
    // ����������
    ~TensorsController();
    // �������� �������� ������ + ����������� �����������
    void createInputTensor(uint32_t width, uint32_t height,
        uint32_t image_converter_board_type,
        uint32_t image_converter_color_format);
    // �������� ������� ��� �������� ��������
    void createOutputTensors(NMDL_ModelInfo info);

};

#endif