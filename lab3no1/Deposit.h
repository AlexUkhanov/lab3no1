#pragma once
#include "DateUtils.h"
#include <string>
#include <fstream>


using namespace std;

//тип счета
struct TypeDeposit {
	//Название типа вклада
	string typeName;
	//Возможность пополнения
	bool incMoney;
	//Возможность снятия
	bool decMoney;
	//Процент
	double percent;
};

//счет
struct Deposit {
	//Номер счета
	int32_t number;
	//Код счета
	int32_t code;
	//ФИО
	string fio;
	//Сумма
	double sum;
	//id типа вклада
	int32_t idTypeDep;
	//Дата вклада
	Date date;
	//Период
	int32_t period;
};

//Вывод заголовка типов
void TypeDepHeader();

extern int32_t const countTypes;
//типы счетов
extern TypeDeposit typDep[3];

//изменить тип счета
int ChangeTypeDep(TypeDeposit &tp);

//вывод счета в консоль
void TypeDepToConsole(TypeDeposit tp);

//Открытие счета для клиента
bool OpenDeposit(Deposit &dep);

//Показ информации о счете
void ShowDeposit(Deposit dep);

//изменение в консоли
bool ChangeDepositForUser(Deposit &dep);

//ввод из файла
bool DepositFromFile(Deposit &dep, std::fstream *f);

//вывод в файл
void DepositToFile(Deposit dep, std::fstream *f);

//сохранение типов счетов в файл
void SaveTypesToFile(std::string fileName);

//загрузка типов счетов из файла
void LoadTypesFromFile(std::string fileName);


