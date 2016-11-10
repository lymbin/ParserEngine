/*
 * Animation.h
 *
 *  Created on: 30.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Texture.h"

class animation;
class cTexture;

class cAnimationManager
{
	std::vector <animation * > Animations;
public:
	cAnimationManager();
	~cAnimationManager();

	void DeleteAnims();

	void PauseAnims();

	void ResumeAnims();

	void ResetAnims();

	void UpdateAnims();

	void ManageAnimation(animation *managed_anim);
	void UnManageAnimation(animation *managed_anim);
};
class animation
{
	// Для первого типа анимаций текстуры полностью лежат в image и загружаются туда через интерфейс image
	// Смена кадров это смена image'ов.
	//TODO: глянуть потом этот тип анимации
	//std::vector< image * > Textures;

	cTexture * CurrentTexture;

	// Для второго типа анимаций текстуры лежат в одной из того же вектора(только там одна текстура)
	// Смена кадров это смена кусков текстуры
	tRectVector frames;

	// Текущий фрейм анимации
	unsigned int CurrentFrame;

	// Таймер для тиков и подсчёта
	Uint32 SpeedTicks;

	// Скорость анимации
	Uint32 AnimSpeed;

	// Остановлена или нет
	bool Paused;

	// Закончена или нет
	bool AnimOver;

	// Количество повторов анимации для статики
	// У динамики выставляется значение -1, чтобы не прерывать анимации по событиям
	int Repeats;

	std::string ItemName;

public:
	animation();
	~animation();

	// Удаление анимации
	void Delete();

	// Паузим и возобновляем по необходимости анимацию
	void Pause();
	void Resume();

	// Сбрасываем тики и текущий фрейм
	void Reset();

	// Обновляем фрейм(каждый фрейм при событии)
	void Update();

	// Отрисовываем текущий фрейм анимации
	// Нужно лишь задать точку в которой начинается отрисовка
	void Draw(float x, float y, GLfloat Scale = 1, GLfloat Rotation = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Очищаем массив фреймов
	void ClearFrames();

	// Устанавливаем главную текстуру
	void SetTexture(cTexture *Texture);

	// Устанавливаем скорость анимации
	void SetSpeed(Uint32 Speed);

	// Устанавливаем количество повторов, после которых не проигрываем анимацию
	void SetRepeats(int rep);

	void SetItemName(std::string name);

	// Добавляем новый фрейм в индекс или в конец
	void AddNewFrame(PE_Rect aFrame, int index = -1);

	// Прыгаем на фрейм с заданным индексом
	void JumpToFrame(unsigned int index);


	// Пауза
	bool IsPaused();

	bool IsOver();

	// Получаем главную текстуру
	cTexture *GetTexture();

	// Получаем фрейм с индексом
	PE_Rect GetFrame(unsigned int index);

	// Получаем номер текущего фрейма
	int GetTrackNumber();

	// Получаем количество повторов, которые остались
	int GetRepeats();

	// Получаем скорость анимации
	Uint32 GetSpeed();

	// Получаем размер массива фреймов
	unsigned int GetFramesSize();

	std::string GetItemName();

	//int anim_type; // Тип анимации
};


#endif /* ANIMATION_H_ */
