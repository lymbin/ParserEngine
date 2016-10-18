#include "Level.h"

LevelManager *LevelManager::self = 0;

LevelTheme::LevelTheme()
{

}

LevelTheme::~LevelTheme()
{

}

Level::Level(std::string aLevelName, LevelTheme *aLevelTheme) :
		mLevelName(aLevelName)
{
	mLevelId = LevelManager::Instance()->AddLevel(this);
	mLevelTheme = aLevelTheme;
}

Level::~Level()
{

}

bool LevelManager::LoadLevel(int aLevelId)
{
	// TODO: add search in map
	return true;
}

int LevelManager::AddLevel(Level *aLevel)
{
	int levelId = 0;
	if (mLevels.empty())
	{
		mLevels[levelId] = aLevel;
	}
	else
	{
		levelId = mLevels.end()->first + 1;
		mLevels.insert(std::pair<int, Level *>(levelId, aLevel));
	}
	return levelId;
}

bool Level::LoadLevel()
{
	Init();
	return true;
}
