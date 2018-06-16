#pragma once
#include <string>
#include <fstream>

using namespace std;

//структура "дата"
struct Date {
	int32_t day;
	int32_t month;
	int32_t year;
};

//считывание даты из строки
bool DateFromStr(std::string str, Date &date);
std::string DateToStr(Date d);
//вывод даты в консоль
void DateToConsole(Date date);

//сравнение дат
int CompareDates(Date d1, Date d2);

//считывание даты из бинарного файла
bool DateFromBin(Date &d, std::fstream *f);

//запись даты в бинарный файл
void DateToBin(Date d, std::fstream *f);

