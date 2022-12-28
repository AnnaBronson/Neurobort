#include <iostream>

#include "script.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	// Создание экземпляра сценария
	Script script;
	// Запуск процесса эксперимента
	script.segmentationOneImage();
	return 0;
}

