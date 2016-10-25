/*
 * Level.cpp
 *
 *  Created on: 18.10.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "Level.h"

cLevelManager *cLevelManager::self = 0;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cLevelTheme::cLevelTheme(std::string aDataFolder) :
		mDataFolder(aDataFolder)
{
	mPlaylist = 0;
}

//-----------------------------------------------------------------------

cLevelTheme::~cLevelTheme()
{

}

//-----------------------------------------------------------------------

cLevelManager::cLevelManager()
{
	mCurrentLevel = -1;
	mLevels.clear();
}

//-----------------------------------------------------------------------

cLevelManager::~cLevelManager()
{

}

//-----------------------------------------------------------------------

cLevel::cLevel(std::string aLevelName, cLevelTheme *aLevelTheme) :
		mLevelName(aLevelName)
{
	mLevelId = cLevelManager::Instance()->AddLevel(this);
	mLevelTheme = aLevelTheme;
}

//-----------------------------------------------------------------------

cLevel::~cLevel()
{

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool cLevelManager::Start()
{
	return LoadLevel();
}

//-----------------------------------------------------------------------

bool cLevelManager::LoadLevel(int aLevelId)
{
	if (!mLevels.empty())
	{
		tLevelsIt It = mLevels.begin();
		for (; It != mLevels.end(); ++It)
		{
			if (It->first == aLevelId)
			{
				if (UnloadLevel(mCurrentLevel))
				{
					if (It->second->LoadLevel())
					{
						mCurrentLevel = It->first;
						return true;
					}
					else
					{
						LoadLevel(mCurrentLevel);
						return false;
					}
				}
				else
					return false;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------

bool cLevelManager::UnloadLevel(int aLevelId)
{
	if (!mLevels.empty())
	{
		if (aLevelId < 0)
		{
			return mLevels[mCurrentLevel]->UnloadLevel();
		}

		tLevelsIt It = mLevels.begin();
		for (; It != mLevels.end(); ++It)
		{
			if (It->first == aLevelId)
			{
				return It->second->UnloadLevel();
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------

int cLevelManager::AddLevel(cLevel *aLevel)
{
	int levelId = 0;
	if (mLevels.empty())
	{
		mLevels[levelId] = aLevel;
		mCurrentLevel = levelId;
	}
	else
	{
		tLevelsIt It = mLevels.end();
		--It;
		levelId = It->first + 1;
		mLevels.insert(std::pair<int, cLevel *>(levelId, aLevel));
	}
	return levelId;
}

//-----------------------------------------------------------------------

void cLevelManager::RemoveLevel(int aLevelId)
{
	if (aLevelId == mCurrentLevel)
		return;

	if (!mLevels.empty())
	{
		tLevelsIt It = mLevels.begin();
		for (; It != mLevels.end(); ++It)
		{
			if (It->first == aLevelId)
			{
				mLevels.erase(It);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------

int cLevelManager::NextLevel()
{
	if (!mLevels.empty())
	{
		tLevelsIt It = mLevels.end();
		--It;
		if (mCurrentLevel == It->first)
		{
			if (UnloadLevel(mCurrentLevel))
			{
				It = mLevels.begin();
				if (It->second->LoadLevel())
				{
					mCurrentLevel = It->first;
				}
				else
				{
					LoadLevel(mCurrentLevel);
				}
			}
		}
		else
		{
			if (UnloadLevel(mCurrentLevel))
			{
				int aNextLevelId = mCurrentLevel + 1;
				mCurrentLevel = aNextLevelId;

				if (mLevels[aNextLevelId]->LoadLevel())
				{
					mCurrentLevel = aNextLevelId;
				}
				else
				{
					LoadLevel(mCurrentLevel);
				}
			}
		}
	}
	return mCurrentLevel;
}

//-----------------------------------------------------------------------

int cLevelManager::PreviousLevel()
{
	if (!mLevels.empty())
	{
		if (mCurrentLevel == mLevels.begin()->first)
		{
			if (UnloadLevel(mCurrentLevel))
			{
				tLevelsIt It = mLevels.end();
				--It;
				if (LoadLevel(It->first))
				{
					mCurrentLevel = It->first;
				}
				else
				{
					LoadLevel(mCurrentLevel);
				}
			}
		}
		else
		{
			if (UnloadLevel(mCurrentLevel))
			{
				if (LoadLevel(mCurrentLevel-1))
				{
					mCurrentLevel = mCurrentLevel-1;
				}
				else
				{
					LoadLevel(mCurrentLevel);
				}
			}
		}
	}
	return mCurrentLevel;
}

//-----------------------------------------------------------------------

int cLevelManager::GetCurrentLevel()
{
	return mCurrentLevel;
}

//-----------------------------------------------------------------------

cLevel *cLevelManager::GetLevel(int aLevelId)
{
	cLevel *aLevel = 0;
	if (!mLevels.empty())
	{
		tLevelsIt It = mLevels.begin();
		for (; It != mLevels.end(); ++It)
		{
			if (It->first == aLevelId)
			{
				aLevel = It->second;
				break;
			}
		}
	}
	return aLevel;
}

//-----------------------------------------------------------------------

int cLevelManager::Init()
{
	if (mCurrentLevel >= 0)
	{
		mLevels[mCurrentLevel]->Init();
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------

int cLevelManager::CleanUp()
{
	if (mCurrentLevel >= 0)
	{
		mLevels[mCurrentLevel]->CleanUp();
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------

int cLevelManager::CreatingObjects()
{
	if (mCurrentLevel >= 0)
	{
		mLevels[mCurrentLevel]->CreatingObjects();
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------

int cLevelManager::LoadTextures()
{
	if (mCurrentLevel >= 0)
	{
		mLevels[mCurrentLevel]->LoadTextures();
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------

void cLevelManager::FreeTextures()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->FreeTextures();
}

//-----------------------------------------------------------------------

void cLevelManager::FreeObjects()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->FreeObjects();
}

//-----------------------------------------------------------------------

void cLevelManager::Update()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->Update();
}

//-----------------------------------------------------------------------

void cLevelManager::PostUpdate()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->PostUpdate();
}

//-----------------------------------------------------------------------

void cLevelManager::Render()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->Render();
}

//-----------------------------------------------------------------------

void cLevelManager::PostRender()
{
	if (mCurrentLevel >= 0)
		mLevels[mCurrentLevel]->PostRender();
}

//-----------------------------------------------------------------------

bool cLevel::LoadLevel()
{
	return !Init();
}

//-----------------------------------------------------------------------

bool cLevel::UnloadLevel()
{
	return !CleanUp();
}

//-----------------------------------------------------------------------

int cLevel::getLevelId ()
{
	return mLevelId;
}

//-----------------------------------------------------------------------

void cLevel::setLevelId (int aLevelId)
{
	mLevelId = aLevelId;
}

//-----------------------------------------------------------------------

void cLevel::setLevelTheme (cLevelTheme *aLevelTheme)
{
	// stop render
	mLevelTheme = aLevelTheme;
	// start render
}

//-----------------------------------------------------------------------
