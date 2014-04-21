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

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"

const int 			SYS_AUDIO = 0; 		//без аудио
const int			SYS_FULLSCREEN = 0;	//оконный режим
const int			SYS_WIDTH = 800;	//ширина
const int			SYS_HEIGTH = 600;	//высота
const int			SYS_BPP = 32;		//палитра
const int			SYS_FPS = 60;		//FPS

const GLint			SYS_GL_IMG_FILTER = GL_NEAREST; //Стандартный фильтр для изображений

const std::string 	SYS_VERSION = "0.0.0.0.5";
const std::string 	SYS_BUILD = "000005";

class graphics;
class sound;
class p_timer;
class game;
struct textureClass;

class engine
{
protected:
	//Компоненты
	graphics *Graphics;

	SDL_Surface *screen;
	SDL_Event event; //перевести в отдельный компонент

	//p_timer fps;

public:
	int init();
	void CleanUp();
	void ResizeWin(int win_dX, int win_dY);

	SDL_Surface *Screen() {return screen;}
	SDL_Event Event() {return event;}

	static std::string IntToString(int number)
	{
		std::stringstream stream;
		stream << number;
		return stream.str();
	}

	engine();
	~engine();


};
class p_timer
{
	//Таймер
};
class graphics
{
public:
	int init();
	void render();
	void CleanUp();

	graphics();
	~graphics();

};

class image_manager
{
public:
	virtual int Open() = 0;
	void Draw(float x, float y);
	void Draw(float x, float y, float dX, float dY, float delta = 0, int center = 0);
	void Resize(float width, float heigth);
	image_manager() {}
	virtual ~image_manager() {}
};

struct textureClass
{
	//Содержит саму OpenGL текстуру изображения и всевозможные данные о ней
	GLuint tex;
	GLint filter;
	float pxw; //ширина в пикселах
	float pxh; //высота в пикселах

	std::string fileName;
};

class image : public image_manager
{

public:
	textureClass texture;
	image();
	image(std::string file, GLint filter = SYS_GL_IMG_FILTER);
	void render();
	int Open();
	~image();

	textureClass GetTXT() { return texture; }
	float Width();
	float Heigth();
};

class sound
{

};



#endif /* ENGINE_H_ */
