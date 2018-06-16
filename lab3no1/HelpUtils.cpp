#include <iostream>
#include <iomanip>
#include <sstream>
#include "HelpUtils.h"


//считывание строки из бинарного файла
string StrFromBin(fstream *f) {
	string result;
	int32_t len;
	if (!f->read(reinterpret_cast<char*>(&len), sizeof(len))) {
		return "";
	}
	char *buf = new char[len];
	if (!f->read(buf, len)) {
		return "";
	}
	result = buf;
	delete[] buf;
	return result;
}

//Обрезка строки
string trim(const string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return
		str.substr(first, (last - first + 1));
}

//Функция для ввода строковой информации
string InputInformation()
{
	string a;
	getline(cin, a);
	while (trim(a) == "")
	{
		cout << "Ошибка. Повторите ввод:" << endl;
		getline(cin, a);
	}
	return a;
}


//запись строки в бинарный файл
void StrToBin(fstream *f, string str) {
	int32_t len = str.length() + 1;
	f->write(reinterpret_cast<char*>(&len), sizeof(len));
	f->write(str.c_str(), len);
}

void ShowHeader() {
	cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << setw(1) << "|" << setw(7) << left << "Номер"<<"|"
		<<setw(4)<<left<<"Код"<<"|"
		<< setw(15) << left << "ФИО" <<"|"
		<< setw(10) << left << "Сумма"<<"|"
		<< setw(15) << left << "Дата"<<"|"
		<< setw(5) << left << "Срок"<<"|"
		<< setw(19) << left << "Название" << "|"
		<< setw(14) << left << "В-сть пополн." << "|"
		<< setw(14) << left << "В-сть сним" << "|"
		<< setw(8) << left << "Процент" << "|" << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
}

//ввод номера
bool InputNum(double & num) {
	cin >> num;
	bool OK = cin.good();
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	return OK;
}

bool InputNum(int32_t &num) {
	cin >> num;
	bool OK = cin.good();
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	return OK;
}

//вопрос пользователю
bool InputQuery() {
	char c;
	do {
		cout << "Y/N? ";
		cin >> c;
	} while (c != 'n' && c != 'N' && c != 'y' && c != 'Y');
	return c == 'Y' || c == 'y';
}