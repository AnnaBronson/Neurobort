#ifndef _LOGS_H_
#define _LOGS_H_
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

// Функции для осуществления логирования
// Функция для получения текущих даты и времени
string currentDataTime();
// Функция для получения текущей даты
string currentData();
// Функция для записи в файл и вывода на экран сообщения об ошибке
void writeError(string message);
// Функция для записи в файл и выводп на экран сообщения о статусе выполнения программы
void writeStatus(string message);

#endif