#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <time.h>
#include <fstream>

#include "readFile.h"
#include "nmdlController.h"
#include "model.h"
#include "imageController.h"
#include "tensorsController.h"
#include "processor.h"
#include "script.h"
#include "utilites.h"
#include "logs.h"
#include "databaseController.h"


using namespace cv;

Script::Script() {

}

Script::~Script() {

}

void Script::createNmdlSession() {
	nmdlController_.setModeNmdl();
	nmdlController_.showModeNmdl();
	nmdlController_.createNmdlObject();
}

void Script::setModelMultiUnit() {
	if (model_.getIsCompile()) {
		if (model_.getTypeModel() == 0) {
			auto model = model_.compileOnnxModel(nmdlController_.getIsMultiUnit(), 
				nmdlController_.getCompilerBoardType());
			nmdlController_.modelInitialize(model, 0, 0);
		}
		else if (model_.getTypeModel() == 1) {
			auto model = model_.compileDarknetModel(nmdlController_.getIsMultiUnit(),
				nmdlController_.getCompilerBoardType());
			nmdlController_.modelInitialize(model, 0, 0);
		}
	}
	else {
		auto model = model_.readModel();
		nmdlController_.modelInitialize(model, 0, 0);
	}
}

void Script::setModelsBatch() {
	if (model_.getIsCompile()) {
		if (model_.getTypeModel() == 0) {
			for (int i = 0; i < nmdlController_.getUnits(); i++) {
				auto model = model_.compileOnnxModel(nmdlController_.getIsMultiUnit(),
					nmdlController_.getCompilerBoardType());
				nmdlController_.modelInitialize(model, 0, i);
			}
		}
		else if (model_.getTypeModel() == 1) {
			for (int i = 0; i < nmdlController_.getUnits(); i++) {
				auto model = model_.compileDarknetModel(nmdlController_.getIsMultiUnit(),
					nmdlController_.getCompilerBoardType());
				nmdlController_.modelInitialize(model, 0, i);
			}
		}
	}
	else {
		auto model = model_.readModel();
		for (int i = 0; i < nmdlController_.getUnits(); i++)
			nmdlController_.modelInitialize(model, 0, i);
	}
}

void Script::loadModels() {
	if (nmdlController_.getIsMultiUnit()) {
		setModelMultiUnit();
	}
	else {
		setModelsBatch();
	}
	for (int i = 0; i < nmdlController_.getUnits(); i++)
		nmdlController_.loadInformation(i);
}

void Script::prepareModel() {
	model_.setModelMode();
	model_.showModeModel();
	loadModels();
}

void Script::setImage() {
	model_info_ = nmdlController_.getModelInformaion();
	imageController_.setFileName();
	imageController_.cropImage(model_info_.input_tensors[0].width,
		model_info_.input_tensors[0].height);
	imageController_.countFrames();
}

void Script::setImage(string name) {
	model_info_ = nmdlController_.getModelInformaion();
	imageController_.setFileName(name);
	imageController_.cropImage(model_info_.input_tensors[0].width,
		model_info_.input_tensors[0].height);
	imageController_.countFrames();
}

void Script::prepareTensors() {
	for (int i = 0; i < imageController_.getNumberFrames(); i++) {
		tensorsController_.createInputTensor(model_info_.input_tensors[0].width,
			model_info_.input_tensors[0].height,
			nmdlController_.getImageConverterBoardType(),
			nmdlController_.getImageColorFormat());

		imageController_.normalizationImage(imageController_.getFrameNames()[i], 
			tensorsController_.inputs[i],
			model_info_.input_tensors[0].width, 
			model_info_.input_tensors[0].height,
			nmdlController_.getImageColorFormat(), model_.getDivider(),
			model_.getAdder(),
			nmdlController_.getImageConverterBoardType());
	}
	writeStatus("входные тензоры созданы");
	tensorsController_.createOutputTensors(model_info_);
}

void Script::startProcess() {
	try {
		if (nmdlController_.getIsMultiUnit()) {
			for (int i = 0; i < imageController_.getNumberFrames(); i++) {
				vector<const float*> inT = { tensorsController_.inputs[i].data() };
				vector<float*> outT = { tensorsController_.outputs[0].data() };
				NMDL_Process(nmdlController_.nmdl, 0, inT.data());
				processor_.waitForOutput(nmdlController_.nmdl, 0, outT.data());
				finalTensor_.push_back(tensorsController_.outputs[0]);
			}
		}
		else if (nmdlController_.getIsBatch()) {
			int countImg = 0;
			int units = 0;
			while (countImg < imageController_.getNumberFrames()) {
				vector<const float*> inT = { tensorsController_.inputs[countImg].data() };
				vector<float*> outT = { tensorsController_.outputs[units % nmdlController_.getUnits()].data() };
				NMDL_Process(nmdlController_.nmdl, units % nmdlController_.getUnits(), inT.data());
				processor_.waitForOutput(nmdlController_.nmdl, units % nmdlController_.getUnits(), outT.data());
				finalTensor_.push_back(tensorsController_.outputs[units % nmdlController_.getUnits()]);
				countImg++;
				units++;
			}
		}
		writeStatus("вычисления произведены корректно");
	}
	catch (...) {
		writeError("вычисления произведены некорректно");
	}
}

float Script::getResultSegmentation() {
	try {
		long int count_of_pixels = 0;
		long int count_of_cloud_pixels = 0;
		for (int i = 0; i < imageController_.getNumberFrames(); i++) {
			Mat img = imread(imageController_.getFrameNames()[i]);
			cout << imageController_.getFrameNames()[i] << endl;
			int img_width = img.size().width;
			int img_height = img.size().height;
			if (model_info_.output_tensors[0].depth > 1) {
				for (int y = 0, cur_data = 0; y < img_height; ++y) {
					for (int x = 0; x < img_width; ++x, cur_data += model_info_.output_tensors[0].depth) {
						auto pixel = Utilities::SoftMax(&finalTensor_[i][cur_data], model_info_.output_tensors[0].depth);
						for (int z = 1; z < model_info_.output_tensors[0].depth; z++) {
							if (pixel[z] > 0.5) {
								img.at<Vec3b>(y, x)[0] = 0;
								img.at<Vec3b>(y, x)[1] = 0;
								img.at<Vec3b>(y, x)[2] = 255;
								count_of_cloud_pixels++;
							}
							count_of_pixels++;
						}
					}

				}
			}
			else if (model_info_.output_tensors[0].depth == 1) {
				for (int y = 0, cur_data = 0; y < img_height; ++y) {
					for (int x = 0; x < img_width; ++x, cur_data += model_info_.output_tensors[0].depth) {
						if (finalTensor_[i][cur_data] >= 0.5f) {
							img.at<Vec3b>(y, x)[0] = 0;
							img.at<Vec3b>(y, x)[1] = 0;
							img.at<Vec3b>(y, x)[2] = 255;
							count_of_cloud_pixels++;
						}
						count_of_pixels++;
					}
				}
			}
			string addr = imageController_.getFrameNames()[i];
			int begin = addr.find('I');
			int end = addr.find('.');
			string buff = "images\\segmentation\\";
			for (int i = begin; i < end; i++)
				buff += addr[i];
			buff += "_segm.bmp";
			imwrite(buff, img);
		}
		float cloud_percent = count_of_cloud_pixels * 1.0 / count_of_pixels * 100;
		cout << "Процент облачности: " << cloud_percent << endl;
		writeStatus("создана маска облачности, определен процент облачности");
		return cloud_percent;
	}
	catch (...) {
		writeError("не удалось создать маску облачности и определить процент облачности");
	}
}

void Script::compressImage() {
	Mat img = imread("images\\assemblyBmp\\result.bmp");
	imwrite("images\\assemblyBmp\\pressResult.jpg", img);
}

float Script::imageAssembly() {
	try {
		long int count_of_pixels = 1;
		long int count_of_cloud_pixels = 1;
		Mat img = imread(imageController_.getFileName());
		int wTens = model_info_.output_tensors[0].width;
		int hTens = model_info_.output_tensors[0].height;
		int countX = imageController_.image_width / wTens;
		int countY = imageController_.image_height / hTens;
		int smX = 0;
		int smY = 0;
		cout << countX << " " << countY << endl;
		for (int t = 0; t < finalTensor_.size(); t++) {
			if (model_info_.output_tensors[0].depth > 1) {
				for (int y = smY * hTens, cur_data = 0; (y < (smY + 1) * hTens) && (y < img.size().height); ++y) {
					for (int x = smX * wTens; (x < (smX + 1) * wTens) && (x < img.size().width); ++x, cur_data += model_info_.output_tensors[0].depth) {
						auto pixel = Utilities::SoftMax(&finalTensor_[t][cur_data], model_info_.output_tensors[0].depth);
						for (int z = 1; z < model_info_.output_tensors[0].depth; z++) {
							if (pixel[z] > 0.5) {
								img.at<Vec3b>(y, x)[0] = 0;
								img.at<Vec3b>(y, x)[1] = 0;
								img.at<Vec3b>(y, x)[2] = 255;
								count_of_cloud_pixels++;
							}
							count_of_pixels++;
						}
					}

				}
			}
			else if (model_info_.output_tensors[0].depth == 1) {
				for (int y = smY * hTens, cur_data = 0; (y < (smY + 1) * hTens) && (y < img.size().height); ++y) {
					for (int x = smX * wTens; (x < (smX + 1) * wTens) && (x < img.size().width); ++x, cur_data += model_info_.output_tensors[0].depth) {
						if (finalTensor_[t][cur_data] >= 0.5f) {
							img.at<Vec3b>(y, x)[0] = 0;
							img.at<Vec3b>(y, x)[1] = 0;
							img.at<Vec3b>(y, x)[2] = 255;
							count_of_cloud_pixels++;
						}
						count_of_pixels++;
					}
				}
			}
			smX++;
			if (smX == countX) {
				smY++;
				smX = 0;
			}
		}
		string buff = "images\\assemblyBmp\\result.bmp";
		imwrite(buff, img);
		float cloud_percent = count_of_cloud_pixels * 1.0 / count_of_pixels * 100;
		cout << "Процент облачности: " << cloud_percent << endl;
		writeStatus("создана маска облачности, определен процент облачности");
		compressImage();
		writeStatus("результат сжат");
		return cloud_percent;
	}
	catch (Exception e) {
		writeError("не удалось создать маску облачности и определить процент облачности"+e.msg);
	}
}


void Script::readConfig() {
	ifstream configFile("config.txt");
	uint32_t buffer, buffer1, buffer2;
	string rowName, choise;
	configFile >> rowName >> buffer;
	nmdlController_.setBoardType(buffer);
	configFile >> rowName >> buffer;
	nmdlController_.setImageConverterColor(buffer);
	configFile >> rowName >> buffer;
	configFile >> rowName >> buffer1;
	configFile >> rowName >> buffer2;
	nmdlController_.setWorkMode(buffer, buffer1, buffer2);
	nmdlController_.setConverter();
	nmdlController_.showModeNmdl();
	nmdlController_.createNmdlObject();
	configFile >> rowName >> buffer;
	model_.setTypeModel(buffer);
	configFile >> rowName >> choise;
	model_.setNameModel(choise);
	configFile >> rowName >> choise;
	if (model_.getTypeModel() == 1)
		model_.setWeightModel(choise);
	configFile >> rowName >> buffer;
	model_.setNeedCompilation(buffer);
	float add[3];
	float div[3];
	configFile >> rowName >> add[0] >> add[1] >> add[2];
	configFile >> rowName >> div[0] >> div[1] >> div[2];
	model_.setAdderDivider(add, div);
	model_.showModeModel();
	loadModels();
}

void Script::readConfigForOneImage() {
	ifstream configFile("config.txt");
	uint32_t buffer, buffer1, buffer2;
	string rowName, choise;
	configFile >> rowName >> buffer;
	nmdlController_.setBoardType(buffer);
	configFile >> rowName >> buffer;
	nmdlController_.setImageConverterColor(buffer);
	configFile >> rowName >> buffer;
	configFile >> rowName >> buffer1;
	configFile >> rowName >> buffer2;
	nmdlController_.setWorkMode(buffer, buffer1, buffer2);
	nmdlController_.setConverter();
	nmdlController_.showModeNmdl();
	nmdlController_.createNmdlObject();
	configFile >> rowName >> buffer;
	model_.setTypeModel(buffer);
	configFile >> rowName >> choise;
	model_.setNameModel(choise);
	configFile >> rowName >> choise;
	if (model_.getTypeModel() == 1)
		model_.setWeightModel(choise);
	configFile >> rowName >> buffer;
	model_.setNeedCompilation(buffer);
	float add[3];
	float div[3];
	configFile >> rowName >> add[0] >> add[1] >> add[2];
	configFile >> rowName >> div[0] >> div[1] >> div[2];
	model_.setAdderDivider(add, div);
	model_.showModeModel();
	loadModels();
	configFile >> rowName >> choise;
	choise = "E:\\Programm\\Clouds 1.6 config\\Clouds\\images\\" + choise;
	imageController_.setFileName(choise);
}


void Script::cycleSegmentation() {
	int start = clock();
	int start_segm;
	readConfig();
	databaseController_.databaseConnection();
	vector<string> nameImages = databaseController_.getImagesAddress();
	vector<int> idImages = databaseController_.getImagesId();
	for (int i = 0; i < 1/*nameImages.size()*/; i++) {
		start_segm = clock();
		setImage(nameImages[i]);
		databaseController_.setImageData(idImages[i], imageController_.image_width,
			imageController_.image_height, "test");
		prepareTensors();
		startProcess();
		float res = getResultSegmentation();
		cloud_percent_.push_back(res);
		float t = (clock() - start_segm) * 1.0 / 1000;
		time_images_segmentation_.push_back(t);
		databaseController_.setNmdlExperimetData(idImages[i], res, time_images_segmentation_[i]);
	}
	time_calculation_ = (clock() - start) * 1.0 / 1000;
	cout << time_calculation_;
}

void Script::segmentationOneImage() {
	int start = clock();
	readConfigForOneImage();
	setImage(imageController_.getFileName());
	prepareTensors();
	startProcess();
	float res = imageAssembly();
	cloud_percent_.push_back(res);
	time_calculation_ = (clock() - start) * 1.0 / 1000;
	cout << time_calculation_;
}

