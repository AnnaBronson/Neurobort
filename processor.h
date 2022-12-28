// Предназначен для выполнения вычислений (сегментации или классификации) 
// и определения fps

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include <iostream>

#include "nmdl.h"

using namespace std;

class Processor {
    
public:
    // Значение FPS (частота обработки: количество кадров в секунду)
    double fps;
    // Конструктор
    Processor();
    // Дестрктор
    ~Processor();
    // Функция вычисления и получения результата
    void waitForOutput(NMDL_HANDLE nmdl, uint32_t unit_num, float* outputs[]);
};

#endif