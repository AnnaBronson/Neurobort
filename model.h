// Класс, описывающий модель и содержащих функции для работы с ней
#ifndef _MODEL_H_
#define _MODEL_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Model {
private:
    // Имя модели
    string name_;
    // Имя файла с весами
    string weight_name_;
    // Тип модели
    size_t type_model_;
    // Делители
    float divider_[3];
    // Слагаемые
    float adder_[3];
    // Необходимость компиляции
    bool is_compile_;



public:
    // Конструктор
    Model();
    // Деструктор
    ~Model();
    // Установка параметров модели
    void setModelMode();
    // Вывод настройки модели в консоль
    void showModeModel();
    // Получение имени модели
    string getName();
    // Получение имени файла с весами
    string getWeightName();
    // Получение типа модели
    size_t getTypeModel();
    // Получение делителей
    float* getDivider();
    // Получение слагаемых
    float* getAdder();
    // Получение необходимости компиляции модели
    bool getIsCompile();
    // Чтение модели
    vector<float> readModel();
    // Компиляция модели типа onnx
    vector<float> compileOnnxModel(bool is_multi_unit,
        uint32_t compiler_board_type);
    // Компиляция модели типа darknet
    vector<float> compileDarknetModel(bool is_multi_unit,
        uint32_t compiler_board_type);
    // Установка типа модели
    void setTypeModel(size_t type);
    // Установка имени модели
    void setNameModel(string name);
    // Установка имени файла с весами
    void setWeightModel(string name);
    // Установка необходимости компилировать модель
    void setNeedCompilation(size_t choise);
    // Установка делителей и слагаемых
    void setAdderDivider(float add[], float div[]);
};

#endif