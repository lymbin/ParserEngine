/*
 * Level.h
 *
 *  Created on: 18 окт. 2016 г.
 *      Author: dmitry
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <map>

class Level;

typedef std::map<int, Level *> tLevels;

class LevelManager {
	LevelManager() { mLevels.clear(); }
	~LevelManager() { } // TODO: deallocate all Levels??

	tLevels 				mLevels;

public:
	bool LoadLevel(int aLevelId = 0);
	int AddLevel(Level *aLevel);

	static LevelManager *Instance()
	{
		if (!self)
			self = new LevelManager();

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
	static LevelManager 	*self;
};

class LevelTheme {
public:
	LevelTheme();
	~LevelTheme();

	// Music and sprite sets holds here
};

class Level {
public:
	Level(std::string aLevelName = "", LevelTheme *aLevelTheme = 0);
	virtual ~Level();

	bool LoadLevel();

	int getLevelId ();
	void setLevelId (int aLevelId); // You can manual set Id for this level. It's not recommended.

	void setLevelTheme (LevelTheme *aLevelTheme);

private:
	int 			mLevelId;

protected:
	std::string		mLevelName;
	LevelTheme 		*mLevelTheme;

	virtual void Init() { }
	// Copied from game.cpp
	// Определяем объекты
	virtual int CreatingObjects() { return 0; }

	// Загружаем текстуры
	virtual int LoadTextures() { return 0; }

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

};



#endif /* LEVEL_H_ */
