// �����, ��������������� ��������� ������������,
// ������� ��� ��������� ������ � ��������
#ifndef _SCRIPT_H_
#define _SCRIPT_H_
#include <iostream>

#include "nmdl.h"

#include "nmdlController.h"
#include "model.h"
#include "imageController.h"
#include "tensorsController.h"
#include "processor.h"
#include "databaseController.h"

using namespace std;

class Script {
	private:
		// ��������� nmdl �������� ��������
		NmdlController nmdlController_;
		// ��������� ������ �������� ��������
		Model model_;
		// ��������� ����������� ����������� �������� ��������
		ImageController imageController_;
		// ��������� ����������� �������� �������� ��������
		TensorsController tensorsController_;
		// ��������� ����������� �������� ��������
		Processor processor_;
		// ��������� ����������� �� ������ � ����� ������
		DatabaseController databaseController_;
		// ���������� � ����������� �� ��������� nmdl ������
		NMDL_ModelInfo model_info_;
		// �������� ������ �������� ������
		vector<vector<float>> finalTensor_;
		// ������� ����������
		vector<float> cloud_percent_;
		// ����� ���������� ����������
		float time_calculation_;
		// ����� ���������� ��� ������� �����������
		vector<float> time_images_segmentation_;
		
	public:
		// �����������
		Script();
		// ����������
		~Script();
		// �������� � ��������� ���������� nmdl
		void createNmdlSession();
		// ��������� ������ � ������ multi unit
		void setModelMultiUnit();
		// ��������� ������� ������� batch
		void setModelsBatch();
		// ��������� ������ ��� �������� ���������� ��������
		void prepareModel();
		// ��������� �������� �����������
		void setImage();
		void setImage(string name);
		// ���������� ������� � �������� ��������
		void prepareTensors();
		// ���������� (����������� ��� �������������)
		void startProcess();
		// ��������� ��������� ����������: ���������������� 
		// ����������� � �������� ����������
		float getResultSegmentation();
		// ��������� ��������� ������ �� �������, ���������� �� ���� ������
		void cycleSegmentation();
		// ��������� ��������� ������ �� ����� ������������
		void readConfig();
		// ���������� � �������� ������ �� NMDL
		void loadModels();
		// ������ �����������
		float imageAssembly();
		// ������ ����������� ��� ������ �����������
		void readConfigForOneImage();
		// ����������� ������ �����������
		void segmentationOneImage();
		// ������ �����������
		void compressImage();
};

#endif
