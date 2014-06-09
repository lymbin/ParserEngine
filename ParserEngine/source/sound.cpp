/*
 * sound.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "Sound.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cSound::cSound(string file)
{
	Sound = 0;
	fileName = file;
	mpAudio = 0;

	if(file != "")
	{
		Open(file);
	}
}

//-----------------------------------------------------------------------

cSound::~cSound()
{
	if(mpAudio)
	{
		mpAudio->UnManageSound(this);
		mpAudio = 0;
	}

	if(Sound)
	{
		Mix_FreeChunk(Sound);
		Sound = 0;
	}
}

//-----------------------------------------------------------------------

cMusic::cMusic(std::string file)
{
	Music = 0;
	fileName = file;
	mpAudio = 0;
	mbIsPaused = false;

	if(file != "")
	{
		Open(file);
	}
}

//-----------------------------------------------------------------------

cMusic::~cMusic()
{
	if(mpAudio)
	{
		mpAudio->UnManageMusic(this);
		mpAudio = 0;
	}

	if(Music)
	{
		Stop();
		Mix_FreeMusic(Music);
		Music = 0;
	}
}
//-----------------------------------------------------------------------

cAudio::cAudio()
{
	CurrentMusic = 0;
	SoundVolume	= MusicVolume = SYS_AUDIO_VOLUME;
	mMusic.clear();
	mSounds.clear();
}

//-----------------------------------------------------------------------

cAudio::~cAudio()
{
	DeleteAllSounds();
	DeleteAllMusic();

	Mix_CloseAudio();
	Mix_Quit();

#ifdef DEBUG_SYS
	cout << "Audio clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

// Инициализация аудио системы
int cAudio::init()
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
	SoundVolume = Volume;
}

//-----------------------------------------------------------------------

// Устанавливаем громкость музыки
void cAudio::SetMusicVolume(unsigned int Volume)
{
	if(Volume > 128)
		Volume = 128;

	Mix_VolumeMusic(Volume);
	MusicVolume = Volume;
}

//-----------------------------------------------------------------------

// Получаем громкость звуков
unsigned int cAudio::GetSoundVolume()
{
	return SoundVolume;
}

//-----------------------------------------------------------------------

// Получаем громкость музыки
unsigned int cAudio::GetMusicVolume()
{
	return MusicVolume;
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
	CurrentMusic = 0;
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

void cAudio::PlayLoopMusic()
{
	// Проигрывание музыки в определённом порядке
	// TODO: подумать над этой функцией
}

//-----------------------------------------------------------------------

// Паузим музыку
void cAudio::PauseMusic()
{
	if(Mix_PlayingMusic())
	{
		if(!Mix_PausedMusic())
		{
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
			Mix_ResumeMusic();
		}
	}
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

//Добавляем звук в вектор для управления
void cAudio::ManageSound(cSound *apSound)
{
	if(!apSound)
		return;

	tSoundListIt It = mSounds.begin();
	for( ; It != mSounds.end(); ++It)
	{
		if((*It) == apSound)
		{
			return;
		}
	}

	mSounds.push_back(apSound);
}

//-----------------------------------------------------------------------

// Удаляем звук из вектора управления
void cAudio::UnManageSound(cSound *apSound)
{
	// Внимание: Это только удалит звук из вектора управления, но не удалит сам звук
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage, но можно и не удалять объект
	// 	менеджер сам всё удалит при выходе из программы/удалении менеджера

	if(!apSound || mSounds.empty())
		return;

	tSoundListIt It = mSounds.begin();
	for( ; It != mSounds.end(); ++It)
	{
		if((*It) == apSound)
		{
			mSounds.erase(It);
			return;
		}
	}
}

//-----------------------------------------------------------------------

// Функции управления памятью музыки
void cAudio::ManageMusic(cMusic *apMusic)
{
	//Добавляем звук в вектор для управления
	if(!apMusic)
		return;

	tMusicListIt It = mMusic.begin();
	for( ; It != mMusic.end(); ++It)
	{
		if((*It) == apMusic)
		{
			return;
		}
	}

	mMusic.push_back(apMusic);

}

//-----------------------------------------------------------------------

void cAudio::UnManageMusic(cMusic *apMusic)
{
	// Удаляем музыку из вектора управления
	// Внимание: Это только удалит музыку из вектора управления, но не удалит саму музыку
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage, но можно и не удалять объект
	// 	менеджер сам всё удалит при выходе из программы/удалении менеджера

	if(!apMusic || mMusic.empty())
		return;

	tMusicListIt It = mMusic.begin();
	for( ; It != mMusic.end(); ++It)
	{
		if((*It) == apMusic)
		{
			if(CurrentMusic == apMusic)
				CurrentMusic = 0;
			mMusic.erase(It);
			return;
		}
	}
}

//-----------------------------------------------------------------------

cMusic *cAudio::GetCurrentMusic()
{
	return CurrentMusic;
}

//-----------------------------------------------------------------------

void cAudio::SetCurrentMusic(cMusic *apMusic)
{
	CurrentMusic = apMusic;
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

	fileName = file;

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
			cout << "Failed to play sound : " << fileName << " " << Mix_GetError() << endl;
#endif
		}
	}
}

//-----------------------------------------------------------------------

void cSound::SetAudio(cAudio *apAudio)
{
	mpAudio = apAudio;
}

//-----------------------------------------------------------------------

int cMusic::Open(std::string file)
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

//-----------------------------------------------------------------------

void cMusic::Play(unsigned int Repeats)
{
	if(Music)
	{
		if(Mix_PlayingMusic())
		{
			Stop();
		}
		if(Mix_PlayMusic(Music, Repeats))
		{
			std::cout << "Unable to play " << fileName << std::endl;
		}
		if(mpAudio && (mpAudio->GetCurrentMusic()!=this))
		{
			mpAudio->SetCurrentMusic(this);
		}
	}
}

//-----------------------------------------------------------------------

void cMusic::SetAudio(cAudio *apAudio)
{
	mpAudio = apAudio;

	if(mpAudio)
		mpAudio->ManageMusic(this);


}

//-----------------------------------------------------------------------

void cMusic::Pause()
{
	//TODO: перенести в мэйн Audio, т.к. паузит текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		 if(!Mix_PausedMusic())
		 {
			 Mix_PauseMusic();
			 mbIsPaused = true;
		 }
	}
}

//-----------------------------------------------------------------------

void cMusic::Resume()
{
	//TODO: перенести в мэйн Audio, т.к. возобновляет текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		 if(Mix_PausedMusic())
		 {
			 Mix_ResumeMusic();
			 mbIsPaused = false;
		 }
	}
}

//-----------------------------------------------------------------------

bool cMusic::IsPaused()
{
	return mbIsPaused;
}
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cMusic::Stop()
{
	//TODO: перенести в мэйн Audio, т.к. останавливает текущую проигрываемую музыку, а не эту
	if(Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

//-----------------------------------------------------------------------
