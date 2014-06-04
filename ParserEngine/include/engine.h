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

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#include <GL/glext.h>
#endif

#include "SystemConstants.h"

#include "timer.h"
#include "GraphicTypes.h"
#include "keys.h"
#include "PhysicsTypes.h"
#include "Graphics.h"
#include "Sound.h"
#include "Input.h"
#include "Events.h"
#include "Physics.h"


class graphics;
class camera;
class audio;
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
	audio *Audio;
	input *Input;
	events *Events;
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

// TODO: нереализованные классы:
class memory_manager
{
	// Общий менеджер памяти
};

#endif /* ENGINE_H_ */
