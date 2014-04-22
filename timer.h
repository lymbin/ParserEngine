/*
 * timer.h
 *
 *  Created on: 21.04.2014
 *      Author: dmitry
 */

#ifndef TIMER_H_
#define TIMER_H_

class p_timer
{
	//Класс таймера
	//Начальное значение таймера
	int startTicks;

	//Значение таймера при паузе
	int pausedTicks;

	//Пауза
	bool paused;
	//Старт
	bool started;

public:
	p_timer();

	//Функции для работы с таймером
	void start();
	void stop();
	void pause();
	void unpause();

	//Получить текущее время
	int get_ticks();

	bool is_started() {return started;}
	bool is_paused() {return paused;}
};

#endif /* TIMER_H_ */
