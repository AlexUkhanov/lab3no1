#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "HelpUtils.h"

using namespace std;

//класс контейнер 
template <typename T>
class Container
{
private:
	vector<T> _accs; //контейнер
	string _sourceName;
	void(*_save)(T, fstream*); //функция сохранения
	bool (*_load)(T&, fstream*); //функция загрузки

	//сохранение в файл
	void Save() {
		if (_sourceName == "") {
			return;
		}
		fstream file;
		file.open(_sourceName, ios::binary | ios::out);
		for (T el : _accs) {
			_save(el, &file);
		}
		file.close();
	}

public:

	Container() {
		_sourceName = "";
	}

	//конструктор
	Container(string fileName, bool (*ld)(T&, fstream*),
		void (*sv)(T, fstream*))
	{
		_load = ld;
		_save = sv;
		if (fileName == "") {
			_sourceName = "DefaultSource.bin";
		}
		else {
			_sourceName = fileName;
		}
		fstream file;
		file.open(_sourceName, ios::binary | ios::in);
		if (file.is_open()) {
			T el;
			while (_load(el, &file)) {
				Add(el);
			}
		}
		else {
			file.open(_sourceName, ios::binary | ios::out);
		}
		file.close();
	}

	//деструктор
	~Container() {
		Save();
		_accs.clear();
	}

		//вывод в консоль
	void ToConsole(void(*output)(T)) {
		for (T r : _accs) {
			output(r);
		}
		cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << endl <<"Всего записей: " << _accs.size() << endl;
	}
	
	//кол-во элементов в контейнере
	int Count() {
		return _accs.size();
	}
	
	//получить элемент по индексу
	T GetElem(int ind) {
		if (ind < 0 || ind >= _accs.size()) {
			throw "Неверный индекс";
		}
		return _accs[ind];
	}

	//сортировка
	void Sort(bool(*compare)(T, T)) {
		sort(_accs.begin(), _accs.end(), compare);
	}

	//возвращает индекс элемента с заданным критерием
	int LinearSearch(bool(*func)(T, T), T crit) {
		int i = 0;
		for (T rec : _accs) {
			if (func(rec, crit)) {
				return i;
			}
			++i;
		}
		return -1;
	}

	//выборка подмножества по заданному критерию
	Container<T> LinearSearchSubSet(bool(*func)(T, T), T crit) {
		Container<T> subSet;
		for (T rec : _accs) {
			if (func(rec, crit)) {
				subSet.Add(rec);
			}
		}
		return subSet;
	}


	//очистка
	void Clear() {
		_accs.clear();
		Save();
	}

	//добавить 
	void Add(T client) {
		_accs.push_back(client);
		Save();
	}

	//удаление по индексу
	void RemoveInd(int ind){
		if (ind < 0 || ind >= _accs.size()) {
			throw "Неверный индекс";
		}
		_accs.erase(_accs.begin() + ind);
		Save();
	}


	//изменение записи в консоли
	bool Change(bool(*func)(T&), int ind) {
		if (ind < 0 || ind >= _accs.size()) {
			return false;
		}
		func(_accs[ind]);
		Save();
		return true;
	}


	//ввод с консоли
	void FromConsole(bool(*input)(T&)){
		T t;
		do {
			cout << endl;
			if (input(t)) {
				Add(t);
				cout << "Запись успешно довавлена, еще? ";
			}
			else {
				cout << "Введено некорректное значение, повторить? ";
			}
		} while (InputQuery());
		Save();
	}

	//считывание из файла
	bool FromFile(bool(*input)(T&, fstream*), fstream *f) {
		if (!f->is_open()) {
			return false;
		}
		T tmp;
		while (!f->eof()) {
			if (input(tmp, f)) {
				Add(tmp);
			}
			else {
				return false;
			}
		}
		Save();
		return true;
	}

	//запись в поток
	void ToFile(void(*output)(T, fstream*), fstream *f) {
		if (!f->is_open()) {
			throw "Файл не открыт";
		}
		for (T r : _accs) {
			output(r, f);
		}
	}

};
