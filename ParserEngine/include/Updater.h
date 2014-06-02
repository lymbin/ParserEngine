/*
 * Updater.h
 *
 *  Created on: 02.06.2014
 *      Author: dmitry
 */

#ifndef UPDATER_H_
#define UPDATER_H_

#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>

// Базовый класс для обновляемых данных
class iUpdateable
{
public:
	iUpdateable(const std::string asName) : msName(asName) {}

	virtual void OnDraw() {}	// Перерисовка
	virtual void Reset() {}		// Сброс настроек
	virtual void OnStart() {}	// Установка начальных данных
	virtual void OnExit() {}	// Выполняем при выходе, на последней итерации фреймов или при удалении
	virtual void Update() {}	// Обновление данных
	const std::string GetName() {return msName;}
private:
	std::string msName;	// Имя(для информации)
};

// Переопределяем типы
// Карта содержит несколько листов обновляющихся данных
typedef std::list<iUpdateable*> tUpdateableList;
typedef tUpdateableList::iterator tUpdateableListIt;

typedef std::map<std::string, tUpdateableList> tUpdateContainerMap;
typedef tUpdateContainerMap::iterator tUpdateContainerMapIt;

// Класс, содержащий все обновляемые объекты, и вызывающий Update и Draw
// для каждого члена каждый кадр
// TODO: Не забыть вести логирование обновщика
class cUpdater
{
public:
	cUpdater();
	~cUpdater();

	void OnDraw();
	void OnStart();
	void Reset();
	void OnExit();
	void Update();

	bool SetContainer(std::string asContainer);

	std::string GetCurrentContainerName();

	bool AddContainer(std::string asName);

	bool AddUpdate(std::string asContainer, iUpdateable *apUpdate);

private:
	std::string msCurrentUpdates;

	tUpdateContainerMap m_mapUpdateContainer;

	tUpdateableList *mpCurrentUpdates;
};


#endif /* UPDATER_H_ */
