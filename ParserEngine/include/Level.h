/*
 * Level.h
 *
 *  Created on: 18.10.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <map>
#include "Sound.h"

class cLevel;

typedef std::map<int, cLevel *> tLevels;
typedef std::map<int, cLevel *>::iterator tLevelsIt;

class cLevelManager {
	cLevelManager();
	~cLevelManager();

	tLevels 	mLevels;
	int			mCurrentLevel;

public:
	bool	Start();
	bool 	LoadLevel(int aLevelId = 0);
	bool 	UnloadLevel(int aLevelId = -1);
	int 	AddLevel(cLevel *aLevel);
	void 	RemoveLevel(int aId);
	int 	NextLevel();
	int 	PreviousLevel();
	int 	GetCurrentLevel();
	cLevel	*GetLevel(int aLevelId);

	int Init();
	int CleanUp();
	int CreatingObjects();
	int LoadTextures();
	void FreeTextures();
	void FreeObjects();
	void Update();
	void PostUpdate();
	void Render();
	void PostRender();

	static cLevelManager *Instance()
	{
		if (!self)
			self = new cLevelManager();

		return self;
	}

	static bool DeleteInstance()
	{
		if (self)
		{
			delete self;
			self = 0;
			return true;
		}
		return false;
	}

protected:
	static cLevelManager 	*self;
};

class cLevelTheme {
public:
	cLevelTheme(std::string aDataFolder);
	~cLevelTheme();

	// Music and sprite sets holds here
private:
	std::string mDataFolder;
	cPlaylist 	*mPlaylist;
};

class cLevel {
	friend cLevelManager;
public:
	cLevel(std::string aLevelName = "", cLevelTheme *aLevelTheme = 0);
	virtual ~cLevel();

	bool 	LoadLevel();
	bool	UnloadLevel();

	int 	getLevelId ();
	void 	setLevelId (int aLevelId); // You can manual set Id for this level. It's not recommended.

	void 	setLevelTheme (cLevelTheme *aLevelTheme);

private:
	int 			mLevelId;

protected:
	std::string		mLevelName;
	cLevelTheme 	*mLevelTheme;

	virtual int Init() = 0;

	virtual int CleanUp() = 0;

	// Copied from game.cpp
	// Определяем объекты
	virtual int  CreatingObjects() { return 0; }

	// Загружаем текстуры
	virtual int  LoadTextures() { return 0; }

	// Освобождаем текстуры
	virtual void FreeTextures() { }

	// Освобождаем объекты
	virtual void FreeObjects() { }

	// Обновляем координаты текстур и объектов
	virtual void Update() { }

	// Обновление данных объекта в конце выполнения общего update
	virtual void PostUpdate() { }

	// Отрисовываем текстуры и объекты
	virtual void Render() { }

	virtual void PostRender() { }

};

#endif /* LEVEL_H_ */
