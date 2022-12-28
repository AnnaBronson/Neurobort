// �����, ����������� ������ � ���������� ������� ��� ������ � ���
#ifndef _MODEL_H_
#define _MODEL_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Model {
private:
    // ��� ������
    string name_;
    // ��� ����� � ������
    string weight_name_;
    // ��� ������
    size_t type_model_;
    // ��������
    float divider_[3];
    // ���������
    float adder_[3];
    // ������������� ����������
    bool is_compile_;



public:
    // �����������
    Model();
    // ����������
    ~Model();
    // ��������� ���������� ������
    void setModelMode();
    // ����� ��������� ������ � �������
    void showModeModel();
    // ��������� ����� ������
    string getName();
    // ��������� ����� ����� � ������
    string getWeightName();
    // ��������� ���� ������
    size_t getTypeModel();
    // ��������� ���������
    float* getDivider();
    // ��������� ���������
    float* getAdder();
    // ��������� ������������� ���������� ������
    bool getIsCompile();
    // ������ ������
    vector<float> readModel();
    // ���������� ������ ���� onnx
    vector<float> compileOnnxModel(bool is_multi_unit,
        uint32_t compiler_board_type);
    // ���������� ������ ���� darknet
    vector<float> compileDarknetModel(bool is_multi_unit,
        uint32_t compiler_board_type);
    // ��������� ���� ������
    void setTypeModel(size_t type);
    // ��������� ����� ������
    void setNameModel(string name);
    // ��������� ����� ����� � ������
    void setWeightModel(string name);
    // ��������� ������������� ������������� ������
    void setNeedCompilation(size_t choise);
    // ��������� ��������� � ���������
    void setAdderDivider(float add[], float div[]);
};

#endif