// Класс, предоставляющий интерфейс пользователю,
// обертка для упрощения работы с системой
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
		// Экземпляр nmdl текущего сценария
		NmdlController nmdlController_;
		// Экземпляр модели текущего сценария
		Model model_;
		// Экземпляр контроллера изображений текущего сценария
		ImageController imageController_;
		// Экземпляр контроллера тензоров текущего сценария
		TensorsController tensorsController_;
		// Экземпляр вычислителя текущего сценария
		Processor processor_;
		// Экземпляр контроллера по работе с базой данных
		DatabaseController databaseController_;
		// Информация о загруженной на экземпляр nmdl модели
		NMDL_ModelInfo model_info_;
		// Итоговый единый выходной тензор
		vector<vector<float>> finalTensor_;
		// Процент облачности
		vector<float> cloud_percent_;
		// Время выполнения вычислений
		float time_calculation_;
		// Время вычисления для каждого изображения
		vector<float> time_images_segmentation_;
		
	public:
		// Конструктор
		Script();
		// Деструктор
		~Script();
		// Создание и настройка экземпляра nmdl
		void createNmdlSession();
		// Настройка модели в режиме multi unit
		void setModelMultiUnit();
		// Настройка моделей врежиме batch
		void setModelsBatch();
		// Настройка модели для текущего экземпляра сценария
		void prepareModel();
		// Настройка входного изображение
		void setImage();
		void setImage(string name);
		// Подготовка входных и выходных тензоров
		void prepareTensors();
		// Вычисления (сегментация или классификация)
		void startProcess();
		// Получение итогового результата: сегментированных 
		// изображений и процента облачности
		float getResultSegmentation();
		// Потоковая обработка данных по адресам, полученным из базы данных
		void cycleSegmentation();
		// Получение начальных данных из файла конфигурации
		void readConfig();
		// Компиляция и загрузка модели на NMDL
		void loadModels();
		// Единое изображение
		float imageAssembly();
		// Чтение кофигурации для одного изображения
		void readConfigForOneImage();
		// Сегментация одного изображения
		void segmentationOneImage();
		// Сжатие изображения
		void compressImage();
};

#endif
