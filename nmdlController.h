// Класс, управляющий экземпляром nmdl
#ifndef _NMDLCONTROLLER_H_
#define _NMDLCONTROLLER_H_

#include <iostream>
#include <vector>

#include "nmdl.h"
#include "nmdl_image_converter.h"

using namespace std;

class NmdlController {
private:
    // Тип вычислительного устройства
    uint32_t board_type_;
    // Для какого типа устройства производить компиляцию модели
    uint32_t compiler_board_type_;
    // Для какого типа устройства производить конвертацию изображения
    uint32_t image_converter_board_type_;
    // Цветовая схема изображения
    NMDL_IMAGE_CONVERTER_COLOR_FORMAT image_converter_color_format_;
    // Использование бэтч-обработки
    bool is_batch_;
    // Количество юнитов
    size_t units_;
    // Использование режима мульти-юнит
    bool is_multi_unit_;
    // Информация о загруженной модели
    NMDL_ModelInfo model_info_;

public:
    // Экземпляр nmdl
    NMDL_HANDLE nmdl;
    // Конструктор
    NmdlController();
    // Деструктор
    ~NmdlController();
    // Установка конфигурации экземпляра nmdl
    void setModeNmdl();
    // Установка типа устройства
    void setBoardType(uint32_t board_type);
    // Установка типа конвартации изображения
    void setImageConverterColor(uint32_t image_converter_color_format);
    // Установка режима работы и количества юнитов
    void setWorkMode(uint32_t is_multi_unit, uint32_t is_batch, size_t units);
    // Настройка типа конвертации и компиляции
    void setConverter();
    // Получение типа вычислительного устройства
    uint32_t getBoardType();
    // Получение типа устройства для компиляции модели
    uint32_t getCompilerBoardType();
    // Получение типа устройства для конвертации изображения
    uint32_t getImageConverterBoardType();
    // Получение цветовой схемы изображения
    NMDL_IMAGE_CONVERTER_COLOR_FORMAT getImageColorFormat();
    // Получение использования бэтч-обработки
    bool getIsBatch();
    // Получение количества юнитов
    size_t getUnits();
    // Получение использования режима мульти-юнит
    bool getIsMultiUnit();
    // Проверка наличия выбранного вычислительного модуля
    uint32_t checkBoard();
    // Вывод версии nmdl в консоль
    void showNMDLVersion();
    // Вывод настройки экземпляра nmdl
    void showModeNmdl();
    // Создание nmdl
    void createNmdlObject();
    // Инициализация модели
    void modelInitialize(vector<float>& model, uint32_t board_number,
        uint32_t proc_number);
    // Получение и вывод информации о модели, загруженной в экземпляр
    void loadInformation(uint32_t unit_num);
    // Получение для использования информации о модели, загруженной в экземпляр
    NMDL_ModelInfo getModelInformaion();

};

#endif