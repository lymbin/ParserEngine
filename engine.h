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

#include "timer.h"


const int 			SYS_AUDIO = 0; 		//без аудио
const int			SYS_FULLSCREEN = 0;	//оконный режим
const int			SYS_WIDTH = 800;	//ширина
const int			SYS_HEIGTH = 600;	//высота
const int			SYS_BPP = 32;		//палитра
const int			SYS_FPS = 60;		//FPS

const GLint			SYS_GL_IMG_FILTER = GL_NEAREST; //Стандартный фильтр для изображений
const int			SYS_TEXT_SIZE = 16;				//Размер текста по умолчанию
const int			SYS_TEXT_DEPTH = 32;			//Глубина прорисовки текста

const std::string 	SYS_VERSION = "0.0.0.0.9";
const std::string 	SYS_BUILD = "000009";

class graphics;
class sound;

class game;
struct textureClass;

class font;
class text;

class engine
{
protected:
	//Компоненты
	graphics *Graphics;

	SDL_Surface *screen;
	SDL_Event event; //перевести в отдельный компонент

	//Таймер для подсчёта FPS
	p_timer fps;

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
class graphics
{
public:
	int init();
	void render();
	void CleanUp();

	graphics();
	~graphics();

};

class texture_manager
{
	//TODO: подумать над необходимостью класса
public:
	//virtual int Open(std::string source) = 0;
	virtual void Draw(float x, float y) = 0;																	//простая отрисовка
	virtual void Draw(float x, float y, float dx, float dy, float delta = 0, int center = 0) = 0;				//отрисовка в определённом размере
	virtual void Draw(float width, float heigth, float top_x, float top_y, float top_dx, float top_dy,		//отрисовка кусочка изображения
						float x, float y, float dx, float dy, float delta = 0, int center = 0) = 0;				//в определённом размере
	//virtual void Resize(float width, float heigth) = 0;
	texture_manager() {}
	virtual ~texture_manager() {}
};

struct textureClass
{
	//Содержит саму OpenGL текстуру изображения и всевозможные данные о ней
	GLuint tex;
	float pxw; //ширина в пикселах
	float pxh; //высота в пикселах

	std::string fileName; //Путь до файла
};

class image : public texture_manager
{
	textureClass texture;
public:
	image();
	image(std::string file, GLint filter = SYS_GL_IMG_FILTER);
	~image();

	textureClass GetTXT() { return texture; }
	float Width();
	float Heigth();

	//void render();
	void Draw(float x, float y);																//простая отрисовка
	void Draw(float x, float y, float dx, float dy, float delta = 0, int center = 0) ;			//отрисовка в определённом размере
	void Draw(float width, float heigth, float top_x, float top_y, float top_dx, float top_dy,	//отрисовка кусочка изображения
			float x, float y, float dx, float dy, float delta = 0, int center = 0) ;			//в определённом размере

	int Open(std::string source, GLint filter = SYS_GL_IMG_FILTER);
};
struct fontFormatting
{
	SDL_Color 	textcolor;
	SDL_Color 	bgcolor;
	int 		size;
	bool		bold;
	bool		italic;
	bool		underline;
};
class font
{
	friend text;
	TTF_Font 		*ttf_font;
	std::string 	fileName;
	fontFormatting	format;
public:
	font();
	font(std::string file, int size);
	~font();

	static int 	FontInit();
	TTF_Font	*GetFont() {return ttf_font;}
	int 		GetSize() { return format.size;}
	bool		isBold() { return format.bold;}
	bool		isItalic() { return format.italic;}
	bool		isUnderline() { return format.underline;}
	SDL_Color	GetColor() { return format.textcolor;}
	SDL_Color	GetBGColor() { return format.bgcolor;}
	fontFormatting GetFormat() { return format;}

	int Open(std::string source, int size);

	void SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255);
	void SetBGColor(Uint8 R, Uint8 G, Uint8 B);
	void SetStyle(bool bold, bool italic, bool underline);
	void SetFormat(bool bold, bool italic, bool underline, int size,
			Uint8 R, Uint8 G, Uint8 B, Uint8 A,
			Uint8 bgR, Uint8 bgG, Uint8 bgB);
	void Resize(int size);

	void Write(std::string text, GLuint tex, GLfloat x, GLfloat y);

	int GetHeigth();
	int CalcTextWidth(std::string text);
	int CalcTextHeigth(std::string text);

};
class text
{
	GLuint 		tex;
	std::string textString;
	font		*textFont;
	GLfloat x;
	GLfloat y;
public:
	GLuint 		GetTXT() {return tex;}
	font		*GetFont() {return textFont;}
	std::string GetText() { return textString;}


	text();
	text(std::string textStrings);
	text(std::string textStrings, font	*textFont);
	text(std::string textStrings, std::string fontFile, int fontSize);
	~text();

	void CreateTex();
	void ResizeText(int textSize);
	void Write(GLfloat new_x, GLfloat new_y, int center = 0);
	void Write(GLfloat new_x, GLfloat new_y, std::string textStrings, int center = 0);
	//void Write(GLfloat new_x, GLfloat new_y, std::string textStrings, int center = 0);

	void SetText(std::string newText) { textString = newText;}
	void SetFont(font *newFont);
	void SetCoordinates(GLfloat new_x = 0, GLfloat new_y = 0);

	void Draw(float x, float y, float dx, float dy, float delta = 0, int center = 0) ; //отрисовка в определённом размере


};
class sound
{

};



#endif /* ENGINE_H_ */
