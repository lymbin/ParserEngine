/*
 * engine.h
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <math.h>

#include <vector>
#include <list>
#include <map>
#include <stack>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#include <GL/glext.h>
#endif

#include "SystemConstants.h"

#include "timer.h"
#include "GraphicTypes.h"
#include "MathTypes.h"
#include "InputTypes.h"
#include "PhysicsTypes.h"
#include "Graphics.h"
#include "Sound.h"
#include "Input.h"
#include "Events.h"
#include "Physics.h"
#include "Gui.h"
#include "GuiTypes.h"

class graphics;
class cCamera;
class cAudio;
class window;

class cTexture;

class input;
class events;

class engine
{
public:
	engine();
	~engine();

	// Инициализация всех систем движка
	int init();

	// Очищаем все системы движка
	void CleanUp();



	//Компоненты
	graphics *Graphics;
	cAudio 		*Audio();
	input *Input;
	events *Events;
	cPhysics *Physics;
	//textures
	//animation
	//timer

	//TODO: Всё что ниже к движку не относится - перенести в собственные системы

	// Храним количество отрисованных кадров
	unsigned int frame;

	//Таймер для подсчёта FPS
	p_timer fps;

	static std::string IntToString(int number)
	{
		std::stringstream stream;
		stream << number;
		return stream.str();
	}
};

// TODO: нереализованные классы и идеи для классов:
class memory_manager
{
	// Общий менеджер памяти для объектов
};
class manager_manager
{
	// Менеджер для всех остальных менеджеров - босс :)
};
class texture_manager1
{
	// Менеджер для текстур - вроде бы класс реализован, но не используется так как надо
	// А надо вывести менеджер в глобал/синглтон и в принудительном порядке каждую новую
	//	текстуру добавлять в менеджер. Распределить потоки картой.
};
class animation_manager1
{
	// Менеджер для анимации
};
class buttonss
{
	// Не забыть про класс кнопок и вообще виджетов
};
#endif /* ENGINE_H_ */
