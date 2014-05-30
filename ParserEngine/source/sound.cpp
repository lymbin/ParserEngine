/*
 * sound.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "Sound.h"

using namespace std;

sound::sound(string file)
{
	Sound = 0;
	fileName = file;

	if(file != "")
	{
		Open(file);
	}
}
sound::~sound()
{
	Delete();
}
int sound::Open(std::string file)
{
	Sound = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(), "rb"), 1);
	if(!Sound)
	{
#ifdef DEBUG_ERRORS
		cout << "Open sound - failure : " << file << endl;
#endif
		return -1;
	}

	fileName = file;

	//TODO: добавление в менеджер

	return 0;
}

void sound::Delete()
{
	if(Sound)
		Mix_FreeChunk(Sound);
}

void sound::Play(unsigned int Repeats)
{
	if(Sound)
	{
		if(Mix_PlayChannel(-1, Sound, Repeats) == -1)
		{
#ifdef DEBUG_ERRORS
			cout << "Failed to play sound : " << fileName << " " << Mix_GetError() << endl;
#endif
		}
	}
}

music::music(std::string file)
{
	Music = 0;
	fileName = file;

	if(file != "")
	{
		Open(file);
	}
}
music::~music()
{
	Delete();
}

void music::Stop()
{
	//TODO: перенести в мэйн Audio, т.к. останавливает текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

void music::Pause()
{
	//TODO: перенести в мэйн Audio, т.к. паузит текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		 if(!Mix_PausedMusic())
		 {
			 Mix_PauseMusic();
		 }
	}
}
void music::Resume()
{
	//TODO: перенести в мэйн Audio, т.к. возобновляет текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		 if(Mix_PausedMusic())
		 {
			 Mix_ResumeMusic();
		 }
	}
}
int music::Open(std::string file)
{
	Music = Mix_LoadMUS(file.c_str());

	if(!Music)
	{
#ifdef DEBUG_ERRORS
		cout << "Open music - failure : " << file << endl;
#endif
		return -1;
	}

	fileName = file;

	//TODO: добавление в менеджер

	return 0;
}
void music::Delete()
{
	if(Music)
	{
		Stop();
		Mix_FreeMusic(Music);
	}
}
void music::Play(unsigned int Repeats)
{
	if(Music)
	{
		if(Mix_PlayingMusic())
		{
			Stop();
		}
		Mix_PlayMusic(Music, Repeats);
	}
}

audio::audio()
{
	CurrentMusic = 0;
	SoundVolume	= MusicVolume = SYS_AUDIO_VOLUME;
	Music.clear();
	Sounds.clear();
}
audio::~audio()
{
	DeleteAllSounds();
	DeleteAllMusic();

	Mix_CloseAudio();
	Mix_Quit();

#ifdef DEBUG_SYS
	cout << "Audio clean up - success" << endl;
#endif
}

// Инициализация аудио системы
int audio::init()
{
	// Открываем Mix Audio
	if(Mix_OpenAudio(SYS_AUDIO_RATE, SYS_AUDIO_FORMAT, SYS_AUDIO_CHANNELS, SYS_AUDIO_BUFFERS))
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to load audio system" << endl;
#endif
		return -1;
	}

	// Инициализация плагинов для OGG и MP3 и других
	if(!Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG))
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to init some audio plugins" << endl;
#endif
		return -1;
	}

#ifdef DEBUG_SYS
	cout << "Audio initialization - success" << endl;
#endif
	return 0;
}

// Устанавливаем громкость звуков(в первый попавшийся канал) - TODO: Доделать это
void audio::SetSoundVolume(unsigned int Volume)
{
	if(Volume > 128)
		Volume = 128;

	Mix_Volume(-1, Volume);
	SoundVolume = Volume;
}

// Устанавливаем громкость музыки
void audio::SetMusicVolume(unsigned int Volume)
{
	if(Volume > 128)
		Volume = 128;

	Mix_VolumeMusic(Volume);
	MusicVolume = Volume;
}

// Получаем громкость звуков
unsigned int audio::GetSoundVolume()
{
	return SoundVolume;
}
// Получаем громкость музыки
unsigned int audio::GetMusicVolume()
{
	return MusicVolume;
}

// Удаляем всю музыку из менеджера
void audio::DeleteAllMusic()
{
	StopMusic();

	for(unsigned int loop = 0; loop < Music.size(); ++loop)
	{
		delete Music[loop];
	}

	Music.clear();
}

// Останавливаем музыку
void audio::StopMusic()
{
	if(Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

void audio::PlayLoopMusic()
{
	// Проигрывание музыки в определённом порядке
	// TODO: подумать над этой функцией
}

// Паузим музыку
void audio::PauseMusic()
{
	if(Mix_PlayingMusic())
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}
}

// Возобновляем проигрывание музыки
void audio::ResumeMusic()
{
	if(Mix_PlayingMusic())
	{
		if(Mix_PausedMusic())
		{
			Mix_ResumeMusic();
		}
	}
}

// Удаляем все звуки из менеджера
void audio::DeleteAllSounds()
{
	for(unsigned int loop = 0; loop < Sounds.size(); ++loop)
	{
		delete Sounds[loop];
	}

	Sounds.clear();
}

// Функции управления памятью звуков
void audio::ManageSound(sound *managed_sound)
{
	//Добавляем звук в вектор для управления
	for(unsigned int loop = 0; loop < Sounds.size(); loop++)
	{
		if(Sounds[loop]->Sound == managed_sound->Sound)
		{
			return;
		}
	}
	/*if(!managed_sound->audio)
		managed_sound->SetAudio(this);
		*/

	Sounds.push_back(managed_sound);
}
void audio::UnManageSound(sound *managed_sound)
{
	// Удаляем звук из вектора управления
	// Внимание: Это только удалит звук из вектора управления, но не удалит сам звук
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage
	// TODO: пересмотреть идею менеджеров памяти и сделать их и их компоненты связанными - так что при удалении из вектора удалялся и сам компонент

	int place = -1;

	// Ищем звук в векторе
	for(unsigned int loop = 0; loop < Sounds.size(); loop++)
	{
		if(Sounds[loop]->Sound == managed_sound->Sound)
		{
			place = loop;
			break;
		}
	}

	// звук не найден - выходим
	if(place < 0)
		return;

	if((unsigned int)(place+1) == Sounds.size())
	{
		// Звук в самом конце - удаляем, перед этим обснулив указатель на менеджер
		//Sounds[place]->AudoManager = 0;
		Sounds.pop_back();
	}
	else
	{
		// Звук где-то внутри вектора - удаляем, перед этим обнулив указатель на менеджер
		//TODO: проверить
		//Sounds[place] = Sounds[ Sounds.size() - 1 ];
		//Sounds[place]->AudoManager = 0;
		Sounds.erase( Sounds.begin() + place);
	}
}

// Функции управления памятью музыки
void audio::ManageMusic(music *managed_music)
{
	//Добавляем звук в вектор для управления
	for(unsigned int loop = 0; loop < Music.size(); loop++)
	{
		if(Music[loop]->Music == managed_music->Music)
		{
			return;
		}
	}
	/*if(!managed_sound->audio)
		managed_sound->SetAudio(this);
		*/

	Music.push_back(managed_music);
}
void audio::UnManageMusic(music *managed_music)
{
	// Удаляем музыку из вектора управления
	// Внимание: Это только удалит музыку из вектора управления, но не удалит саму музыку
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage
	// TODO: пересмотреть идею менеджеров памяти и сделать их и их компоненты связанными - так что при удалении из вектора удалялся и сам компонент

	int place = -1;

	// Ищем музыку в векторе
	for(unsigned int loop = 0; loop < Music.size(); loop++)
	{
		if(Music[loop]->Music == managed_music->Music)
		{
			place = loop;
			break;
		}
	}

	// Музыка не найдена - выходим
	if(place < 0)
		return;

	if((unsigned int)(place+1) == Music.size())
	{
		// Музыка в самом конце - удаляем, перед этим обснулив указатель на менеджер
		//Music[place]->AudoManager = 0;
		Music.pop_back();
	}
	else
	{
		// Музыка где-то внутри вектора - удаляем, перед этим обнулив указатель на менеджер
		//TODO: проверить
		//Music[place] = Music[ Music.size() - 1 ];
		//Music[place]->AudoManager = 0;
		Music.erase( Music.begin() + place);
	}
}

