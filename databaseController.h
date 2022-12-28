// Класс, описывающий работу с базой данных
#ifndef _DATABASE_CONTROLLER_H_
#define _DATABASE_CONTROLLER_H_

#include <iostream>
#include <vector>
// Класс, предназначенный для работы с базой данных
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000


class DatabaseController {
public:
    // Параметры для подключения
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
    // Конструктор
    DatabaseController();
    // Подключение к базе данных
    void databaseConnection();
    // Отключение от базу данных
    void databaseDisconnection();
    void insertWorkTime(float work_time);
    void deleteWorkTime(int id);
    // Получение адресов изображений
    vector<string> getImagesAddress();
    // Получение номеров изображений
    vector<int> getImagesId();
    // Установка метаданных об изобаржениях
    void setImageData(int id, int width, int height, string selection);
    // Добавление данных о вычислениях
    void setNmdlExperimetData(int id_image, float clouds, float time);
};
#endif
