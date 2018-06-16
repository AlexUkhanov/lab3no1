#include <iostream>
#include <iomanip>
#include <ctime>
#include "HelpUtils.h"
#include "Deposit.h"

int32_t const countTypes = 3;
TypeDeposit typDep[countTypes];
//Изменение типа счета
int ChangeTypeDep(TypeDeposit & tp) {
	int ans;
	do {
		cout << "1 - Название вклада" << endl;
		cout << "2 - Возможность пополнять средства" << endl;
		cout << "3 - Возможность снимать средства" << endl;
		cout << "4 - Процентная ставка" << endl;
		cout << "0 - Отмена" << endl;
	} while (!InputNum(ans) || ans < 0 || ans > 4);
	if (ans == 0) {
		//пользователь ввел отмену
		return 2;
	}
	string str;
	double per;
	switch (ans) {
	case 1:
		cout << "Введите название вклада" << endl;
		cin >> str;
		if (str == "") {
			//не удалось изменить счет
			return 1;
		}
		tp.typeName = str;
		//счет успешно изменен
		return 0;
	case 2:
		cout << "Разрешить пополнение средств? " << endl;
		tp.incMoney = InputQuery();
		//успешно
		return 0;
	case 3:
		cout << "Разрешить снятие средств? " << endl;
		tp.decMoney = InputQuery();
		//успешно
		return 0;
	case 4:
		cout << "Введите процентную ставку:" << endl;
		if (!InputNum(per)) {
			//ошибка
			return 1;
		}
		tp.percent = per;
		//успешно
		return 0;
	}
}

//Вывод заголовка типов
void TypeDepHeader()
{
	cout << "--------------------------------------------------------------" << endl;
	cout << setw(1) << left << "|№" << "|"
		<< setw(19) << left << "Название"<<"|"
		<< setw(14) << left << "В-сть пополн."<<"|"
		<< setw(14) << left << "В-сть сним"<<"|"
		<< setw(8) << left << "Процент" <<"|"<< endl;
	cout << "--------------------------------------------------------------" << endl;
}

//вывод типов в консоль
void TypeDepToConsole(TypeDeposit tp) {
	cout << setw(1) << left << "|" << setw(19) << left << tp.typeName << "|"
		<< setw(14) << left << (tp.incMoney ? "да" : "нет") << "|"
		<< setw(14) << left << (tp.decMoney ? "да" : "нет") << "|"
		<< setw(8) << left << tp.percent * 100 << "|" << endl;
}

//ввод с консоли
bool OpenDeposit(Deposit &dep) {
	cout << "Введите ФИО: ";
	string str=InputInformation();
	if (trim(str) == "") {
		return false;
	}
	dep.fio = str;
	int len = sizeof(typDep) / sizeof(typDep[0]);
	cout << "Выберите номер типа" << endl;
	TypeDepHeader();

	for (int i = 0; i < len; ++i) {
		cout <<setw(1)<<left<<"|"<<setw(1)<<left<< i + 1;
		TypeDepToConsole(typDep[i]);
	}
	cout << "--------------------------------------------------------------" << endl;
	while (!InputNum(dep.idTypeDep) || dep.idTypeDep < 1 || dep.idTypeDep > len) {
		cout << "Введено неверное значение, повторите попытку" << endl;
	}
	--dep.idTypeDep;
	cout << "Введите сумму: ";
	while (!InputNum(dep.sum) || dep.sum < 0) {
		cout<<"Введена некорректная сумма, повторите ввод:"<<endl;
	}
	cout << "Введите дату открытия счета (дд.мм.гггг): ";
	cin >> str;
	while (!DateFromStr(str, dep.date)) {
		cout << "Введена некорректная дата, повторите ввод" << endl;
		cout << "Введите дату открытия счета (дд.мм.гггг): ";
		cin >> str;
	}
	cout << "Введите срок вклада в месяцах: ";
	while (!InputNum(dep.period) || dep.period < 1) {
		cout << "Введен некорректный срок, повторите ввод:" << endl;
	}
	cout << "Введите код счета (100-999): ";
	while (!InputNum(dep.code) || dep.code > 999 || dep.code <100) {
		cout << "Введена код счета, повторите ввод:" << endl;
	}
	srand(time(NULL));
	dep.number = rand() % 100000;
	ShowHeader();
	ShowDeposit(dep);
	cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "Вы уверены, что хотите открыть вклад? ";
	if (!InputQuery()) {
		return false;
	}

	return true;
}

//вывод на консоль
void ShowDeposit(Deposit dep) {
	cout << setw(1) << "|" << setw(7) << left << dep.number << "|"
		<< setw(4) << left << dep.code<<"|"
		<< setw(15) << left << dep.fio << "|"
		<< setw(10) << left << dep.sum << "|"
		<< setw(15) << left << DateToStr(dep.date) << "|"
		<< setw(5) << left << dep.period ;
	TypeDepToConsole(typDep[dep.idTypeDep]);
}

//снятие и зачисление средств для клиента
bool ChangeDepositForUser(Deposit &dep) {
	int ans;
	cout << "Выберите действие" << endl;
	do {
		cout << "1 - Снять средства" << endl;
		cout << "2 - Пополнить счет" << endl;
		cout << "0 - Отмена" << endl;
	} while (!InputNum(ans) || ans < 0 || ans > 2);
	if (ans == 0) {
		return false;
	}
	int quant;
	//снятие средств
	if (ans == 1) {
		if (!typDep[dep.idTypeDep].decMoney) {
			cout
				<< "Вы не можете снимать средства при данном типе вклада" <<
				endl;
			return false;
		}
		cout << "Введите сумму: ";
		if (!InputNum(quant) || quant >= dep.sum) {
			cout << "Введена неверная сумма" << endl;
			return false;
		}
		dep.sum -= quant;
		return true;
	}
	//пополнение средств
	if (!typDep[dep.idTypeDep].incMoney) {
		cout
			<< "Вы не можете пополнять средства при данном типе вклада" <<
			endl;
		return false;
	}
	cout << "Введите сумму: ";
	if (!InputNum(quant)) {
		cout << "Введена неверная сумма" << endl;
		return false;
	}
	dep.sum += quant;
	return true;
}

//считывание счета из файла
bool DepositFromFile(Deposit &dep, fstream *f) {
	Deposit d;
	if (!f->is_open()) {
		return false;
	}
	string str;
	try {
		f->read(reinterpret_cast<char*>(&(d.number)), sizeof(d.number));
		f->read(reinterpret_cast<char*>(&(d.code)), sizeof(d.code));
		str = StrFromBin(f);
		d.fio = str;
		f->read(reinterpret_cast<char*>(&(d.sum)), sizeof(d.sum));
		f->read(reinterpret_cast<char*>(&(d.idTypeDep)), sizeof(d.idTypeDep));
		if (!DateFromBin(d.date, f)) {
			return false;
		}
		f->read(reinterpret_cast<char*>(&(d.period)), sizeof(d.period));
		dep = d;
		return true;
	}
	catch (...) {
		throw "Не удалось считать из файла";
	}
}

//вывод в файл
void DepositToFile(Deposit dep, fstream *f) {
	f->write(reinterpret_cast<char*>(&(dep.number)), sizeof(dep.number));
	f->write(reinterpret_cast<char*>(&(dep.code)), sizeof(dep.code));
	StrToBin(f, dep.fio);
	f->write(reinterpret_cast<char*>(&(dep.sum)), sizeof(dep.sum));
	f->write(reinterpret_cast<char*>(&(dep.idTypeDep)), sizeof(dep.idTypeDep));
	DateToBin(dep.date, f);
	f->write(reinterpret_cast<char*>(&(dep.period)), sizeof(dep.period));
}

//загрузка типов счетов из файла
void LoadTypesFromFile(string fileName)
{
	fstream file;
	file.open(fileName, ios::binary | ios::in);
	if (file.is_open()) {
		int i = 0;
		bool OK = true;
		string str;
		while (OK) {
			str = StrFromBin(&file);
			if (str == "") {
				break;
			}
			typDep[i].typeName = str;
			OK = 
				file.read(reinterpret_cast<char*>(&(typDep[i].incMoney)), sizeof(typDep[i].incMoney)) &&
				file.read(reinterpret_cast<char*>(&(typDep[i].decMoney)), sizeof(typDep[i].decMoney)) &&
				file.read(reinterpret_cast<char*>(&(typDep[i].percent)), sizeof(typDep[i].percent));
			++i;
		}
		file.close();
	}
	else {
		typDep[0].typeName = "Вклад_1";
		typDep[0].decMoney = typDep[0].incMoney = false;
		typDep[0].percent = 0.07;

		typDep[1].typeName = "Вклад_2";
		typDep[1].decMoney = false;
		typDep[1].incMoney = true;
		typDep[1].percent = 0.02;

		typDep[2].typeName = "Вклад_3";
		typDep[2].decMoney = typDep[2].incMoney = true;
		typDep[2].percent = 0.12;
	}
}

//сохранение типов счетов в файл
void SaveTypesToFile(string fileName) {
	fstream file;
	file.open(fileName, ios::out | ios::binary);
	if (!file.is_open())
		throw "file is not open";
	int i;
	for (i = 0; i < countTypes; ++i) {
		StrToBin(&file, typDep[i].typeName);
		file.write(reinterpret_cast<char*>( &(typDep[i].incMoney) ), sizeof(typDep[i].incMoney) );
		file.write(reinterpret_cast<char*>( &(typDep[i].decMoney) ), sizeof(typDep[i].decMoney));
		file.write(reinterpret_cast<char*>( &(typDep[i].percent) ), sizeof(typDep[i].percent));
	}
	file.close();
}





