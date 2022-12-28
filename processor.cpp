#include <iostream>

#include "nmdl.h"

#include "call.h"
#include "processor.h"
#include "logs.h"

Processor::Processor() {

}

Processor::~Processor() {

}

void Processor::waitForOutput(NMDL_HANDLE nmdl, uint32_t unit_num, float* outputs[]) {
	try {
		uint32_t status = NMDL_PROCESS_FRAME_STATUS_INCOMPLETE;
		while (status == NMDL_PROCESS_FRAME_STATUS_INCOMPLETE) {
			NMDL_GetStatus(nmdl, unit_num, &status);
		};
		Call(NMDL_GetOutput(nmdl, unit_num, outputs, &fps), "GetOutput");
		cout << "Первые четыре результата:" << std::endl;
		for (size_t i = 0; i < 4; ++i) {
			cout << outputs[unit_num][i] << std::endl;
		}
		cout << "fps:" << fps << endl;
		writeStatus("результаты вычислений на юните номер " + to_string(unit_num) + " получены");
	}
	catch (...) {
		writeError("невозможно получить результаты вычислений");
	}
}
