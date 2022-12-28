// Класс, описывающий работу с изображениями и фреймами
#ifndef _IMAGE_CONTROLLER_H_
#define _IMAGE_CONTROLLER_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ImageController {
private:
    // Имя исходного изображения
    string name_;
    // Имена фреймов
    vector<string> frameNames_;
    // Количество фреймов
    uint32_t numberFrames_;

public:
    // Ширина изображения
    int image_width;
    // Высота изображения
    int image_height;

    // Конструктор
    ImageController();
    // Деструктор
    ~ImageController();
    // Установка имени изображения
    void setFileName();
    void setFileName(string name);
    // Получение имени изображения
    string getFileName();
    // Подсчет количества фреймов и получение их имен
    void countFrames();
    // Получение имен фреймов
    vector<string> getFrameNames();
    // Получение количества фреймов
    uint32_t getNumberFrames();
    // Конвертация изображений
    void normalizationImage(string filename,vector<float> &input, uint32_t width, 
        uint32_t height, uint32_t image_converter_color_format, 
        float divider[], float adder[], uint32_t image_converter_board_type);
    // Нарезка изображения на фреймы
    void cropImage(int width, int height);
};

#endif

