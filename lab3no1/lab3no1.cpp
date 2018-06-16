/*
Реализовать виртуальный банк. Предусмотрено два режима работы 
с банком — режим пользователя и режим сотрудника. Пользователь имеет 
возможность открыть счет под заданный процент (выбирается из доступных видов вкладов), 
закрыть счет, снять часть средств со счета (если допускается видом вклада), посмотреть остаток 
средств на счете. Сотрудник банка может редактировать список видов вкладов (название вида, 
процентная ставка, возможность частичного снятия суммы), просматривать список вкладчиков (вкладчик, 
вид вклада, срок вклада, дата открытия вклада, сумма на счету), список вкладчиков должен иметь 
возможность фильтрации по любому полю.
*/
#include <iostream>
#include <string>
#include <iomanip>
#include "Deposit.h"
#include "DateUtils.h"
#include "Container.h"
#include <Windows.h>

using namespace std;

//Меню для клиента
void MenuClient(Container<Deposit> &bank)
{
	int answer,num;
	while (true)
	{
		do {
			cout << "********************************" << endl;
			cout << "1 - Открыть счет" << endl;
			cout << "2 - Просмотреть счета" << endl;
			cout << "0 - Отмена" << endl;
			cout << "********************************" << endl;
		} while (!InputNum(answer) || answer < 0 || answer > 3);
		if (answer == 0) {
			break;
		}
		cout << endl;
		int ind;
		Deposit dep;
		Container<Deposit> cnt;
		if (answer == 1) {
			//открыть счет
			if (OpenDeposit(dep)) {
				bank.Add(dep);
				cout << "Счет открыт." << endl;
			}
			else {
				cout << "Введены неверные данные." << endl;
			}
		}
		else {
			//ввод номера
			string str;
			cout << "Введите ФИО: ";
			str = InputInformation();
			if (trim(str) == "") {
				cout << "Введено пустое поле" << endl;
				break;
			}
			dep.fio = str;
			for (;;) {
				cnt = bank.LinearSearchSubSet(
					[](Deposit d, Deposit crit) {
					return d.fio == crit.fio;
				},
					dep);
				if (cnt.Count() == 0) {
					cout << "На данные ФИО счетов не зарегестрировано" << endl;
					break;
				}
				ShowHeader();
				cnt.ToConsole(ShowDeposit);
				
				do {
					cout << "********************************" << endl;
					cout << "1 - Изменить средства" << endl;
					cout << "2 - Закрыть счет" << endl;
					cout << "0 - Назад" << endl;
					cout << "********************************" << endl;
				} while (!InputNum(answer) || answer < 0 || answer > 3);
				if (answer == 0) {
					break;
				}
				//ввод номера
				cout << "Введите номер счета: ";
				if (!InputNum(num) || num < 0) {
					break;
				}
				dep.number = num;
				ind = cnt.LinearSearch(
					[](Deposit d, Deposit crit) {
					return d.number == crit.number;
				},
					dep);
				if (ind == -1) {
					cout << "У вас нет такого счета." << endl;
					break;
				}
				//изменение средств
				if (answer == 1) {
					if (bank.Change(ChangeDepositForUser, ind)) {
						cout << "Счет изменен." << endl;
					}
					else {
						cout << "Не удалось изменить счет." << endl;
					}
				}
				//закрытие счета
				else {
					bank.RemoveInd(ind);
					cout << "Счет закрыт." << endl;
				}
			}
		}
		system("pause");
	}
}

//Меню для сотрудника
void MenuEmployee(Container<Deposit> &bank)
{
	int ans, num;
	Deposit dep;
	int len;
	for (;;) {
		do {
			cout << "********************************" << endl;
			cout << "1 - Отфильтровать список вкладчиков" << endl;
			cout << "2 - Изменить тип вклада" << endl;
			cout << "3 - Просмотреть всех вкладчиков" << endl;
			cout << "0 - Отмена" << endl;
			cout << "********************************" << endl;
		} while (!InputNum(ans) || ans < 0 || ans > 3);
		if (ans == 0) {
			break;
		}
		cout << endl;
		Deposit crit;
		Container<Deposit> subs;
		int num;
		string str;
		Date date;
		int res;
		switch (ans)
		{
		case 1:
			cout << "Отфильтровать по:" << endl;
			do {
				cout << "1 - Номеру счета" << endl;
				cout << "2 - ФИО" << endl;
				cout << "3 - Сумме" << endl;
				cout << "4 - Названию вклада" << endl;
				cout << "5 - Возможности пополнять счет" << endl;
				cout << "6 - Возможности снимать средства" << endl;
				cout << "7 - Дате открытия" << endl;
				cout << "8 - Сроку договора" << endl;
				cout << "9 - Процентной ставке" << endl;
				cout << "0 - Отмена" << endl;
			} while (!InputNum(ans) || ans < 0 || ans > 9);
			if (ans == 0) {
				break;
			}

			switch (ans)
			{
			case 1:
				cout << "Введите номер счета: ";
				if (!InputNum(num) || num < 0) {
					cout << "Введены неверные данные" << endl;
					break;
				}
				crit.number = num;
				subs = bank.LinearSearchSubSet(
					[](Deposit dep, Deposit crit) {
					return dep.number == crit.number;
				}, crit);
				break;
			case 2:
				cout << "Введите ФИО: ";
				str = InputInformation();
				if (trim(str) == "") {
					cout << "Введены неверные данные" << endl;
					break;
				}
				crit.fio = str;
				subs = bank.LinearSearchSubSet(
					[](Deposit dep, Deposit crit) {
					return dep.fio == crit.fio;
				}, crit);
				break;
			case 3:
				cout << "Введите сумму: ";
				if (!InputNum(num) || num < 0) {
					cout << "Введены неверные данные" << endl;
					break;
				}
				crit.sum = num;
				subs = bank.LinearSearchSubSet(
					[](Deposit dep, Deposit crit) {
					return dep.sum == crit.sum;
				}, crit);
				break;
			case 4:
				cout << "Выберите вклад " << endl;
				do {
					cout << "1 - " << typDep[0].typeName << endl;
					cout << "2 - " << typDep[1].typeName << endl;
					cout << "3 - " << typDep[2].typeName << endl;
				} while (!InputNum(ans) || ans < 1 || ans > 3);
				crit.idTypeDep = ans;
				subs = bank.LinearSearchSubSet(
					[](Deposit dep, Deposit crit) {
					return dep.idTypeDep == crit.idTypeDep;
				}, crit);

				break;
			case 5:
				cout << "По способности пополнять средства: ";
				subs = bank.LinearSearchSubSet(
					[](Deposit d, Deposit crit) {
					return typDep[d.idTypeDep].incMoney;
				},
					crit);
				break;
			case 6:
				cout << "По способности снимать средства: ";
				subs = bank.LinearSearchSubSet(
					[](Deposit d, Deposit crit) {
					return typDep[d.idTypeDep].decMoney;
				},
					crit);
				break;
			case 7:
				cout << "Введите дату: ";
				cin >> str;
				if (!DateFromStr(str, date)) {
					cout << "Введены неверные данные" << endl;
					break;
				}
				crit.date = date;
				subs = bank.LinearSearchSubSet(
					[](Deposit d, Deposit crit) {
					return CompareDates(d.date, crit.date) == 0;
				},
					crit);
				break;
			case 8:
				cout << "Введите срок вклада: ";
				if (!InputNum(num)) {
					cout << "Введены неверные данные" << endl;
					break;
				}
				crit.period = num;
				subs = bank.LinearSearchSubSet(
					[](Deposit d, Deposit crit) {
					return d.period == crit.period;
				},
					crit);
				break;
			case 9:
				cout << "Введите процент: ";
				do {
					cout << "1 - " << typDep[0].percent << endl;
					cout << "2 - " << typDep[1].percent << endl;
					cout << "3 - " << typDep[2].percent << endl;
				} while (!InputNum(ans) || ans < 1 || ans > 3);
				crit.idTypeDep = ans;
				subs = bank.LinearSearchSubSet(
					[](Deposit dep, Deposit crit) {
					return dep.idTypeDep == crit.idTypeDep;
				}, crit);
				break;
			}
			ShowHeader();
			subs.ToConsole(ShowDeposit);
			break;
			//изменить тип вклада
		case 2:
			len = sizeof(typDep) / sizeof(typDep[0]);

			do {
				cout << "Выберите номер типа" << endl;
				TypeDepHeader();

				for (int i = 0; i < len; ++i) {
					cout << setw(1) << left << "|" << setw(1) << left << i + 1;
					TypeDepToConsole(typDep[i]);
				}
				cout << "--------------------------------------------------------------" << endl;
			} while (!InputNum(ans) || ans < 1 || ans > 3);
			res = ChangeTypeDep(typDep[--ans]);
			if (res == 0) {
				cout << "Вклад успешно изменен" << endl;
				SaveTypesToFile("types.bin");
			}
			else if (res == 1) {
				cout << "Не удалось изменить вклад" << endl;
			}
			break;
			//показ всех вкладчиков
		case 3:
			cout << endl;
			ShowHeader();
			bank.ToConsole(ShowDeposit);
			break;
		}
		cout << endl;
		system("pause");
		cout << endl;
	}
}

int main()
{
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	setlocale(LC_ALL, "Rus"); // Подключение русского языка
	Container<Deposit> bank("bank.bin", DepositFromFile, DepositToFile);
	int ans;
	LoadTypesFromFile("types.bin");
	do
	{
		do {
			cout << "********************************" << endl;
			cout << "Выберите тип учетной записи:" << endl;
			cout << "1 - Клиент" << endl;
			cout << "2 - Сотрудник банка" << endl;
			cout << "0 - Выход" << endl;
			cout << "********************************" << endl;
		} while (!InputNum(ans) || ans < 0 || ans > 2);
		switch (ans)
		{
			case 0:
				break;
			case 1:
				MenuClient(bank);
				break;
			case 2:
				MenuEmployee(bank);
				break;
		}
	} while (ans != 0);
	SaveTypesToFile("types.bin");
    return 0;
}

