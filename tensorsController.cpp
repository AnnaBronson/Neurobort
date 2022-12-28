#include <iostream>
#include <vector>

#include "nmdl.h"
#include "nmdl_image_converter.h"

#include "tensorsController.h"
#include "logs.h"

using namespace std;

TensorsController::TensorsController() {

}

TensorsController::~TensorsController() {

}

void TensorsController::createInputTensor(uint32_t width,
	uint32_t height, uint32_t image_converter_board_type,
	uint32_t image_converter_color_format) {
	try {
		vector<float>inputBuff(NMDL_IMAGE_CONVERTER_RequiredSize(width,
			height, image_converter_color_format, image_converter_board_type));
		inputs.push_back(inputBuff);
	}
	catch (...) {
		writeError("невозможно создать входные тенхоры");
	}
}

void TensorsController::createOutputTensors(NMDL_ModelInfo info) {
	try {
		vector<float> output;
		for (size_t j = 0; j < info.output_tensor_num; ++j) {
			output.resize(static_cast<size_t>(
				info.output_tensors[j].width) *
				info.output_tensors[j].height *
				info.output_tensors[j].depth);
			outputs.push_back(output);
		}
		writeStatus("выходные тензоры созданы");
	}
	catch (...) {
		writeError("невозможно создать выходные тензоры");
	}
}
