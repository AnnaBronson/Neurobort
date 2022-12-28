// �����, ����������� ������ � ������������� � ��������
#ifndef _IMAGE_CONTROLLER_H_
#define _IMAGE_CONTROLLER_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ImageController {
private:
    // ��� ��������� �����������
    string name_;
    // ����� �������
    vector<string> frameNames_;
    // ���������� �������
    uint32_t numberFrames_;

public:
    // ������ �����������
    int image_width;
    // ������ �����������
    int image_height;

    // �����������
    ImageController();
    // ����������
    ~ImageController();
    // ��������� ����� �����������
    void setFileName();
    void setFileName(string name);
    // ��������� ����� �����������
    string getFileName();
    // ������� ���������� ������� � ��������� �� ����
    void countFrames();
    // ��������� ���� �������
    vector<string> getFrameNames();
    // ��������� ���������� �������
    uint32_t getNumberFrames();
    // ����������� �����������
    void normalizationImage(string filename,vector<float> &input, uint32_t width, 
        uint32_t height, uint32_t image_converter_color_format, 
        float divider[], float adder[], uint32_t image_converter_board_type);
    // ������� ����������� �� ������
    void cropImage(int width, int height);
};

#endif

