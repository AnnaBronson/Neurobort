#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "nmdl_compiler.h"

#include "model.h"
#include "readFile.h"
#include "logs.h"


using namespace std;

Model::Model() {
    type_model_ = 2;
    for (int i = 0; i < 3; i++) {
        divider_[i] = 1.0;
        adder_[i] = 0.0;
    }
}

Model::~Model() {
}

void Model::setModelMode() {
    cout << "----------НАСТРОЙКА МОДЕЛИ----------" << endl;
    cout << "Выберите тип модели: " << endl;
    cout << "0 - onnx " << endl;
    cout << "1 - darknet" << endl;
    do {
        cin >> type_model_;
        if ((type_model_ != 0) && (type_model_ != 1)) {
            cout << "Неверно выбран тип модели! Повторите попытку!" << endl;
            type_model_ = 2;
        }
    } while (type_model_ == 2);

    cout << "Введите имя файла модели: ";
    string buffer;
    bool flag = true;
    while (flag) {
        cin >> buffer;
        buffer = "models\\" + buffer;
        ifstream inFile(buffer);
        if (inFile.is_open() == false) {
            cout << "Файл не найден!" << endl;
            cout << "Попробуйте еще раз: " << endl;
        }
        else {
            flag = false;
            inFile.close();
        }
    }
    name_ = buffer;

    if (type_model_ == 1) {
        cout << "Введите имя файла с весами: ";
        flag = true;
        while (flag) {
            cin >> buffer;
            buffer = "weights\\" + buffer;
            ifstream inFile(buffer);
            if (inFile.is_open() == false) {
                cout << "Файл не найден!" << endl;
                cout << "Попробуйте еще раз: " << endl;
            }
            else {
                flag = false;
                inFile.close();
            }
        }
        weight_name_ = buffer;
    }

    cout << "Нужно ли компилировать модель?" << endl;
    cout << "0 - Да" << endl;
    cout << "1 - Нет" << endl;
    int choise = -1;
    do {
        cin >> choise;
        if (choise == 0) {
            is_compile_ = true;
        }
        else if (choise == 1) {
            is_compile_ = false;
        }
        else {
            cout << "Выбор сделан неверно! Повторите попытку!" << endl;
            choise = -1;
        }
    } while (choise == -1);

    cout << "Введите делители: ";
    cin >> divider_[0] >> divider_[1] >> divider_[2];
    cout << "Введите слагаемые: ";
    cin >> adder_[0] >> adder_[1] >> adder_[2];
    writeStatus("настройка параметров модели завершена");
}

void Model::showModeModel() {
    cout << "----------НАСТРОЙКИ МОДЕЛИ----------" << endl;
    cout << "Тип модели: ";
    if (type_model_ == 0)
        cout << "onnx" << endl;
    else if (type_model_ == 1)
        cout << "darknet" << endl;
    cout << "Имя файла модели: " << name_ << endl;
    if (type_model_ == 1)
        cout << "Имя файла с весами: " << weight_name_ << endl;
    cout << "Делители: ";
    for (int i = 0; i < 3; i++)
        cout << divider_[i] << " ";
    cout << "\nСлагаемые: ";
    for (int i = 0; i < 3; i++)
        cout << adder_[i] << " ";
    cout << "\n------------------------------------" << endl;
}

string Model::getName() {
    return name_;
}

string Model::getWeightName() {
    return weight_name_;
}

size_t Model::getTypeModel() {
    return type_model_;
}

float* Model::getDivider() {
    return divider_;
}

float* Model::getAdder() {
    return adder_;
}

vector<float> Model::compileOnnxModel(bool is_multi_unit,
    uint32_t compiler_board_type) {
    float* nm_model = nullptr;
    uint32_t nm_model_floats = 0u;
    try {
        auto model_read = ReadFile<char>(name_);
        NMDL_COMPILER_CompileONNX(is_multi_unit, compiler_board_type, model_read.data(),
            static_cast<uint32_t>(model_read.size()), &nm_model, &nm_model_floats);
        vector<float> model(nm_model, nm_model + nm_model_floats);
        NMDL_COMPILER_FreeModel(compiler_board_type, nm_model);
        writeStatus("компиляция модели типа ONNX завершена");
        return model;
    }
    catch (...) {
        writeError("компиляция модели типа ONNX произведена с ошибкой");
    }
    
}

vector<float> Model::compileDarknetModel(bool is_multi_unit,
    uint32_t compiler_board_type) {
    float* nm_model = nullptr;
    uint32_t nm_model_floats = 0u;
    try {
        auto config = ReadFile<char>(name_);
        auto weights = ReadFile<char>(weight_name_);
        NMDL_COMPILER_CompileDarkNet(is_multi_unit, compiler_board_type,
            config.data(), config.size(), weights.data(), weights.size(),
            &nm_model, &nm_model_floats);
        std::vector<float> model(nm_model, nm_model + nm_model_floats);
        NMDL_COMPILER_FreeModel(compiler_board_type, nm_model);
        writeStatus("компиляция модеи типа Darknet завершена");
        return model;
    }
    catch (...) {
        writeError("компиляция модели типа Darknet произведена с ошибкой");
    }
}

vector<float> Model::readModel() {
    try {
        auto model = ReadFile<float>(name_);
        writeStatus("чтение модели произведено успешно");
        return model;
    }
    catch (...) {
        writeError("чтение параметров модели произведено с ошибкой");
    }
}

bool Model::getIsCompile() {
    return is_compile_;
}

void Model::setTypeModel(size_t type ) {
    if ((type == 0)|| (type == 1)) {
        type_model_ = type;
        writeStatus("тип модели задан");
    }
    else
        writeError("тип модели задан неверно");
}

void Model::setNameModel(string name) {
    name = "models\\" + name;
    ifstream inFile(name);
    if (inFile.is_open() == false) {
        writeError("модель не найдена");
    }
    else {
        inFile.close();
        name_ = name;
        writeStatus("модель задана");
    }
}

void Model::setWeightModel(string name) {
    name = "weights\\" + name;
    ifstream inFile(name);
    if (inFile.is_open() == false) {
        writeError("веса не найдены");
    }
    else {
        inFile.close();
        weight_name_ = name;
        writeStatus("веса заданы");
    }
}

void Model::setNeedCompilation(size_t choise) {
    if (choise == 0) {
        is_compile_ = true;
        writeStatus("необходимость компиляции установлена");
    }
    else if (choise == 1) {
        is_compile_ = false;
        writeStatus("необходимость компиляции установлена");
    }
    else
        writeError("неверно задана необходимость компиляции");
}

void Model::setAdderDivider(float add[], float div[]) {
    for (int i = 0; i < 3; i++) {
        adder_[i] = add[i];
        divider_[i] = div[i];
    }
    writeStatus("слагаемые и делители установлены");
}