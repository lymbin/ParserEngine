/*
 * Sound.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>

//#include "smpeg.h" - поддержка mp3 - не активна
#include "SDL/SDL_mixer.h"
#include "SystemConstants.h"

class cSound;
class cMusic;
class cAudio;

class cSound
{
	friend cAudio;

	Mix_Chunk *Sound;

	std::string fileName;
	cAudio *mpAudio;

public:
	cSound(std::string file = "");
	~cSound();

	int Open(std::string file);

	void Play(unsigned int Repeats = 0);
	void SetAudio(cAudio *apAudio);
};
class cMusic
{
	friend cAudio;

	Mix_Music *Music;

	std::string fileName;
	cAudio *mpAudio;

	bool mbIsPaused;

	void Stop();

public:
	cMusic(std::string file = "");
	~cMusic();

	int Open(std::string file);

	void Play(unsigned int Repeats = 0);
	void Pause();
	void Resume();
	bool IsPaused();

	void SetAudio(cAudio *apAudio);
};

typedef std::list<cSound *> tSoundList;
typedef std::list<cSound *>::iterator tSoundListIt;

typedef std::list<cMusic *> tMusicList;
typedef std::list<cMusic *>::iterator tMusicListIt;

class cAudio
{
	// Менеджер звуков и музыки
	unsigned int SoundVolume;
	unsigned int MusicVolume;

	tSoundList mSounds;
	tMusicList mMusic;

	cMusic *CurrentMusic;

public:
	cAudio();
	~cAudio();

	// Инициализация аудио системы
	int init();

	// Устанавливаем громкость звуков(в первый попавшийся канал) - TODO: Доделать это
	// и громкость музыки
	void SetSoundVolume(unsigned int Volume);
	void SetMusicVolume(unsigned int Volume);

	// Получаем громкость
	unsigned int GetSoundVolume();
	unsigned int GetMusicVolume();

	// Удаляем всю музыку из менеджера
	void DeleteAllMusic();

	// Останавливаем музыку
	void StopMusic();

	// Проигрывание музыки в лупе - тест
	void PlayLoopMusic();

	// Паузим музыку
	void PauseMusic();

	// Возобновляем проигрывание музыки
	void ResumeMusic();

	// Удаляем все звуки из менеджера
	void DeleteAllSounds();

	// Функции управления памятью звуков
	void ManageSound(cSound *apSound);
	void UnManageSound(cSound *apSound);

	// Функции управления памятью музыки
	void ManageMusic(cMusic *apMusic);
	void UnManageMusic(cMusic *apMusic);

	cMusic *GetCurrentMusic();
	void SetCurrentMusic(cMusic *apMusic);
};


#endif /* SOUND_H_ */
