#ifndef _TENSORS_CONTROLLER_H_
#define _TENSORS_CONTROLLER_H_
#include <iostream>
#include <vector>

#include "nmdl.h"

using namespace std;

class TensorsController {

public:
    // Выходные тензоры 
    vector<vector<float>>outputs;
    // Входные тензоры
    vector<vector<float>>inputs;
    // Конструктор
    TensorsController();
    // Деструктор
    ~TensorsController();
    // Создание входного фрейма + конвертация изображения
    void createInputTensor(uint32_t width, uint32_t height,
        uint32_t image_converter_board_type,
        uint32_t image_converter_color_format);
    // Создание буферов для выходных тензоров
    void createOutputTensors(NMDL_ModelInfo info);

};

#endif