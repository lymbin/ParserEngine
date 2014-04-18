/*
 * engine.h
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#ifndef ENGINE_H_
#define ENGINE_H_

//Закомментировать строчки ниже для запрета показа отладочной информации и ошибок
#define DEBUG_SYS
#define DEBUG_ERRORS

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

const int 			SYS_AUDIO = 0; 		//без аудио
const int			SYS_FULLSCREEN = 0;	//оконный режим
const int			SYS_WIDTH = 800;	//ширина
const int			SYS_HEIGTH = 600;	//высота
const int			SYS_BPP = 32;		//палитра

const std::string 	SYS_VERSION = "0.0.0.0.3";
const std::string 	SYS_BUILD = "000003";

class graphics;
class sound;
class game;

class engine
{
	friend game;
	//Компоненты
	graphics *Graphics;

	SDL_Surface *screen;
	SDL_Event event; //перевести в отдельный компонент

public:
	int init();
	void CleanUp();
	static std::string IntToString(int number)
	{
		std::stringstream stream;
		stream << number;
		return stream.str();
	}

	engine();
	~engine();


};

class graphics
{
public:
	int init();
	void CleanUp();

	graphics();
	~graphics();

};
class image
{
	GLuint tex;
public:
	int fload_image(std::string file);
	image();
};

class sound
{

};

class game: public engine
{
	static bool quit;
public:
	void MainLoop();
	game();
	~game();
};




#endif /* ENGINE_H_ */
