#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "dirent.h" //Для Windows

#include "nmdl_image_converter.h"

#include "imageController.h"
#include "readFile.h"
#include "logs.h"

using namespace std;
using namespace cv;

ImageController::ImageController() {}

ImageController::~ImageController() {}

void ImageController::setFileName() {
    cout << "Введите название файла изображения: ";
    string buffer;
    bool flag = true;
    while (flag) {
        cin >> buffer;
        buffer = "images\\" + buffer;
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
    writeStatus("файл с исходным изображением задан");
    name_ = buffer;
    cout << "Обрабатываемое изображение: " << name_ << endl;
}

void ImageController::setFileName(string name) {
    ifstream inFile(name);
    cout << name << endl;
    if (inFile.is_open() == false) {
        writeError("невозможно открыть файл с исходным изображением");
    }
    else {
        name_ = name;
        writeStatus("файл с исходным изображением задан");
    }
}


void ImageController::countFrames() {
    int file_count = 0;
    string nm;
    string addr = "images\\result\\";
    DIR* dirp;
    try {
        dirp = opendir("images\\result\\");
        struct dirent* entry;
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_REG) {
                file_count++;
                nm = addr + entry->d_name;
                frameNames_.push_back(nm);
            }
        }
        numberFrames_ = file_count;
        cout << "Количеств фреймов: " << numberFrames_ << endl;
        closedir(dirp);
        writeStatus("имена фреймов получены, определно количество фреймов");
    }
    catch (...) {
        writeError("невозможно получить имена фреймов и определить их количество");
    }
}

vector<string> ImageController::getFrameNames() {
    return frameNames_;
}

string ImageController::getFileName() {
    return name_;
}

uint32_t ImageController::getNumberFrames() {
    return numberFrames_;
}

void ImageController::normalizationImage(string filename, vector<float> &input, uint32_t width, uint32_t height,
    uint32_t image_converter_color_format, float divider[], float adder[],
    uint32_t image_converter_board_type) {
    auto img_frame = ReadFile<char>(filename);
    if (NMDL_IMAGE_CONVERTER_Convert(img_frame.data(), input.data(),
        static_cast<uint32_t>(img_frame.size()), width, height,
        image_converter_color_format, divider, adder, image_converter_board_type)) {
        writeError("конвертация изображения не произведена");
    }
}

void ImageController::cropImage(int width, int height) {
    try {
        Mat img = imread(name_);
        int img_width = img.size().width;
        int img_height = img.size().height;
        image_width = img.size().width;
        image_height = img.size().height;
        Mat crop;
        string buff;
        int num_y = 0;
        int num_x = 0;
        if (img_width % width != 0 && img_height % height != 0) {
            int new_width = img_width + (width - img_width % width);
            int new_height = img_height + (height - img_height % height);
            resize(img, img, Size(new_width, new_height), INTER_LINEAR);
            imwrite(name_, img);
        }
        else if (img_width % width != 0 && img_height % height == 0) {
            int new_width = img_width + (width - img_width % width);
            int new_height = img_height;
            resize(img, img, Size(new_width, new_height), INTER_LINEAR);
            imwrite(name_, img);
        }
        else if (img_width % width == 0 && img_height % height != 0) {
            int new_width = img_width;
            int new_height = img_height + (height - img_height % height);
            resize(img, img, Size(new_width, new_height), INTER_LINEAR);
            imwrite(name_, img);
        }
        int countImg = (img_width / width) * (img_height / height);
        cout << countImg << endl;
        int num = 1;
        while (countImg > 0) {
            num *= 10;
            countImg /= 10;
        }
        for (int y = 0; y < img_height; y += height) {
            for (int x = 0; x < img_width; x += width) {
                crop = img(Range(y, y + height), Range(x, x + width));
                buff = "images\\result\\IMG" + to_string(num) + "(" + to_string(num_x) + "_" + to_string(num_y) + ").bmp";
                imwrite(buff, crop);
                num_x++;
                num++;
            }
            num_y++;
            num_x = 0;
        }
        writeStatus("произведена нарезка изображения на фреймы");
    }
    catch (Exception e) {
        writeError("невозможно произвести нарезку изображения на фреймы "+e.msg);
    }
}

