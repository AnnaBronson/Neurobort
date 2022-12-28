#include <iostream>
#include <array>
#include <string>

#include "nmdl.h"
#include "nmdl_compiler.h"
#include "nmdl_image_converter.h"

#include "nmdlController.h"
#include "logs.h"

using namespace std;

NmdlController::NmdlController() {
}

NmdlController::~NmdlController() {
    NMDL_Release(nmdl);
    NMDL_Destroy(nmdl);
}

void NmdlController::setModeNmdl() {
    cout << "----------НАСТРОЙКА NMDL СЕССИИ----------" << endl;
    cout << "Выберите тип вычислительного модуля: " << endl;
    cout << "0 - Симулятор" << endl;
    cout << "1 - MC12101" << endl;
    cout << "2 - MC12705" << endl;
    cout << "3 - NMSTICK" << endl;
    cout << "4 - NMCARD" << endl;
    cout << "5 - NMMEZZO" << endl;
    uint32_t buffer = -1;
    while (buffer == -1) {
        cin >> buffer;
        if (buffer == 0)
            board_type_ = NMDL_BOARD_TYPE_SIMULATOR;
        else if (buffer == 1)
            board_type_ = NMDL_BOARD_TYPE_MC12101;
        else if (buffer == 2)
            board_type_ = NMDL_BOARD_TYPE_MC12705;
        else if (buffer == 3)
            board_type_ = NMDL_BOARD_TYPE_NMSTICK;
        else if (buffer == 4)
            board_type_ = NMDL_BOARD_TYPE_NMCARD;
        else if (buffer == 5)
            board_type_ = NMDL_BOARD_TYPE_NMMEZZO;
        else {
            buffer = -1;
            cout << "Неверно выбран тип устройства! Повторите попытку!" << endl;
        }
    }
    cout << "Выберите цветовую схему изображения: " << endl;
    cout << "0 - RGB" << endl;
    cout << "1 - RBG" << endl;
    cout << "2 - GRB" << endl;
    cout << "3 - GBR" << endl;
    cout << "4 - BRG" << endl;
    cout << "5 - BGR" << endl;
    cout << "6 - INTENSITY" << endl;
    do {
        cin >> buffer;
        if (buffer == 0)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RGB;
        else if (buffer == 1)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RBG;
        else if (buffer == 2)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GRB;
        else if (buffer == 3)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GBR;
        else if (buffer == 4)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BRG;
        else if (buffer == 5)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BGR;
        else if (buffer == 6)
            image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_INTENSITY;
        else {
            buffer = -1;
            cout << "Неверно выбрана цветовая схема! Повторите попытку!" << endl;
        }
    } while (buffer == -1);
    setConverter();
    cout << "Выберите режим работы: " << endl;
    cout << "0 - multi unit" << endl;
    cout << "1 - batch" << endl;
    do {
        cin >> buffer;
        if (buffer == 0) {
            is_multi_unit_ = true;
            is_batch_ = false;
            units_ = 0;
        }
        else if (buffer == 1) {
            is_multi_unit_ = false;
            is_batch_ = true;
            cout << "Введите количество юнитов: " << endl;
            do {
                cin >> units_;
                if ((units_ > 4) || (units_ < 1)) {
                    cout << "Недопустимое значение! Повторите попытку! " << endl;
                    units_ = -1;
                }
            } while (units_ == -1);
        }
        else {
            buffer = -1;
            cout << "Неверно выбран режим работы! Повторите попытку!" << endl;
        }
    } while (buffer == -1);
    writeStatus("настройка параметров nmdl завершена");
}


void NmdlController::setConverter() {
    if (board_type_ == 0 || board_type_ == 2 || board_type_ == 4) {
        image_converter_board_type_ = NMDL_IMAGE_CONVERTER_BOARD_TYPE_MC12705;
        compiler_board_type_ = NMDL_COMPILER_BOARD_TYPE_MC12705;
    }
    else if (board_type_ == 1 || board_type_ == 3 || board_type_ == 5) {
        image_converter_board_type_ = NMDL_IMAGE_CONVERTER_BOARD_TYPE_MC12101;
        compiler_board_type_ = NMDL_COMPILER_BOARD_TYPE_MC12101;
    }
}


uint32_t NmdlController::getBoardType() {
    return board_type_;
}

uint32_t NmdlController::getCompilerBoardType() {
    return compiler_board_type_;
}

uint32_t NmdlController::getImageConverterBoardType() {
    return image_converter_board_type_;
}

NMDL_IMAGE_CONVERTER_COLOR_FORMAT NmdlController::getImageColorFormat() {
    return image_converter_color_format_;
}

bool NmdlController::getIsBatch() {
    return is_batch_;
}

size_t NmdlController::getUnits() {
    return units_;
}

bool NmdlController::getIsMultiUnit() {
    return is_multi_unit_;
}

void NmdlController::showNMDLVersion() {
    uint32_t major = 0;
    uint32_t minor = 0;
    uint32_t patch = 0;
    try {
        NMDL_GetLibVersion(&major, &minor, &patch);
        cout << "Версия библиотеки NMDL: " << major << "." << minor
            << "." << patch << endl;
        writeStatus("получена версия библиотеки NMDL");
    }
    catch (...) {
        writeError("не удалось получить версию библиотеки NMDL");
    }
}

uint32_t NmdlController::checkBoard() {
    try {
        uint32_t boards = -1;
        NMDL_GetBoardCount(board_type_, &boards);
        writeStatus("получено количество активных устройств");
        return boards;
    }
    catch (...) {
        writeError("не удалось получить количество устройств");
    }
}

void NmdlController::showModeNmdl() {
    cout << "----------КОНФИГУРАЦИЯ ЭКЗЕМПЛЯРА NMDL----------" << endl;
    cout << "Вычислительное устройство: ";
    switch (board_type_)
    {
    case NMDL_BOARD_TYPE_SIMULATOR:
        cout << "Симулятор" << endl;
        break;
    case NMDL_BOARD_TYPE_MC12101:
        cout << "MC12101" << endl;
        break;
    case NMDL_BOARD_TYPE_MC12705:
        cout << "MC12705" << endl;
        break;
    case NMDL_BOARD_TYPE_NMSTICK:
        cout << "NMSTICK" << endl;
        break;
    case NMDL_BOARD_TYPE_NMCARD:
        cout << "NMCARD" << endl;
        break;
    case NMDL_BOARD_TYPE_NMMEZZO:
        cout << "NMMEZZO" << endl;
        break;
    default:
        cout << "Тип вычислительного устройства не выбран" << endl;
        break;
    }
    cout << "Количество устройств: " << checkBoard() << endl;
    cout << "Тип устройства для конвертации изображения: ";
    switch (image_converter_board_type_)
    {
    case NMDL_IMAGE_CONVERTER_BOARD_TYPE_MC12101:
        cout << "MC12101" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_BOARD_TYPE_MC12705:
        cout << "MC12705" << endl;
        break;
    default:
        cout << "Тип конвертации изображения не выбран" << endl;
        break;
    }
    cout << "Тип устройства для компиляции модели: ";
    switch (compiler_board_type_)
    {
    case NMDL_COMPILER_BOARD_TYPE_MC12101:
        cout << "MC12101" << endl;
        break;
    case NMDL_COMPILER_BOARD_TYPE_MC12705:
        cout << "MC12705" << endl;
        break;
    default:
        cout << "Тип компиляции модели не выбран" << endl;
        break;
    }
    cout << "Цветовая схема: ";
    switch (image_converter_color_format_)
    {
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RGB:
        cout << "RGB" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RBG:
        cout << "RBG" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GRB:
        cout << "GRB" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GBR:
        cout << "GBR" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BRG:
        cout << "BRG" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BGR:
        cout << "BGR" << endl;
        break;
    case NMDL_IMAGE_CONVERTER_COLOR_FORMAT_INTENSITY:
        cout << "INTENSITY" << endl;
        break;
    default:
        cout << "Цветовая схема не установлена" << endl;
        break;
    }
    cout << "Режим работы: ";
    if (is_batch_) {
        cout << "бэтч" << endl;
        cout << "Количество юнитов: " << units_ << endl;
    }
    if (is_multi_unit_)
        cout << "мульти-юнит" << endl;
    showNMDLVersion();
    cout << "------------------------------------------------" << endl;
}

void NmdlController::createNmdlObject() {
    try{
        NMDL_Create(&nmdl);
        writeStatus("экземпляр NMDL создан");
    }
    catch (...) {
        writeError("ошибка при создании экземпляра Nmdl");
    }
}

void NmdlController::modelInitialize(vector<float>& model, uint32_t board_number,
    uint32_t proc_number) {
    array<const float*, NMDL_MAX_UNITS> models = { model.data() };
    array<uint32_t, NMDL_MAX_UNITS> model_floats = { static_cast<uint32_t>(model.size()) };
    try {
        NMDL_RESULT r = NMDL_Initialize(nmdl, board_type_, board_number, proc_number, models.data(),
            model_floats.data());
        cout << "Результат инициализации: " << r << endl;
        string mess = "произведена инициализация на борту номер " + to_string(board_number)
            + ", на процессоре номер " + to_string(proc_number);
        writeStatus(mess);
    }
    catch (...) {
        string message = "не удалось инициализировать модель на устройстве номер " + to_string(board_number) 
            + ", на процессоре номер " + to_string(proc_number);
        writeError(message);
    }
}

void NmdlController::loadInformation(uint32_t unit_num) {
    try {
        NMDL_GetModelInfo(nmdl, unit_num, &model_info_);
        string mess = "получена информация о модели на юните номер " + to_string(unit_num);
        writeStatus(mess);
        cout << "Количество входных тензоров: " << model_info_.input_tensor_num << endl;
        for (size_t i = 0; i < model_info_.input_tensor_num; ++i) {
            cout << "Входные тензоры " << i << ": " <<
                model_info_.input_tensors[i].width << ", " <<
                model_info_.input_tensors[i].height << ", " <<
                model_info_.input_tensors[i].depth <<
                endl;
        }
        cout << "Количество выходных тензоров: " << model_info_.output_tensor_num << endl;
        for (size_t i = 0; i < model_info_.output_tensor_num; ++i) {
            cout << "Выходные тензоры " << i << ": " <<
                model_info_.output_tensors[i].width << ", " <<
                model_info_.output_tensors[i].height << ", " <<
                model_info_.output_tensors[i].depth <<
                endl;
        }
    }
    catch (...) {
        string message = "не удалось получить информацию о юните номер " + to_string(unit_num);
        writeError(message);
    }
}

NMDL_ModelInfo NmdlController::getModelInformaion() {
    return model_info_;
}

void NmdlController::setBoardType(uint32_t board_type) {
    if (board_type == 0) {
        board_type_ = NMDL_BOARD_TYPE_SIMULATOR;
        writeStatus("задан тип вычислительного устройства");
    }
    else if (board_type == 1) {
        board_type_ = NMDL_BOARD_TYPE_MC12101;
        writeStatus("задан тип вычислительного устройства");
    }
    else if (board_type == 2) {
        board_type_ = NMDL_BOARD_TYPE_MC12705;
        writeStatus("задан тип вычислительного устройства");
    }
    else if (board_type == 3) {
        board_type_ = NMDL_BOARD_TYPE_NMSTICK;
        writeStatus("задан тип вычислительного устройства");
    }
    else if (board_type == 4) {
        board_type_ = NMDL_BOARD_TYPE_NMCARD;
        writeStatus("задан тип вычислительного устройства");
    }
    else if (board_type == 5) {
        board_type_ = NMDL_BOARD_TYPE_NMMEZZO;
        writeStatus("задан тип вычислительного устройства");
    }
    else {
        writeError("неверно задан тип вычислительного устройства");
    }
}

void NmdlController::setImageConverterColor(uint32_t image_converter_color_format) {
    if (image_converter_color_format == 0) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RGB;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 1) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_RBG;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 2) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GRB;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 3) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_GBR;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 4) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BRG;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 5) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_BGR;
        writeStatus("задана цветовая схема изображения");
    }
    else if (image_converter_color_format == 6) {
        image_converter_color_format_ = NMDL_IMAGE_CONVERTER_COLOR_FORMAT_INTENSITY;
        writeStatus("задана цветовая схема изображения");
    }
    else {
        writeError("неверно задана цветовая схема изображения");
    }
}

void NmdlController::setWorkMode(uint32_t is_multi_unit, uint32_t is_batch, size_t units) {
    if (is_multi_unit == 1 && is_batch == 0) {
        is_multi_unit_ = true;
        is_batch = false;
        units_ = units;
        writeStatus("задан режим работы NMDL");
    }
    else if (is_multi_unit == 0 && is_batch == 1) {
        is_multi_unit_ = false;
        is_batch = true;
        units_ = units;
        writeStatus("задан режим работы NMDL");
    }
    else {
        writeError("некорректный режим работы NMDL");
    }
}