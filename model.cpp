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
    cout << "----------��������� ������----------" << endl;
    cout << "�������� ��� ������: " << endl;
    cout << "0 - onnx " << endl;
    cout << "1 - darknet" << endl;
    do {
        cin >> type_model_;
        if ((type_model_ != 0) && (type_model_ != 1)) {
            cout << "������� ������ ��� ������! ��������� �������!" << endl;
            type_model_ = 2;
        }
    } while (type_model_ == 2);

    cout << "������� ��� ����� ������: ";
    string buffer;
    bool flag = true;
    while (flag) {
        cin >> buffer;
        buffer = "models\\" + buffer;
        ifstream inFile(buffer);
        if (inFile.is_open() == false) {
            cout << "���� �� ������!" << endl;
            cout << "���������� ��� ���: " << endl;
        }
        else {
            flag = false;
            inFile.close();
        }
    }
    name_ = buffer;

    if (type_model_ == 1) {
        cout << "������� ��� ����� � ������: ";
        flag = true;
        while (flag) {
            cin >> buffer;
            buffer = "weights\\" + buffer;
            ifstream inFile(buffer);
            if (inFile.is_open() == false) {
                cout << "���� �� ������!" << endl;
                cout << "���������� ��� ���: " << endl;
            }
            else {
                flag = false;
                inFile.close();
            }
        }
        weight_name_ = buffer;
    }

    cout << "����� �� ������������� ������?" << endl;
    cout << "0 - ��" << endl;
    cout << "1 - ���" << endl;
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
            cout << "����� ������ �������! ��������� �������!" << endl;
            choise = -1;
        }
    } while (choise == -1);

    cout << "������� ��������: ";
    cin >> divider_[0] >> divider_[1] >> divider_[2];
    cout << "������� ���������: ";
    cin >> adder_[0] >> adder_[1] >> adder_[2];
    writeStatus("��������� ���������� ������ ���������");
}

void Model::showModeModel() {
    cout << "----------��������� ������----------" << endl;
    cout << "��� ������: ";
    if (type_model_ == 0)
        cout << "onnx" << endl;
    else if (type_model_ == 1)
        cout << "darknet" << endl;
    cout << "��� ����� ������: " << name_ << endl;
    if (type_model_ == 1)
        cout << "��� ����� � ������: " << weight_name_ << endl;
    cout << "��������: ";
    for (int i = 0; i < 3; i++)
        cout << divider_[i] << " ";
    cout << "\n���������: ";
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
        writeStatus("���������� ������ ���� ONNX ���������");
        return model;
    }
    catch (...) {
        writeError("���������� ������ ���� ONNX ����������� � �������");
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
        writeStatus("���������� ����� ���� Darknet ���������");
        return model;
    }
    catch (...) {
        writeError("���������� ������ ���� Darknet ����������� � �������");
    }
}

vector<float> Model::readModel() {
    try {
        auto model = ReadFile<float>(name_);
        writeStatus("������ ������ ����������� �������");
        return model;
    }
    catch (...) {
        writeError("������ ���������� ������ ����������� � �������");
    }
}

bool Model::getIsCompile() {
    return is_compile_;
}

void Model::setTypeModel(size_t type ) {
    if ((type == 0)|| (type == 1)) {
        type_model_ = type;
        writeStatus("��� ������ �����");
    }
    else
        writeError("��� ������ ����� �������");
}

void Model::setNameModel(string name) {
    name = "models\\" + name;
    ifstream inFile(name);
    if (inFile.is_open() == false) {
        writeError("������ �� �������");
    }
    else {
        inFile.close();
        name_ = name;
        writeStatus("������ ������");
    }
}

void Model::setWeightModel(string name) {
    name = "weights\\" + name;
    ifstream inFile(name);
    if (inFile.is_open() == false) {
        writeError("���� �� �������");
    }
    else {
        inFile.close();
        weight_name_ = name;
        writeStatus("���� ������");
    }
}

void Model::setNeedCompilation(size_t choise) {
    if (choise == 0) {
        is_compile_ = true;
        writeStatus("������������� ���������� �����������");
    }
    else if (choise == 1) {
        is_compile_ = false;
        writeStatus("������������� ���������� �����������");
    }
    else
        writeError("������� ������ ������������� ����������");
}

void Model::setAdderDivider(float add[], float div[]) {
    for (int i = 0; i < 3; i++) {
        adder_[i] = add[i];
        divider_[i] = div[i];
    }
    writeStatus("��������� � �������� �����������");
}