#pragma once
#include <fstream>
using namespace std;

//вопрос пользователю
bool InputQuery();
//считывание строки из бинарного файла
string StrFromBin(fstream *f);
//запись строки в бинарный файл
void StrToBin(fstream *f, string str);
//Обрезка строки
string trim(const string& str);
//ввод номера
bool InputNum(double & num);
bool InputNum(int32_t &num);
//Функция для ввода строковой информации
string InputInformation();
//Показать заголовок
void ShowHeader();