// Функция чтения данных из файла в вектор заданного типа
#ifndef _READ_FILE_H_
#define _READ_FILE_H_
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

template <typename T>
vector<T> ReadFile(string filename) {
	ifstream ifs(filename, ios::binary | ios::ate);
	if (!ifs.is_open()) {
		throw runtime_error("Невозможно открыть файл: " + filename);
	}
	auto fsize = static_cast<size_t>(ifs.tellg());
	ifs.seekg(0);
	vector<T> data(fsize / sizeof(T));
	ifs.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(T));
	return data;
}

#endif