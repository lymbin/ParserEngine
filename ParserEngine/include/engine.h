/*
 * engine.h
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#ifndef ENGINE_H_
#define ENGINE_H_

//Закомментировать строчки ниже для запрета показа отладочной информации и ошибок
#define DEBUGGING

#ifdef DEBUGGING
#define DEBUG_SYS
#define DEBUG_ERRORS
#define DEBUG_INFOS
#endif


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
#include "GraphicTypes.h"
#include "keys.h"
#include "PhysicsTypes.h"

///////////////// MAIN SYSTEM CONSTANTS /////////////////

const int 			SYS_AUDIO = 0; 					//аудио(0 - отключено, !0 - включено)
const int			SYS_FULLSCREEN = 0;				//оконный режим(0 - оконный режим, !0 - полноэкранны режим)
const int			SYS_REGULATING_FRAME_RATE = 0;	//регулируем ли FPS(0 - нет, !0 - да)

const int			SYS_WIDTH = 1024;				//ширина
const int			SYS_HEIGTH = 768;				//высота
const int			SYS_BPP = 32;					//палитра
const int			SYS_FPS = 60;					//FPS

const int			SYS_FRAME_PIXELS = 10;


///////////////// GRAPHIC SYSTEM CONSTANTS /////////////////

const GLint			SYS_GL_IMG_FILTER = GL_NEAREST; //Стандартный фильтр для изображений
const int			SYS_TEXT_SIZE = 16;				//Размер текста по умолчанию
const int			SYS_TEXT_DEPTH = 32;			//Глубина прорисовки текста
const std::string	SYS_GL_IMG_ZIP_MODE = "rb";

///////////////// AUDIO SYSTEM CONSTANTS /////////////////

const int			SYS_AUDIO_VOLUME = 64;
const int			SYS_AUDIO_RATE = 44100;
const Uint16 		SYS_AUDIO_FORMAT = AUDIO_S16; /* 16-bit stereo */
const int			SYS_AUDIO_CHANNELS = 2;
const int 			SYS_AUDIO_BUFFERS = 1024;

///////////////// VERSIONS CONSTANTS /////////////////

const std::string 	SYS_VERSION = "0.0.0.0.38";
const std::string 	SYS_BUILD = "000038";
const std::string	SYS_TEST_VERSION = "0.1.3.38";



class graphics;
class camera;
class audio;
class window;

class game;
struct textureClass;
//class texture;
class image;
class texture_manager;


class font_manager;
class font;
class text;

class animation;

class input;
class events;

class collision_AABB;
class collision_OBB;
class collision_body;
class collision_layer;
class collision;


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
	collision *Collision;
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
class graphics
{
	friend image;
	texture_manager *TextureManager;	// Менеджер текстур
	font_manager 	*FontManager;

	SDL_Surface *screen;	// Сурфейс окна
	GLuint CurrentTexture;	// Текущая забинженная текстура

	int FullScreen;

	int	ScreenWidth; 	//ширина
	int	ScreenHeigth;	//высота
	int	ScreenBpp;		//палитра

	camera *Camera;
	window *Window;

public:
	graphics(int W = SYS_WIDTH, int H = SYS_HEIGTH, int BPP = SYS_BPP);
	~graphics();

	// Инициализируем все внутренние подсистемы
	int init();

	// Инициализация OpenGL
	int initGL();

	// Очищаем всю графическую систему
	void CleanUp();

	// Очистка экрана
	void ClearScreen();

	// Очищаем цвет
	void ClearColor();

	// Устанавливаем цвет
	void SetColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f);

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
						GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Рисуем заполненный цветом прямоугольник
	void DrawFilledRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
								GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Рисуем линию
	void DrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
					GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Устанавливаем текущую забинженную текстуру
	void SetCurrentTexture(GLuint texture);

	SDL_Surface *Screen();
	GLuint GetCurrentTexture();

	int GetScreenWidth();
	int GetScreenHeigth();
	int GetScreenBpp();

	camera *GetCamera();
	window *GetWindow();
};
class camera
{
	GLfloat gCameraX;
	GLfloat gCameraY;
public:
	camera();
	~camera();

	void MoveTo(GLfloat x, GLfloat y);
	void SetPosition(GLfloat x, GLfloat y);

	GLfloat GetXposition();
	GLfloat GetYposition();
};
class window
{
	graphics *Graphics;
	bool windowed;
public:
	window(graphics *gfx = 0);

	int handle_event(SDL_Event event);
	void ToggleFullScreen();

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
	friend animation;
	textureClass texture;
	texture_manager *TextureManager; // TODO:в глобал или в синглтон

	//TODO: протестировать
	//std::vector< std::vector< bool > > m_PixelOn; // Храним пиксели текстуры для модуля столкновений(коллизии)

public:
	image(std::string file = "", GLint filter = SYS_GL_IMG_FILTER);
	~image();

	void SetTexManager(texture_manager *TextureManager);

	textureClass GetTXT();

	// Вывод реальных размеров изображения
	float Width();
	float Heigth();

	// Различные функции отрисовки
	void DrawTransform(float x, float y, PE_Rect *Box,
					GLfloat Scale = 1, GLfloat Rotation = 0);

	/*
	 *  Простая отрисовка полной текстуры в точке (x,y) и размерами исходной текстуры
	 *  с заданным увеличением и поворотом текстуры(по умолчанию отсутствует)
	 *  а также цветом текстуры(по умолчанию белый)
	 */
	void Draw(float x, float y, GLfloat Scale = 1, GLfloat Rotation = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	/*
	 *  Отрисовка куска текстуры в точке (x, y)
	 *  с параметрами куска - (top_x, top_y) (dx, dy)
	 *  с заданным увеличением и поворотом текстуры(по умолчанию отсутствует)
	 *  а также цветом текстуры(по умолчанию белый)
	 */
	void Draw(float x, float y, PE_Rect *Box,
			GLfloat Scale = 1, GLfloat Rotation = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);



	// Открываем изображение из файла
	int Open(std::string source, GLint filter = SYS_GL_IMG_FILTER);
	int OpenFromZip(std::string source, GLint filter = SYS_GL_IMG_FILTER);

	// Создаём текстуру из сурфейса(бывшая часть функции Open)
	void MakeTexture(SDL_Surface *Surface, GLint filter = SYS_GL_IMG_FILTER, bool LoadPixels = false);

	// Полностью перерисовываем изображения с различными функциями отрисовки
	void Redraw(float x, float y, GLfloat Scale = 1, GLfloat Rotatation = 0,
				GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);
	void Redraw(float x, float y, PE_Rect *Box,
			GLfloat Scale = 1, GLfloat Rotatation = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

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
	SDL_Color 	textcolor; 	// Цвет текста TODO: убрать отсюда в текст
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
	// обязательно удалить tex после каждого кадра - иначе будет засорять память
	void Write(std::string text, GLfloat x, GLfloat y, GLuint *tex,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Получаем высоту шрифта
	int GetHeigth();

	// Получаем ширину и высоту текстуры с текстом данного шрифта
	int CalcTextWidth(std::string text);
	int CalcTextHeigth(std::string text);

};

class text_manager
{
	// Менеджер текста - управляет памятью.
	// Пока не используется для текста из-за сложности передачи каждому новому тексту указатель на менеджер
	// TODO: доделать позднее выделив в глобал или в синглтон
	friend text;
	friend graphics;
	graphics *Graphics;
protected:

	// Вектор хранящий все текстуры, которыми управляем
	std::vector< text *> Texts;
public:
	text_manager();
	~text_manager();

	// Получаем информацию по тексту
	text *GetTextInfos(GLuint texture);

	//Удаляем текст
	void DeleteText();

	//Добавляем и удаляем из вектора управляющего текстом
	void ManageText(text *managed_text);
	void UnManageText(text *managed_text);

	void SetGraphics(graphics *setGraphics);
};
class text
{
	friend game;
	friend text_manager;

	GLuint 		tex;		// OpenGL текстура
	std::string textString;	// Сам текст для написания(можно менять и делать всё новые и новые записи тем же шрифтом)
	font		*textFont;	// Указатель на шрифт

	GLfloat x;	// Точка отрисовки
	GLfloat y;	//

	// Создаём текстуру
	void CreateTex();
	void Bind();

	// Отрисовка текста от точки, в определённом размере, с заданным углом поворота,
	//		относительно центра или левого верхнего угла указанным цветом
	void Draw(float x, float y, int size = -1, GLfloat Rotation = 0, int center = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f) ;

public:
	text(std::string textStrings = "", std::string fontFile = "", int fontSize = SYS_TEXT_SIZE);
	text(std::string textStrings, font	*textFont);
	~text();

	// Получаем все private настройки
	GLuint 		GetTXT() {return tex;}
	font		*GetFont() {return textFont;}
	std::string GetText() { return textString;}

	void Write(GLfloat new_x, GLfloat new_y, std::string text = "", int size = -1, GLfloat Rotation = 0, int center = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Устанавливаем текст для отрисовки
	void SetText(std::string newText);

	// Устанавливаем шрифт
	void SetFont(font *newFont);

	// Новые координаты
	void SetCoordinates(GLfloat new_x = 0, GLfloat new_y = 0);

	// Меняем размер текста(шрифта)
	// TODO: переделать - сделать изменение размера текста не зависимое от размера шрифта
	void ResizeText(int textSize);
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
class animation_manager
{
	std::vector <animation * > Animations;
public:
	animation_manager();
	~animation_manager();

	void DeleteAnims();

	void PauseAnims();

	void ResumeAnims();

	void ResetAnims();

	void UpdateAnims();

	void ManageAnimation(animation *managed_anim);
	void UnManageAnimation(animation *managed_anim);
};
class animation
{
	// Для первого типа анимаций текстуры полностью лежат в image и загружаются туда через интерфейс image
	// Смена кадров это смена image'ов.
	//TODO: глянуть потом этот тип анимации
	//std::vector< image * > Textures;

	image * CurrentTexture;

	// Для второго типа анимаций текстуры лежат в одной из того же вектора(только там одна текстура)
	// Смена кадров это смена кусков текстуры
	std::vector< PE_Rect > frames;

	// Текущий фрейм анимации
	uint CurrentFrame;

	// Таймер для тиков и подсчёта
	Uint32 SpeedTicks;

	// Скорость анимации
	Uint32 AnimSpeed;

	// Остановлена или нет
	bool Paused;

	// Закончена или нет
	bool AnimOver;

	// Количество повторов анимации для статики
	// У динамики выставляется значение -1, чтобы не прерывать анимации по событиям
	int Repeats;

	std::string ItemName;

public:
	animation();
	~animation();

	// Удаление анимации
	void Delete();

	// Паузим и возобновляем по необходимости анимацию
	void Pause();
	void Resume();

	// Сбрасываем тики и текущий фрейм
	void Reset();

	// Обновляем фрейм(каждый фрейм при событии)
	void Update();

	// Отрисовываем текущий фрейм анимации
	// Нужно лишь задать точку в которой начинается отрисовка
	void Draw(float x, float y, GLfloat Scale = 1, GLfloat Rotation = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Очищаем массив фреймов
	void ClearFrames();

	// Устанавливаем главную текстуру
	void SetTexture(image *Texture);

	// Устанавливаем скорость анимации
	void SetSpeed(Uint32 Speed);

	// Устанавливаем количество повторов, после которых не проигрываем анимацию
	void SetRepeats(int rep);

	void SetItemName(std::string name);

	// Добавляем новый фрейм в индекс или в конец
	void AddNewFrame(PE_Rect, int index = -1);

	// Прыгаем на фрейм с заданным индексом
	void JumpToFrame(unsigned int index);


	// Пауза
	bool IsPaused();

	bool IsOver();

	// Получаем главную текстуру
	image *GetTexture();

	// Получаем фрейм с индексом
	PE_Rect GetFrame(unsigned int index);

	// Получаем номер текущего фрейма
	int GetTrackNumber();

	// Получаем количество повторов, которые остались
	int GetRepeats();

	// Получаем скорость анимации
	Uint32 GetSpeed();

	// Получаем размер массива фреймов
	uint GetFramesSize();

	std::string GetItemName();

	//int anim_type; // Тип анимации
};
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

	// Обновить
	int Update();

	int handle_event(SDL_Event event);

	engine *Engine;
};

class collision_AABB
{
	// AABB бокс столкновений
public:
	collision_AABB();
	~collision_AABB();

	// Проверка столкновения
	bool OverlapsAABB(collision_AABB aabb);
	bool OverlapsOBB(collision_OBB obb);

	void SetAABBBox(PE_Rect NewBox);

	// Положение тела
	PE_Rect AABBBodyBox;
	PE_Point MinPoint, MaxPoint;
};
class collision_OBB
{
	// OBB бокс столкновений
public:
	collision_OBB();
	~collision_OBB();

	// Проверка столкновения
	//bool OverlapsAABB(collision_AABB aabb);
	//bool OverlapsOBB(collision_OBB obb);

	// Положение тела
	PE_Rect OBBBodyBox;

};
class collision_body: public collision_OBB, public collision_AABB
{
	// Тело столкновений - по-сути коробка, содержащаяся в слоях(слое) столкновений
	friend class collision;
	friend class collision_layer;

	// Указатель на класс столкновений
	collision *Collision;

	// Вектор указателей на все слои в которые входит(пересекается) данное тело
	std::vector< collision_layer *> CurrentLayers;

	// Проходимо ли тело
	unsigned int CollisionPass;

	// Тип столкновения
	int CollisionType;

	// Тип рамки столкновения
	int BoundingVolumeType;

public:
	collision_body(int ColPass = COLLISION_UNPASSABLE, int ColType = COLLISION_OUTSIDE, int BVType = COLLISION_AABB);
	~collision_body();

	// Устанавливем слой
	void AddNewLayer(collision_layer *layer);

	// Удаляем слой
	void RemoveLayer(unsigned int LayerID = -1);

	// Удаляем все слои
	void ClearLayers();

	// Проверяем содержание в слоях
	void UpdateLayers();

	bool CheckCollision();

	// Устанавливаем тип столкновения
	void SetCollisionType(int Type);

	// Устанавливаем тип столкновения
	void SetCollisionPass(unsigned int Pass);

	// Устанавливаем тип коробки
	void SetBoundingVolumeType(int BVType);

	// Получаем тип тела
	unsigned int IsPassable();

	// Получаем тип столкновения
	int GetCollisionType();

	// Получаем тип коробки столкновений
	int GetBoundingVolumeType();

	// Получаем определённый слой столкновений
	collision_layer *GetCollisionLayer(unsigned int LayerID = 0);

	// Получаем размер вектора слоёв
	int GetLayersSize();
};
class collision_layer
{
	// Класс слоя столкновений хранит тела, которые могут сталкиваться в ограниченной области
	friend class collision;

	// Указатель на класс столкновений
	collision *Collision;

	// Тела в текущем слое
	std::vector< collision_body *> bodies;

	// Прямоугольная рамка слоя
	PE_Rect LayerBorder;

	// Оптимизируем систем столкновений - сортировка, удаление "лишних" тел столкновения
	void OptimizeCollisions();
	void SortCollisions();

public:
	collision_layer(PE_Rect Box);
	collision_layer(GLfloat W = SYS_WIDTH, GLfloat H = SYS_HEIGTH, GLfloat X = 0, GLfloat Y = 0);
	~collision_layer();

	// Добавляем тело в слой
	void AddCollisionBody(collision_body *body);

	// Проверяем где именно находится заданное тело, относительно слоя
	int CheckBodyInLayer(collision_body *body);

	// Проверяем столкновение тела с телами из слоя
	bool CheckCollision(collision_body *body);

	// Удаляем тело из слоя
	void EraseBody(collision_body *body);

	// Функции полного удаления тел
	void DeleteAll();
	void DeleteCollisionBody(collision_body *body);

	// Задаём границы слоя
	void SetLayerBorder(PE_Rect Border);

	// Получаем границы слоя
	PE_Rect GetLayerBorder();
};
class collision
{
	// Класс, хранящий все слои столкновений и управляющий всей системой столкновений

	// Все тела в системе столкновений
	std::vector< collision_layer *> layers;

#ifdef DEBUGGING
	// Включена ли система
	bool IsEnabled;
#endif

public:
	collision();
	~collision();

	// Инициализация системы
	int init();

	// Добавляем новое тело в систему столкновений
	collision_body *NewCollisionBody(unsigned int LayerId, int ColPass = COLLISION_UNPASSABLE, int ColType = COLLISION_OUTSIDE, int BVType = COLLISION_AABB);

	// Добавляем новый слой в систему столкновений
	collision_layer *NewCollisionLayer(GLfloat W = SYS_WIDTH, GLfloat H = SYS_HEIGTH, GLfloat X = 0, GLfloat Y = 0);

	// Добавляем тело в слой с указанным id
	void AddBodyToLayer(collision_body *body, unsigned int LayerId);

	// Удаляем тело из слоя с указанным id
	void EraseBodyFromLayer(collision_body *body, unsigned int LayerId);

	// Очищаем выбранный слой
	void EraseCollisionLayer(unsigned int LayerId);
	void EraseCollisionLayer(collision_layer *layer);

	// Общая функция проверки столкновений двух прямоугольников - является базовой для многих функций проверки столкновений в других классах системы
	static bool CheckCollision(PE_Rect A, PE_Rect B);

	// Ищем слои для тела и добавляем их
	void NewLayersInBody(collision_body *body);

	// Удаление столкновений из системы
	void DeleteAll();

#ifdef DEBUGGING
	// Включение/выключение системы столкновений
	void SwitchCollision();
#endif
};

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


// TODO: нереализованные классы:
class button
{
	// Класс кнопки и обработка нажатия
};
class memory_manager
{
	// Общий менеджер памяти
};

#endif /* ENGINE_H_ */
