/*
 * Events.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include "SDL2/SDL.h"
#include "Input.h"
#include "Graphics.h"

class input;
class window;

// Класс событий, которые должны обрабатывать разные классы
// Например обрабатывать нажатие кнопок будет класс Input, класс Window будет обрабатывать изменения размеров окна
class events
{
	SDL_Event event;

public:
	events(input *In = 0, window *Win = 0);
	~events();

	int init();

	int handle_events();

	input *Input;
	window *Window;
};


#endif /* EVENTS_H_ */
