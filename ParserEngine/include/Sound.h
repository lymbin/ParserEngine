/*
 * Sound.h - music and sound system
 *
 *  Created on: 30.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>

//#include "smpeg.h" - поддержка mp3 - не активна
#include "SDL2/SDL_mixer.h"
#include "SystemConstants.h"

class cSound;
class cMusic;
class cAudio;
class cPlaylist;

class cSound
{
	friend cAudio;

	Mix_Chunk *Sound;

	std::string mFileName;

public:
	cSound(std::string file = "");
	~cSound();

	int Open(std::string file);

	void Play(unsigned int Repeats = 0);
};
class cMusic
{
	friend cAudio;
	friend cPlaylist;

	Mix_Music *mMusic;

	std::string mFileName;
	std::string mName;

	cPlaylist *mParent;

	cMusic(cPlaylist *aParent, std::string file = "");
	~cMusic();

	void Stop();
public:
	int Open(std::string file);
	cPlaylist *Playlist();

	void Play(unsigned int Repeats = 0);
	void Pause();
	void Resume();
	std::string GetName();
};

typedef std::list<cSound *> tSoundList;
typedef std::list<cSound *>::iterator tSoundListIt;

typedef std::list<cMusic *> tMusicList;
typedef std::list<cMusic *>::iterator tMusicListIt;

typedef std::map<int, cMusic *> tMusicMap;
typedef std::map<int, cMusic *>::iterator tMusicMapIt;

// Playlist class for playing music in a row. Used by LevelTheme.
class cPlaylist {
	friend cAudio;

	tMusicMap 	mMusic;
	int			mCurrentMusic;

	cPlaylist();
	~cPlaylist();

	void Stop();
public:
	void Play(bool aPlayBack = false);
	void Play(int aMusicId);
	void Pause();
	void Resume();

	void Next();
	void Prev();

	cMusic 	*AddMusic(std::string aFile);
	void 	AddMusic(cMusic *aMusic, int aId = -1);
	void 	RemoveMusic(cMusic *aMusic);
	void 	RemoveMusic(int aId);

	cMusic 	*GetCurrentMusic();
	void 	SetCurrentMusic(cMusic *aMusic);
	void 	SetCurrentMusic(int aId = 0);
	int 	GetMusicId(cMusic *aMusic);
};

typedef std::list<cPlaylist *> tPlaylistList;
typedef std::list<cPlaylist *>::iterator tPlaylistListIt;

class cAudio
{
	// Менеджер звуков и музыки
	unsigned int 	mSoundVolume;
	unsigned int 	mMusicVolume;

	bool 			mIsPaused;

	tSoundList 		mSounds;
	tMusicList 		mMusic;				// store music not in playlists (TODO: remove it cause playlist can hold one music)
	tPlaylistList 	mPlaylists;

	cPlaylist		*mCurrentPlaylist;

	cAudio();
	~cAudio();

protected:
	static cAudio *self;

public:
	static cAudio *Instance()
	{
		if (!self)
			self = new cAudio();

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

	// Инициализация аудио системы.
	int Init();

	// Конструкторы списков воспроизведения, музыки и звуков.
	cPlaylist 	*CreatePlaylist();
	cMusic 		*CreateMusic(std::string file = "");
	cSound 		*CreateSound(std::string file = "");

	// Деструкторы списков воспроизведения, музыки и звуков.
	void RemovePlaylist (cPlaylist *aPlaylist);
	void RemoveMusic (cMusic *aMusic);
	void RemoveSound (cSound *aSound);

	// Удаляем все списки воспроизведения
	void DeleteAllPlaylists();
	// Удаляем всю музыку
	void DeleteAllMusic();
	// Удаляем все звуки
	void DeleteAllSounds();

	// Устанавливаем громкость звуков(в первый попавшийся канал) - TODO: Доделать это
	// и громкость музыки
	void SetSoundVolume(unsigned int Volume);
	void SetMusicVolume(unsigned int Volume);

	// Получаем громкость
	unsigned int GetSoundVolume();
	unsigned int GetMusicVolume();

	// Останавливаем музыку
	void StopMusic();

	void PlayMusic();

	// Паузим музыку
	void PauseMusic();

	// Возобновляем проигрывание музыки
	void ResumeMusic();

	bool IsPaused();

	cMusic *GetCurrentMusic();
	void SetCurrentMusic(cMusic *apMusic);
};


#endif /* SOUND_H_ */
