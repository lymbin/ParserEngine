/*
 * sound.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "Sound.h"

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
	if(Sound)
	{
		Mix_FreeChunk(Sound);
		Sound = 0;
	}
}

//-----------------------------------------------------------------------

cMusic::cMusic(cPlaylist *aParent, std::string file)
{
	Music = 0;
	mFileName = file;
	mParent = aParent;

	if(file != "")
	{
		Open(file);
	}
}

//-----------------------------------------------------------------------

cMusic::~cMusic()
{
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

cMusic *cAudio::CreateMusic(std::string file)
{
	// TODO: вообще-то необходимо создать плейлист с одной записью и вывести указатель на неё.
	cPlaylist *aPlaylist = new cPlaylist();
	mPlaylists.push_back(aPlaylist);
	cMusic *aMusic = new cMusic(aPlaylist, file);
	aPlaylist->AddMusic(aMusic);
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
			mMusic.erase(It);
			return;
		}
	}
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
	Music = Mix_LoadMUS(file.c_str());

	if(!Music)
	{
#ifdef DEBUG_ERRORS
		cout << "Open music - failure : " << file << endl;
#endif
		return -1;	//TODO: перенести в мэйн Audio, т.к. останавливает текущую проигрываемую музыку, а не эту
		if(Mix_PlayingMusic())
		{
			Mix_HaltMusic();
		}
	}

	mFileName = file;

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
			std::cout << "Unable to play " << mFileName << std::endl;
		}
	}
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
		 }
	}
}

//-----------------------------------------------------------------------

void cPlaylist::Play()
{

}

//-----------------------------------------------------------------------

void cPlaylist::Pause()
{

}

//-----------------------------------------------------------------------

void cPlaylist::Resume()
{

}

//-----------------------------------------------------------------------

void cPlaylist::Next()
{

}

//-----------------------------------------------------------------------

void cPlaylist::Prev()
{

}

//-----------------------------------------------------------------------

bool cAudio::IsPaused()
{
	return mIsPaused;
}

//-----------------------------------------------------------------------

void cPlaylist::AddMusic(cMusic *aMusic, int aId)
{
	if (!aMusic)
		return;

	if (!mMusic.empty())
	{
		if (mMusic.find(aId)->second)
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

	mMusic.insert(std::pair<int, cMusic *>(aId, aMusic));
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
