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

#include "timer.h"


const int 			SYS_AUDIO = 0; 		//без аудио
const int			SYS_FULLSCREEN = 0;	//оконный режим
const int			SYS_WIDTH = 1024;	//ширина
const int			SYS_HEIGTH = 768;	//высота
const int			SYS_BPP = 32;		//палитра
const int			SYS_FPS = 60;		//FPS

const GLint			SYS_GL_IMG_FILTER = GL_NEAREST; //Стандартный фильтр для изображений
const int			SYS_TEXT_SIZE = 16;				//Размер текста по умолчанию
const int			SYS_TEXT_DEPTH = 32;			//Глубина прорисовки текста
const std::string	SYS_GL_IMG_ZIP_MODE = "rb";

const int			SYS_AUDIO_VOLUME = 64;
const int			SYS_AUDIO_RATE = 44100;
const Uint16 		SYS_AUDIO_FORMAT = AUDIO_S16; /* 16-bit stereo */
const int			SYS_AUDIO_CHANNELS = 2;
const int 			SYS_AUDIO_BUFFERS = 1024;

const std::string 	SYS_VERSION = "0.0.0.0.18";
const std::string 	SYS_BUILD = "000018";

class graphics;
class audio;

class game;
struct textureClass;
//class texture;
class image;
class texture_manager;

class font_manager;
class font;
class text;

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
	//input
	//fonts
	//textures
	//animation
	//timer

	//TODO: Всё что ниже к движку не относится - перенести в собственные системы
	SDL_Event event; //перевести в отдельный компонент

	//Таймер для подсчёта FPS
	p_timer fps;

	SDL_Event 	Event() {return event;}

	static std::string IntToString(int number)
	{
		std::stringstream stream;
		stream << number;
		return stream.str();
	}
};
class graphics
{
	friend image;
	texture_manager *TextureManager;	// Менеджер текстур
	font_manager 	*FontManager;

	SDL_Surface *screen;	// Сурфейс окна
	GLuint CurrentTexture;	// Текущая забинженная текстура

	int FullScreen;

public:
	graphics();
	~graphics();

	// Инициализируем все внутренние подсистемы
	int init();

	// Инициализация OpenGL
	int initGL();

	// Очищаем всю графическую систему
	void CleanUp();

	// Очистка экрана
	void ClearScreen();

	// Отрисовываем буферы на экране
	void SwapBuffers();

	// Меняем размеры окна
	void ResizeWin(int win_dX, int win_dY);

	// Меняем полноэкранный режим
	void ToggleFullScreen();

	// Рисуем примитивы
	// TODO: возможно вынести в одтельный класс
	// TODO: позаботиться об их удалении без очистки экрана
	// TODO: дать только некоторым классам использовать эти функции
	//		например, из примитивов рисовать что-то полезное внутри другого класса

	// Рисуем каркас прямоугольника цветными линиями
	void DrawRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
						GLfloat red = 0, GLfloat green = 0, GLfloat blue = 0, GLfloat alpha = 1);

	// Рисуем заполненный цветом прямоугольник
	void DrawFilledRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
							GLfloat red = 0, GLfloat green = 0, GLfloat blue = 0, GLfloat alpha = 1);

	// Рисуем линию
	void DrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
					GLfloat red = 0, GLfloat green = 0, GLfloat blue = 0, GLfloat alpha = 1);

	// Устанавливаем текущую забинженную текстуру
	void SetCurrentTexture(GLuint texture);

	SDL_Surface *Screen() {return screen;}
	GLuint GetCurrentTexture() {return CurrentTexture;}

};

class texture_manager
{
	// Менеджер текстур - управляет памятью.
	// Пока не используется для изображений из-за сложности передачи каждому новому изображению указатель на менеджер
	// TODO: доделать позднее выделив в глобал или в синглтон
	friend image;
	friend graphics;
	graphics *Graphics;
protected:

	// Вектор хранящий все текстуры, которыми управляем
	std::vector< image *> Textures;
public:
	texture_manager();
	~texture_manager();

	// Получаем информацию по текстуре
	image *GetTextureInfos(GLuint texture);

	//Перезагружаем текстуры
	void ReloadTextures();

	//
	//void RedrawTextures();

	//Удаляем текстуры
	void DeleteTextures();

	//Добавляем и удаляем из вектора управляющего текстурами
	void ManageTexture(image *managed_image);
	void UnManageTexture(image *managed_image);

	void SetGraphics(graphics *setGraphics);

};

struct textureClass
{
	//Содержит саму OpenGL текстуру изображения и всевозможные данные о ней
	GLuint tex;
	float pxw; //ширина в пикселах
	float pxh; //высота в пикселах

	std::string fileName; //Путь до файла
};

class image
{
	friend texture_manager;
	textureClass texture;
	texture_manager *TextureManager; // TODO:в глобал или в синглтон

	//TODO: протестировать
	//std::vector< std::vector< bool > > m_PixelOn; // Храним пиксели текстуры для модуля столкновений(коллизии)

public:
	image();
	image(std::string file, GLint filter = SYS_GL_IMG_FILTER);
	~image();

	void SetTexManager(texture_manager *TextureManager);

	textureClass GetTXT() { return texture; }

	// Вывод реальных размеров изображения
	float Width();
	float Heigth();

	// Различные функции отрисовки
	void Draw(float x, float y);																//простая отрисовка
	void Draw(float x, float y, float dx, float dy, float delta = 0, int center = 0);			//отрисовка в определённом размере
	void Draw(float width, float heigth, float top_x, float top_y, float top_dx, float top_dy,	//отрисовка кусочка изображения
			  float x, float y, float dx, float dy, float delta = 0, int center = 0);			//в определённом размере

	// Открываем изображение из файла
	int Open(std::string source, GLint filter = SYS_GL_IMG_FILTER);
	int OpenFromZip(std::string source, GLint filter = SYS_GL_IMG_FILTER);

	// Создаём текстуру из сурфейса(бывшая часть функции Open)
	void MakeTexture(SDL_Surface *Surface, GLint filter = SYS_GL_IMG_FILTER, bool LoadPixels = false);

	// Полностью перерисовываем изображения с различными функциями отрисовки
	void Redraw(float x, float y, float dx = -1, float dy = -1, float delta = 0, int center = 0);
	void Redraw(float width, float heigth, float top_x, float top_y, float top_dx, float top_dy,
				float x, float y, float dx = -1, float dy = -1, float delta = 0, int center = 0);

	// Биндим текстуру для работы с OpenGL
	void Bind();

	// Перезагружаем текстуру в памяти из файла
	void Reload();

	// Удаляем текстуру
	void Delete();

};

class font_manager
{
	// Менеджер шрифтов - управляет памятью всех шрифтов как менеджер текстур
	// Пока не используется, всё по той же причине, что и менеджер текстур - из-за сложности передачи каждому новому шрифту указатель на менеджер
	// TODO: доделать позднее выделив в глобал или в синглтон
	friend font;

	// Добавляем и удаляем шрифты из менеджера
	void ManageFont(font *managed_font);
	void UnManageFont(font *managed_font);

	// Храним все шрифты
	std::vector < font *> Fonts;
	graphics *Graphics;
public:
	font_manager();
	~font_manager();

	// Инициализация TTF
	static int FontsInit();

	// Удаляем шрифты из памяти
	void DeleteFonts();

	// Устанавливаем указатель на графику
	void SetGraphics(graphics *setGraphics);

};
struct fontFormatting
{
	// Формат шрифта
	SDL_Color 	textcolor; 	// Цвет текста TODO: убрать отсюда
	SDL_Color 	bgcolor;	// Цвет задней текстуры
	int 		size;		// Размер шрифта
	bool		bold;		// Жирный
	bool		italic;		// Курсив
	bool		underline;	// Подчёркивание
};
class font
{
	friend text;
	friend font_manager;

	TTF_Font 		*ttf_font; 	// SDL TTF шрифт
	std::string 	fileName;	// Файл со шрифтом
	fontFormatting	format;		// Формат шрифта, содержащий множество параметров
	bool 			StaticFont; // Шрифт не меняется при изменении камеры TODO: протестировать позднее

	font_manager *FontManager;
public:
	font(std::string file = "", int size = SYS_TEXT_SIZE);
	~font();

	// Получаем все private параметры, в том числе формата шрифта
	TTF_Font	*GetFont() {return ttf_font;}
	int 		GetSize() { return format.size;}
	bool		isBold() { return format.bold;}
	bool		isItalic() { return format.italic;}
	bool		isUnderline() { return format.underline;}
	SDL_Color	GetColor() { return format.textcolor;}
	SDL_Color	GetBGColor() { return format.bgcolor;}
	fontFormatting GetFormat() { return format;}
	bool GetStaticFont() { return StaticFont;}

	// Открываем шрифт с заданным размером
	int Open(std::string source, int size = SYS_TEXT_SIZE);

	// Устанавливаем цвет шрифта, заднего фона, стиль и формат
	void SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255);
	void SetBGColor(Uint8 R, Uint8 G, Uint8 B);
	void SetStyle(bool bold, bool italic, bool underline);
	void SetFormat(bool bold, bool italic, bool underline, int size,
			Uint8 R, Uint8 G, Uint8 B, Uint8 A,
			Uint8 bgR, Uint8 bgG, Uint8 bgB);

	// Изменяем размер шрифта
	void Resize(int size);

	// Изменяем статический шрифт для движения с камерой TODO: протестировать
	void SetStatic(bool static_font);

	// Устанавливаем менеджер шрифтов для полуавтоматического управления памятью менеджером
	void SetTexManager(font_manager *FonManager);

	// Пишем текст прямо из класса шрифт и вносим текст в менеджер текста
	void Write(std::string text, GLuint tex, GLfloat x, GLfloat y);

	// Получаем высоту шрифта
	int GetHeigth();

	// Получаем ширину и высоту текстуры с текстом данного шрифта
	int CalcTextWidth(std::string text);
	int CalcTextHeigth(std::string text);

};
class text
{
	friend game;

	GLuint 		tex;		// OpenGL текстура
	std::string textString;	// Сам текст для написания(можно менять и делать всё новые и новые записи тем же шрифтом)
	font		*textFont;	// Указатель на шрифт

	GLfloat x;	// Точка отрисовки
	GLfloat y;	//

	// Создаём текстуру
	void CreateTex();

public:
	text(std::string textStrings = "", std::string fontFile = "", int fontSize = SYS_TEXT_SIZE);
	text(std::string textStrings, font	*textFont);
	~text();

	// Получаем все private настройки
	GLuint 		GetTXT() {return tex;}
	font		*GetFont() {return textFont;}
	std::string GetText() { return textString;}

	void Write(GLfloat new_x, GLfloat new_y, int center = 0);
	void Write(GLfloat new_x, GLfloat new_y, std::string textStrings, int center = 0);
	//void Write(GLfloat new_x, GLfloat new_y, std::string textStrings, int center = 0);

	// Устанавливаем текст для отрисовки
	void SetText(std::string newText);

	// Устанавливаем шрифт
	void SetFont(font *newFont);

	// Новые координаты
	void SetCoordinates(GLfloat new_x = 0, GLfloat new_y = 0);

	// Меняем размер текста(шрифта)
	// TODO: переделать - сделать изменение размера текста не зависимое от размера шрифта
	void ResizeText(int textSize);

	void Draw(float x, float y, float dx, float dy, float delta = 0, int center = 0) ; //отрисовка в определённом размере


};
class input
{
	//Система ввода: клики мышкой, кнопками клавиатуры и внутренняя система ввода - кнопки, текстовые боксы и т.п.
public:
	int init();
	void CleanUp();

	input();
	~input();

};
class button
{

};


class sound
{
	friend audio;
	Mix_Chunk *Sound;
	std::string fileName;
public:
	sound(std::string file = "");
	~sound();

	int Open(std::string file);

	void Delete();

	void Play(unsigned int Repeats = 0);
};
class music
{
	friend audio;
	Mix_Music *Music;
	std::string fileName;

	void Stop();

	void Pause();

	void Resume();
public:
	music(std::string file = "");
	~music();

	int Open(std::string file);

	void Delete();

	void Play(unsigned int Repeats = 0);
};
class audio
{
	// Менеджер звуков и музыки
	unsigned int SoundVolume;
	unsigned int MusicVolume;

	std::vector< sound *> Sounds;
	std::vector< music *> Music;

	music *CurrentMusic;

public:
	audio();
	~audio();

	// Инициализация аудио системы
	int init();

	// Устанавливаем громкость звуков(в первый попавшийся канал) - TODO: Доделать это
	// и громкость музыки
	void SetSoundVolume(unsigned int Volume);
	void SetMusicVolume(unsigned int Volume);

	// Получаем громкость
	unsigned int GetSoundVolume();
	unsigned int GetMusicVolume();

	// Удаляем всю музыку из менеджера
	void DeleteAllMusic();

	// Останавливаем музыку
	void StopMusic();

	// Проигрывание музыки в лупе - тест
	void PlayLoopMusic();

	// Паузим музыку
	void PauseMusic();

	// Возобновляем проигрывание музыки
	void ResumeMusic();

	// Удаляем все звуки из менеджера
	void DeleteAllSounds();

	// Функции управления памятью звуков
	void ManageSound(sound *managed_sound);
	void UnManageSound(sound *managed_sound);

	// Функции управления памятью музыки
	void ManageMusic(music *managed_music);
	void UnManageMusic(music *managed_music);
};



#endif /* ENGINE_H_ */
