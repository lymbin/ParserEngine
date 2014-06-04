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

#include "SDL/SDL_mixer.h"
#include "SystemConstants.h"

class sound;
class audio;
class music;

class sound
{
	friend audio;
	Mix_Chunk *Sound;
	std::string fileName;
public:
	sound(std::string file = "");
	~sound();

	int Open(std::string file);

	void Delete();

	void Play(unsigned int Repeats = 0);
};
class music
{
	friend audio;
	Mix_Music *Music;
	std::string fileName;

	void Stop();

	void Pause();

	void Resume();
public:
	music(std::string file = "");
	~music();

	int Open(std::string file);

	void Delete();

	void Play(unsigned int Repeats = 0);
};
class audio
{
	// Менеджер звуков и музыки
	unsigned int SoundVolume;
	unsigned int MusicVolume;

	std::vector< sound *> Sounds;
	std::vector< music *> Music;

	music *CurrentMusic;

public:
	audio();
	~audio();

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
	void ManageSound(sound *managed_sound);
	void UnManageSound(sound *managed_sound);

	// Функции управления памятью музыки
	void ManageMusic(music *managed_music);
	void UnManageMusic(music *managed_music);
};


#endif /* SOUND_H_ */
