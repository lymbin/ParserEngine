/*
 * timer.cpp
 *
 *  Created on: 21.04.2014
 *      Author: dmitry
 */

#include "timer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>

#include "SDL/SDL.h"

p_timer::p_timer()
{
	//Создаём таймер
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void p_timer::start()
{
	//Устнавливаем значение started в истину
	started = true;

	//Паузу в ложь
	paused = false;

	//Получаем стартовое время
	startTicks = SDL_GetTicks();
}
void p_timer::stop()
{
	//Останавливаем таймер
	started = false;

	paused = false;
}
int p_timer::get_ticks()
{
	//Получаем время таймера
	if(started)
	{
		if(paused)
		{	//Если на паузе, то выводим время паузы
			return pausedTicks;
		}
		else
		{	//Если таймер работает, то получаем время как разность текущего и начального времени таймера
			return SDL_GetTicks() - startTicks;
		}
	}
	//Таймер не стартанул - выводим ноль
	return 0;
}
void p_timer::pause()
{
	//Устанавливаем паузу только в том случае, если таймер запущен и не стоит на паузе
	if((started)&&(!paused))
	{
		paused = true;

		//Запоминаем время паузы
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}
void p_timer::unpause()
{
	//Анпаузим, только если таймер на паузе
	if(paused)
	{
		paused = false;

		//Стартовое время устанавливаем на точку анпаузы минус время до паузы
		startTicks = SDL_GetTicks() - pausedTicks;

		pausedTicks = 0;
	}
}
