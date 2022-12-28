// �����, ����������� ����������� nmdl
#ifndef _NMDLCONTROLLER_H_
#define _NMDLCONTROLLER_H_

#include <iostream>
#include <vector>

#include "nmdl.h"
#include "nmdl_image_converter.h"

using namespace std;

class NmdlController {
private:
    // ��� ��������������� ����������
    uint32_t board_type_;
    // ��� ������ ���� ���������� ����������� ���������� ������
    uint32_t compiler_board_type_;
    // ��� ������ ���� ���������� ����������� ����������� �����������
    uint32_t image_converter_board_type_;
    // �������� ����� �����������
    NMDL_IMAGE_CONVERTER_COLOR_FORMAT image_converter_color_format_;
    // ������������� ����-���������
    bool is_batch_;
    // ���������� ������
    size_t units_;
    // ������������� ������ ������-����
    bool is_multi_unit_;
    // ���������� � ����������� ������
    NMDL_ModelInfo model_info_;

public:
    // ��������� nmdl
    NMDL_HANDLE nmdl;
    // �����������
    NmdlController();
    // ����������
    ~NmdlController();
    // ��������� ������������ ���������� nmdl
    void setModeNmdl();
    // ��������� ���� ����������
    void setBoardType(uint32_t board_type);
    // ��������� ���� ����������� �����������
    void setImageConverterColor(uint32_t image_converter_color_format);
    // ��������� ������ ������ � ���������� ������
    void setWorkMode(uint32_t is_multi_unit, uint32_t is_batch, size_t units);
    // ��������� ���� ����������� � ����������
    void setConverter();
    // ��������� ���� ��������������� ����������
    uint32_t getBoardType();
    // ��������� ���� ���������� ��� ���������� ������
    uint32_t getCompilerBoardType();
    // ��������� ���� ���������� ��� ����������� �����������
    uint32_t getImageConverterBoardType();
    // ��������� �������� ����� �����������
    NMDL_IMAGE_CONVERTER_COLOR_FORMAT getImageColorFormat();
    // ��������� ������������� ����-���������
    bool getIsBatch();
    // ��������� ���������� ������
    size_t getUnits();
    // ��������� ������������� ������ ������-����
    bool getIsMultiUnit();
    // �������� ������� ���������� ��������������� ������
    uint32_t checkBoard();
    // ����� ������ nmdl � �������
    void showNMDLVersion();
    // ����� ��������� ���������� nmdl
    void showModeNmdl();
    // �������� nmdl
    void createNmdlObject();
    // ������������� ������
    void modelInitialize(vector<float>& model, uint32_t board_number,
        uint32_t proc_number);
    // ��������� � ����� ���������� � ������, ����������� � ���������
    void loadInformation(uint32_t unit_num);
    // ��������� ��� ������������� ���������� � ������, ����������� � ���������
    NMDL_ModelInfo getModelInformaion();

};

#endif