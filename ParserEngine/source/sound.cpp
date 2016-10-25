/*
 * sound.cpp - music and sound system
 *
 *  Created on: 17.04.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "Sound.h"
#include "CommonStringFunctions.h"

using namespace std;

cAudio *cAudio::self = 0;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cSound::cSound(string file)
{
	Sound = 0;
	mFileName = file;

	if(file != "")
	{
		Open(file);
	}
}

//-----------------------------------------------------------------------

cSound::~cSound()
{
	if (Sound)
	{
		Mix_FreeChunk(Sound);
		Sound = 0;
	}
}

//-----------------------------------------------------------------------

cMusic::cMusic(cPlaylist *aParent, std::string file)
{
	mMusic = 0;
	mFileName = file;
	mParent = aParent;

	if (file != "")
	{
		Open(file);
		mName = FindLastNCopyAfter(file, "/");
		FindNReplace(mName, "/", "");
	}
}

//-----------------------------------------------------------------------

cMusic::~cMusic()
{
	if (mMusic)
	{
		Stop();
		Mix_FreeMusic(mMusic);
		mMusic = 0;
	}
}
//-----------------------------------------------------------------------

cAudio::cAudio()
{
	mCurrentPlaylist 	= 0;
	mSoundVolume		= mMusicVolume = SYS_AUDIO_VOLUME;
	mIsPaused			= false;
	mMusic.clear();
	mSounds.clear();
}

//-----------------------------------------------------------------------

cAudio::~cAudio()
{
	DeleteAllPlaylists();
	DeleteAllSounds();
	DeleteAllMusic();

	Mix_CloseAudio();
	Mix_Quit();

#ifdef DEBUG_SYS
	cout << "Audio clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

cPlaylist::cPlaylist()
{
	mCurrentMusic = -1;
}

//-----------------------------------------------------------------------

cPlaylist::~cPlaylist()
{

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

// Инициализация аудио системы
int cAudio::Init()
{
	// Инициализация плагинов для OGG и MP3 и других
	if(!Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG))
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to init some audio plugins" << endl;
#endif
		return -1;
	}

	// Открываем Mix Audio
	if(Mix_OpenAudio(SYS_AUDIO_RATE, SYS_AUDIO_FORMAT, SYS_AUDIO_CHANNELS, SYS_AUDIO_BUFFERS))
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to load audio system" << endl;
#endif
		return -1;
	}

	SetMusicVolume(SYS_AUDIO_VOLUME);
	SetSoundVolume(SYS_AUDIO_VOLUME);

#ifdef DEBUG_SYS
	cout << "Audio initialization - success" << endl;
#endif
	return 0;
}

//-----------------------------------------------------------------------

// Устанавливаем громкость звуков(в первый попавшийся канал) - TODO: Доделать это
void cAudio::SetSoundVolume(unsigned int Volume)
{
	if(Volume > 128)
		Volume = 128;

	Mix_Volume(-1, Volume);
	mSoundVolume = Volume;
}

//-----------------------------------------------------------------------

// Устанавливаем громкость музыки
void cAudio::SetMusicVolume(unsigned int Volume)
{
	if(Volume > 128)
		Volume = 128;

	Mix_VolumeMusic(Volume);
	mMusicVolume = Volume;
}

//-----------------------------------------------------------------------

// Получаем громкость звуков
unsigned int cAudio::GetSoundVolume()
{
	return mSoundVolume;
}

//-----------------------------------------------------------------------

// Получаем громкость музыки
unsigned int cAudio::GetMusicVolume()
{
	return mMusicVolume;
}

//-----------------------------------------------------------------------

// Конструкторы списков воспроизведения, музыки и звуков.
cPlaylist *cAudio::CreatePlaylist()
{
	cPlaylist *aPlaylist = new cPlaylist();
	mPlaylists.push_back(aPlaylist);
	return aPlaylist;
}

//-----------------------------------------------------------------------

// Создаём плейлист и добавляем в него музыку.
cMusic *cAudio::CreateMusic(std::string file)
{
	cPlaylist *aPlaylist = new cPlaylist();
	mPlaylists.push_back(aPlaylist);
	cMusic *aMusic = aPlaylist->AddMusic(file);
	return aMusic;
}

//-----------------------------------------------------------------------

cSound *cAudio::CreateSound(std::string file)
{
	cSound *aSound = new cSound(file);
	mSounds.push_back(aSound);
	return aSound;
}

//-----------------------------------------------------------------------

// Деструкторы списков воспроизведения, музыки и звуков.
void cAudio::RemovePlaylist (cPlaylist *aPlaylist)
{

}

//-----------------------------------------------------------------------

void cAudio::RemoveMusic (cMusic *aMusic)
{

}

//-----------------------------------------------------------------------

void cAudio::RemoveSound (cSound *aSound)
{

}

//-----------------------------------------------------------------------

// Удаляем все списки воспроизведения
void cAudio::DeleteAllPlaylists()
{
	StopMusic();

	tPlaylistListIt It = mPlaylists.begin();
	for( ; It != mPlaylists.end(); ++It)
	{
		delete (*It);
	}

	mPlaylists.clear();
	mCurrentPlaylist = 0;
}

//-----------------------------------------------------------------------

// Удаляем всю музыку из менеджера
void cAudio::DeleteAllMusic()
{
	StopMusic();

	tMusicListIt It = mMusic.begin();
	for( ; It != mMusic.end(); ++It)
	{
		delete (*It);
	}

	mMusic.clear();
}

//-----------------------------------------------------------------------

// Останавливаем музыку
void cAudio::StopMusic()
{
	if(Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

//-----------------------------------------------------------------------

void cAudio::PlayMusic()
{
	if (mCurrentPlaylist)
		mCurrentPlaylist->Play();
}

//-----------------------------------------------------------------------

// Паузим музыку
void cAudio::PauseMusic()
{
	if(Mix_PlayingMusic())
	{
		if(!Mix_PausedMusic())
		{
			mIsPaused = true;
			Mix_PauseMusic();
		}
	}
}

//-----------------------------------------------------------------------

// Возобновляем проигрывание музыки
void cAudio::ResumeMusic()
{
	if(Mix_PlayingMusic())
	{
		if(Mix_PausedMusic())
		{
			mIsPaused = false;
			Mix_ResumeMusic();
		}
	}
}

//-----------------------------------------------------------------------

void cAudio::NextMusic()
{
	if (mCurrentPlaylist)
		return mCurrentPlaylist->Next();
}

//-----------------------------------------------------------------------

void cAudio::PrevMusic()
{
	if (mCurrentPlaylist)
		return mCurrentPlaylist->Prev();
}

//-----------------------------------------------------------------------

// Удаляем все звуки из менеджера
void cAudio::DeleteAllSounds()
{
	tSoundListIt It = mSounds.begin();
	for( ; It != mSounds.end(); ++It)
	{
		delete (*It);
	}

	mSounds.clear();
}

//-----------------------------------------------------------------------

cPlaylist *cAudio::GetCurrentPlaylist()
{
	return mCurrentPlaylist;
}

//-----------------------------------------------------------------------

cMusic *cAudio::GetCurrentMusic()
{
	if (mCurrentPlaylist)
		return mCurrentPlaylist->GetCurrentMusic();
	else
		return 0;
}

//-----------------------------------------------------------------------

void cAudio::SetCurrentMusic(cMusic *apMusic)
{
	mCurrentPlaylist->SetCurrentMusic(apMusic);
}

//-----------------------------------------------------------------------

int cSound::Open(std::string file)
{
	Sound = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(), "rb"), 1);
	if(!Sound)
	{
#ifdef DEBUG_ERRORS
		cout << "Open sound - failure : " << file << endl;
#endif
		return -1;
	}

	mFileName = file;

	//TODO: добавление в менеджер

	return 0;
}

//-----------------------------------------------------------------------

void cSound::Play(unsigned int Repeats)
{
	if(Sound)
	{
		if(Mix_PlayChannel(-1, Sound, Repeats) == -1)
		{
#ifdef DEBUG_ERRORS
			cout << "Failed to play sound : " << mFileName << " " << Mix_GetError() << endl;
#endif
		}
	}
}

//-----------------------------------------------------------------------

int cMusic::Open(std::string file)
{
	mMusic = Mix_LoadMUS(file.c_str());

	if (!mMusic)
	{
#ifdef DEBUG_ERRORS
		cout << "Open music - failure : " << file << endl;
#endif
		return -1;	//TODO: перенести в мэйн Audio, т.к. останавливает текущую проигрываемую музыку, а не эту
		if (Mix_PlayingMusic())
			Mix_HaltMusic();
	}

	mFileName = file;

	return 0;
}

//-----------------------------------------------------------------------

cPlaylist *cMusic::Playlist()
{
	return mParent;
}

//-----------------------------------------------------------------------

void cMusic::Play(unsigned int Repeats)
{
	if (mMusic)
	{
		if (Mix_PlayingMusic())
		{
			Stop();
		}
		if (Mix_PlayMusic(mMusic, Repeats))
		{
			std::cout << "Unable to play " << mFileName << std::endl;
		}
	}
}

//-----------------------------------------------------------------------

void cMusic::Pause()
{
	cAudio::Instance()->PauseMusic();
}

//-----------------------------------------------------------------------

void cMusic::Resume()
{
	cAudio::Instance()->ResumeMusic();
}

//-----------------------------------------------------------------------

std::string cMusic::GetName()
{
	return mName;
}

//-----------------------------------------------------------------------

void cPlaylist::Play(bool aPlayBack)
{
	if (!mMusic.empty()) {
		if (!aPlayBack)
			Next();
		else
			Prev();
	}
}
//-----------------------------------------------------------------------

void cPlaylist::Play(int aMusicId)
{
	mMusic[aMusicId]->Play();
}

//-----------------------------------------------------------------------

void cPlaylist::Pause()
{
	cAudio::Instance()->PauseMusic();
}

//-----------------------------------------------------------------------

void cPlaylist::Resume()
{
	cAudio::Instance()->ResumeMusic();
}

//-----------------------------------------------------------------------

void cPlaylist::Next()
{
	if (!mMusic.empty())
	{
		Stop();
		tMusicMapIt It = mMusic.end();
		--It;
		if (mCurrentMusic == It->first)
		{
			mCurrentMusic = mMusic.begin()->first;
			Play(mCurrentMusic);
		}
		else
		{
			mCurrentMusic += 1;
			Play(mCurrentMusic);
		}
	}
}

//-----------------------------------------------------------------------

void cPlaylist::Prev()
{
	if (!mMusic.empty())
	{
		Stop();
		if (mCurrentMusic == mMusic.begin()->first)
		{
			tMusicMapIt It = mMusic.end();
			--It;
			mCurrentMusic = It->first;
			Play(mCurrentMusic);
		}
		else
		{
			mCurrentMusic -= 1;
			Play(mCurrentMusic);
		}
	}
}

//-----------------------------------------------------------------------

bool cAudio::IsPaused()
{
	return mIsPaused;
}

//-----------------------------------------------------------------------

cMusic *cPlaylist::AddMusic(std::string aFile)
{
	cMusic *aMusic = new cMusic(this, aFile);
	AddMusic(aMusic);
	return aMusic;
}

//-----------------------------------------------------------------------

void cPlaylist::AddMusic(cMusic *aMusic, int aId)
{
	if (!aMusic)
		return;

	int newId = aId;

	if (!mMusic.empty())
	{
		if (newId < 0)
		{
			tMusicMapIt It = mMusic.end();
			It--;
			newId = It->first + 1;
		}
		else if (mMusic.find(aId)->second)
		{
			if (mCurrentMusic == aId)
			{
				if (mMusic.size() > 1)
					Next();
				else
					Stop();
			}

		}
	}
	else if (newId < 0)
	{
		newId = 0;
	}

#ifdef DEBUG_ERRORS
		cout << "Music id " << newId << endl;
#endif

	mMusic.insert(std::pair<int, cMusic *>(newId, aMusic));
}

//-----------------------------------------------------------------------

void cPlaylist::RemoveMusic(cMusic *aMusic)
{
	if (!aMusic)
			return;

	if (!mMusic.empty())
	{
		tMusicMapIt It = mMusic.begin();
		for (; It != mMusic.end(); ++It)
		{
			if (It->second == aMusic)
			{
				if (mCurrentMusic == It->first)
				{
					if (mMusic.size() > 1)
						Next();
					else
						Stop();
				}
				delete It->second;
				mMusic.erase(It);
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------

void cPlaylist::RemoveMusic(int aId)
{
	if (!mMusic.empty())
	{
		tMusicMapIt It = mMusic.find(aId);
		if (It->second)
		{
			if (mCurrentMusic == aId)
			{
				if (mMusic.size() > 1)
					Next();
				else
					Stop();
			}
			delete It->second;
			mMusic.erase(It);
		}
	}
}

//-----------------------------------------------------------------------

cMusic *cPlaylist::GetCurrentMusic()
{
	if (!mMusic.empty())
		return mMusic[mCurrentMusic];
	return 0;
}

//-----------------------------------------------------------------------

void cPlaylist::SetCurrentMusic(cMusic *aMusic)
{
	// TODO: Найти способ лучшего сравнения музыки
}

//-----------------------------------------------------------------------

void cPlaylist::SetCurrentMusic(int aId)
{
	Stop();
	mCurrentMusic = aId;
	Play();
}

//-----------------------------------------------------------------------

int cPlaylist::GetMusicId(cMusic *aMusic)
{
	// TODO: Найти способ лучшего сравнения музыки
	return 0;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cMusic::Stop()
{
	cAudio::Instance()->StopMusic();
}

//-----------------------------------------------------------------------

void cPlaylist::Stop()
{
	cAudio::Instance()->StopMusic();
}

//-----------------------------------------------------------------------
