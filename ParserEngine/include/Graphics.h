/*
 * Graphics.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "SystemConstants.h"
#include "Texture.h"
#include "Font.h"
#include "GraphicTypes.h"
#include "Color.h"

class cTexture;
class texture_manager;
class cFontManager;
class cTextManager;
class camera;
class window;
class cColor;

class iLowLevelGraphics
{
public:
	iLowLevelGraphics();
	virtual ~iLowLevelGraphics();

	virtual int Init(int W, int H, int BPP, int abFullScreen)=0;

	virtual void ClearScreen()=0;
	virtual void SetClearColor(cColor aCol)=0;

	virtual void PushMatrix()=0;
	virtual void PopMatrix()=0;

	virtual void SwapBuffers()=0;

	// Рисуем примитивы
	// TODO: возможно вынести в одтельный класс
	// TODO: позаботиться об их удалении без очистки экрана
	// TODO: дать только некоторым классам использовать эти функции
	//		например, из примитивов рисовать что-то полезное внутри другого класса

	// Рисуем каркас прямоугольника цветными линиями
	void DrawRectangle(float x, float y, float width, float height, cColor aCol = cColor(1.0f, 1.0f));
	void DrawRectangle(PE_Rect aRect, cColor aCol = cColor(1.0f, 1.0f));

	// Рисуем заполненный цветом прямоугольник
	void DrawFilledRectangle(float x, float y, float width, float height, cColor aCol = cColor(1.0f, 1.0f));
	void DrawFilledRectangle(PE_Rect aRect, cColor aCol = cColor(1.0f, 1.0f));

	// Рисуем линию
	void DrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, cColor aCol = cColor(1.0f, 1.0f));
	void DrawLine(PE_Point A, PE_Point B, cColor aCol = cColor(1.0f, 1.0f));
};
class graphics: public iLowLevelGraphics
{
	friend cTexture;
	texture_manager *mpTextureManager;	// Менеджер текстур
	cFontManager 	*mpFontManager;
	cTextManager	*mpTextManager;

	SDL_Window 	*sdlWindow;		// Сурфейс окна
	GLuint CurrentTexture;	// Текущая забинженная текстура
	SDL_Renderer *sdlRenderer;

	camera *Camera;
	window *_Window;

	int mbFullScreen;

	int	mlScreenWidth; 	//ширина
	int	mlScreenHeigth;	//высота
	int	mlScreenBpp;	//палитра
public:
	graphics();
	~graphics();

	// Инициализируем все внутренние подсистемы
	int Init(int W = SYS_WIDTH, int H = SYS_HEIGTH, int BPP = SYS_BPP, int abFullScreen = SYS_FULLSCREEN);

	// Инициализация OpenGL
	int InitGL();

	// Очищаем всю графическую систему
	void CleanUp();

	// Очистка экрана
	void ClearScreen();

	// Очищаем цвет
	void ClearColor();

	// Устанавливаем цвет
	void SetClearColor(cColor aCol);

	void PushMatrix();
	void PopMatrix();

	// Отрисовываем буферы на экране
	void SwapBuffers();

	// Меняем размеры окна
	void ResizeWin(int win_dX, int win_dY);

	// Меняем полноэкранный режим
	void ToggleFullScreen();

	// Устанавливаем текущую забинженную текстуру
	void SetCurrentTexture(GLuint texture);

	SDL_Window *Screen();
	GLuint GetCurrentTexture();

	int GetScreenWidth();
	int GetScreenHeigth();
	int GetScreenBpp();

	camera *GetCamera();
	window *GetWindow();
	texture_manager *GetTextureManager();
	cFontManager 	*GetFontManager();
	cTextManager	*GetTextManager();
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

#endif /* GRAPHICS_H_ */
