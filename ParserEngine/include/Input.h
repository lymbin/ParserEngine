/*
 * Input.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "engine.h"

class engine;

class input
{
	//Система ввода: клики мышкой, кнопками клавиатуры и внутренняя система ввода - кнопки, текстовые боксы и т.п.

	// Карта состояний клавиш
	std::map <int, char> KeyStates;

	// Карта времени зажатия клавиши
	std::map <int, Uint32> KeyHeldTime;

	// Текущие координаты мыши
	int MouseX;
	int MouseY;

	std::vector <char> MouseButtons;

public:
	input();
	~input();

	// Инициализация системы ввода
	int init();

	// Выводим время зажатия клавиши(может понадобиться)
	Uint32 TimeHeld(eKey Key);

	// Клавиша зажата
	bool IsKeyHeld(eKey Key);

	// Клавиша нажата
	bool IsKeyDown(eKey key);

	// Клавиша отпущена
	bool IsKeyUp(eKey key);

	bool IsButtonDown(eButton button);

	bool IsButtonUp(eButton button);

	bool IsButtonHeld(eButton button);

	// Обновить
	int Update();

	int handle_event(SDL_Event event);

	engine *Engine;
};


#endif /* INPUT_H_ */
