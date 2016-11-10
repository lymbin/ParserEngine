/*
 * animation.cpp
 *
 *  Created on: 06.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "Animation.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cAnimationManager::cAnimationManager()
{
	Animations.clear();
}

//-----------------------------------------------------------------------

cAnimationManager::~cAnimationManager()
{
	DeleteAnims();
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cAnimationManager::DeleteAnims()
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		delete Animations[loop];
	}
	Animations.clear();
}

//-----------------------------------------------------------------------

void cAnimationManager::PauseAnims()
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		Animations[loop]->Pause();
	}
}

//-----------------------------------------------------------------------

void cAnimationManager::ResumeAnims()
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		Animations[loop]->Resume();
	}
}

//-----------------------------------------------------------------------

void cAnimationManager::ResetAnims()
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		Animations[loop]->Reset();
	}
}

//-----------------------------------------------------------------------

void cAnimationManager::UpdateAnims()
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		Animations[loop]->Update();
	}
}

//-----------------------------------------------------------------------

void cAnimationManager::ManageAnimation(animation *managed_anim)
{
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		if(Animations[loop] == managed_anim)
		{
			return;
		}
	}

	Animations.push_back(managed_anim);
}

//-----------------------------------------------------------------------

void cAnimationManager::UnManageAnimation(animation *managed_anim)
{
	// Удаляем анимацию из вектора управления
	// Внимание: Это только удалит анимацию из вектора управления, но не удалит саму анимацию
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage
	// TODO: пересмотреть идею менеджеров памяти и сделать их и их компоненты связанными - так что при удалении из вектора удалялся и сам компонент

	int place = -1;

	// Ищем анимацию в векторе
	for(unsigned int loop = 0; loop < Animations.size(); loop++)
	{
		if(Animations[loop] == managed_anim)
		{
			place = loop;
			break;
		}
	}

	// анимация не найден - выходим
	if(place < 0)
		return;

	if((unsigned int)(place+1) == Animations.size())
	{
		// Анимация в самом конце - удаляем, перед этим обнулив указатель на менеджер
		//Sounds[place]->AudoManager = 0;
		Animations.pop_back();
	}
	else
	{
		// Анимация где-то внутри вектора - удаляем, перед этим обнулив указатель на менеджер
		//TODO: проверить
		//Sounds[place] = Sounds[ Sounds.size() - 1 ];
		//Sounds[place]->AudoManager = 0;
		Animations.erase( Animations.begin() + place);
	}
}

//-----------------------------------------------------------------------

animation::animation()
{
	//anim_type = 0;
	CurrentTexture = 0;
	CurrentFrame = 0;

	AnimSpeed = 0;
	SpeedTicks = SDL_GetTicks();
	Paused = false;
	AnimOver = false;

	Repeats = -1; // Без повторов

	frames.clear();

	// TODO: конструировать имя в зависимости от расположения в массиве менеджера, например, anim1, anim2, ...
	ItemName = "";
}

//-----------------------------------------------------------------------

animation::~animation()
{
	Delete();
}

//-----------------------------------------------------------------------

// Очищаем память
void animation::Delete()
{
	if(CurrentTexture)
		delete CurrentTexture;

	CurrentTexture = 0;
	CurrentFrame = 0;

	AnimSpeed = 0;
	SpeedTicks = 0;
	Paused = false;
	AnimOver = false;

	Repeats = 0;

	frames.clear();
}

//-----------------------------------------------------------------------

void animation::Pause()
{
	Paused = true;
}

//-----------------------------------------------------------------------

void animation::Resume()
{
	Paused = false;
	SpeedTicks = SDL_GetTicks();
}

//-----------------------------------------------------------------------

void animation::Reset()
{
	CurrentFrame = 0;
	SpeedTicks = SDL_GetTicks();
}

//-----------------------------------------------------------------------

void animation::Update()
{
	// Получаем следующий фрейм

	if((Paused)||(!CurrentTexture)||(frames.empty()))
	{
		// Выходим если хоть один из случаев выше верен
		return;
	}

	if(AnimSpeed)
	{
		// Скорость анимации задана - значит смотрим нужно ли обновлять фрейм
		if(1000/AnimSpeed > (SDL_GetTicks() - SpeedTicks))
		{
			// Фреймы не надо обновлять - выходим
			return;
		}
	}
	SpeedTicks = SDL_GetTicks(); // Получаем тики для скорости

	CurrentFrame++; // Следющий фрейм

	if(CurrentFrame >= frames.size())
	{
		// Фреймы закончились - для начала чекаем повторы
		// Если повторы не заданы(т.е. динамик), то выходим
		if(Repeats!=-1)
		{
			if(Repeats == 0)
			{
				// Если повторы на нуле, то заканчиваем анимацию
				Paused = true;
				AnimOver = true;
			}
		}
		// Устанавливаем текущий фрейм в начало
		CurrentFrame = 0;
	}
}

//-----------------------------------------------------------------------

// Отрисовываем текстуру анимации
void animation::Draw(float x, float y, GLfloat Scale, GLfloat Rotation,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	CurrentTexture->Draw(x, y, &frames[CurrentFrame],Scale, Rotation, cColor(red, green, blue, alpha));
}

//-----------------------------------------------------------------------

// Очищаем массив фреймов
void animation::ClearFrames()
{
	Reset();
	frames.clear();
}

//-----------------------------------------------------------------------

void animation::SetTexture(cTexture *Texture)
{
	CurrentTexture = Texture;
}

//-----------------------------------------------------------------------

void animation::SetSpeed(Uint32 Speed)
{
	AnimSpeed = Speed;
	SpeedTicks = SDL_GetTicks();
}

//-----------------------------------------------------------------------

// Устанавливаем количество повторов, после которых не проигрываем анимацию
void animation::SetRepeats(int rep)
{
	Repeats = rep;
	AnimOver = false;
}

//-----------------------------------------------------------------------

void animation::SetItemName(std::string name)
{
	ItemName = name;
	// TODO: не забыть чекнуть имя в менеджере
}

//-----------------------------------------------------------------------

// Добавляем новый фрейм в индекс или в конец
void animation::AddNewFrame(PE_Rect Rect, int index)
{
	if((index < 0)||(unsigned int)index > frames.size())
	{
		frames.push_back(Rect);
	}
	else
	{
		frames.insert(frames.begin() + index, Rect);
	}
}

//-----------------------------------------------------------------------

// Прыгаем на фрейм с заданным индексом
void animation::JumpToFrame(unsigned int index)
{
	if(index > frames.size())
		return;

	CurrentFrame = index;
	SpeedTicks = SDL_GetTicks();

	if(index != frames.size())
		AnimOver = false;
}

//-----------------------------------------------------------------------

bool animation::IsPaused()
{
	return Paused;
}

//-----------------------------------------------------------------------

bool animation::IsOver()
{
	return AnimOver;
}

//-----------------------------------------------------------------------

// Получаем главную текстуру
cTexture *animation::GetTexture()
{
	return CurrentTexture;
}

//-----------------------------------------------------------------------

// Получаем фрейм с индексом или текущий фрейм
PE_Rect animation::GetFrame(unsigned int index)
{
	if(index > frames.size())
		return frames[CurrentFrame];
	else
	{
		return frames[index];
	}
}

//-----------------------------------------------------------------------

// Получаем номер текущего фрейма
int animation::GetTrackNumber()
{
	return CurrentFrame;
}

//-----------------------------------------------------------------------

// Получаем количество повторов, которые остались
int animation::GetRepeats()
{
	return Repeats;
}

//-----------------------------------------------------------------------

// Получаем скорость анимации
Uint32 animation::GetSpeed()
{
	return AnimSpeed;
}

//-----------------------------------------------------------------------

unsigned int animation::GetFramesSize()
{
	return frames.size();
}

//-----------------------------------------------------------------------

std::string animation::GetItemName()
{
	return ItemName;
}

//-----------------------------------------------------------------------
