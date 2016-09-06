/*
 * graphics.cpp
 *
 *  Created on: 29.04.2014
 *      Author: dmitry
 */

#include "Graphics.h"

using namespace std;

graphics::graphics()
{
	mlScreenWidth = 0;
	mlScreenHeigth = 0;
	mlScreenBpp = 0;
	mbFullScreen = 0;

	mpFontManager = new cFontManager();
	mpFontManager->SetGraphics(this);

	mpTextManager = new cTextManager();
	mpTextManager->SetGraphics(this);

	mpTextureManager = new texture_manager();
	mpTextureManager->SetGraphics(this);

	CurrentTexture = 0;
	sdlWindow = 0;
	sdlRenderer = 0;

	Camera = new camera();
	_Window = new window(this);
}
graphics::~graphics()
{
	CleanUp();

#ifdef DEBUG_SYS
	cout << "Graphics clean up - success" << endl;
#endif
}

int graphics::Init(int W, int H, int BPP, int abFullScreen)
{
	// Инициализация основных компонентов графики
	mlScreenWidth = W;
	mlScreenHeigth = H;
	mlScreenBpp = BPP;
	mbFullScreen = abFullScreen;

	// Инициализация SDL-графики
	// Задаём текст для заголовка окна
	std::stringstream title;
	title << "FireFly project " << SYS_VERSION << " prealpha test. Build " << SYS_BUILD << ".";
	sdlWindow = SDL_CreateWindow (title.str().c_str(),
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					mlScreenWidth,
					mlScreenHeigth,
					SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!sdlWindow)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set window: " << SDL_GetError() << endl;
#endif
		return -1;
	}

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

	if(!sdlRenderer)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set renderer: " << SDL_GetError() << endl;
#endif
		return -1;
	}

	// Задаём двойную буфферизацию
		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Инициализация шрифтов
	if(cFontManager::FontsInit() < 0)
		return -1;

	// Инициализация OpenGL
	if(InitGL() < 0)
		return -1;

	return 0;
}
int graphics::InitGL()
{
	ClearScreen();
	// Система координат - от точки (0,0) с размером SYS_WIDTHxSYS_HEIGTH
	glViewport(0.0f, 0.0f, mlScreenWidth, mlScreenHeigth);

	//Инициируем матрице проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливаем максимум и минимум в системе координат по разным осям
	glOrtho(0.0, mlScreenWidth, mlScreenHeigth, 0.0, -1.0, 1.0);

	//Инициирцуем матрицу вида
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);	// Включаем текстурирование
	glLoadIdentity();

	// Cохраняем матрицу вида
	glPushMatrix();

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClearDepth(1.0f);


	//Включаем альфа-канал и блендинг
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glDisable(GL_LIGHTING);


	//Проверка на ошибки
	GLenum error = glGetError();
	if(error!=GL_NO_ERROR)
	{
#ifdef DEBUG_ERRORS
		cout << "Error initializing OpenGL: " << gluErrorString(error) << endl;
#endif
		return -1;
	}

#ifdef DEBUG_SYS
	cout << "Graphics initialization - success" << endl;
#endif

	return 0;
}
void graphics::CleanUp()
{
	if(sdlWindow)
		SDL_DestroyWindow(sdlWindow);
	sdlWindow = 0;

	if(mpTextureManager)
		delete mpTextureManager;

	if(mpTextManager)
		delete mpTextManager;

	if(mpFontManager)
		delete mpFontManager;

	if(Camera)
		delete Camera;

	Camera = 0;
	mpTextureManager = 0;
	mpFontManager = 0;
	mpTextManager = 0;
	CurrentTexture = 0;
}
// Изменяем рамер окна
void graphics::ResizeWin(int width, int heigth)
{
	// Обезопасиваем от деления на ноль
	if(!heigth) heigth = 1;

	// Устанавливаем новые параметры окна
	mlScreenWidth = width;
	mlScreenHeigth = heigth;

	// Устанавливаем новый видео режим
	//screen = SDL_SetVideoMode(mlScreenWidth, mlScreenHeigth, mlScreenBpp, SDL_OPENGL|SDL_RESIZABLE);
	ClearScreen();

	// Выходим из программы, если не можем изменить размеры окна
	if(!sdlWindow)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Resize win " << SDL_GetError() << endl;
#endif
		exit(1);
	}

	glPopMatrix();

	// Система координат - от точки (0,0) с размером SYS_WIDTHxSYS_HEIGTH
	glViewport(0.0f, 0.0f, mlScreenWidth, mlScreenHeigth);

	//Инициируем матрице проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливаем максимум и минимум в системе координат по разным осям
	glOrtho(0.0, mlScreenWidth, mlScreenHeigth, 0.0, -1.0, 1.0);

	//Инициирцуем матрицу вида
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);	// Включаем текстурирование
	glLoadIdentity();

	// Cохраняем матрицу вида
	glPushMatrix();

#ifdef DEBUG_SYS
	cout << "Resizing successful! New size: " << width << ":" << heigth << endl;
#endif

}
// Очистка экрана
void graphics::ClearScreen()
{
	if(!sdlRenderer)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set renderer: " << SDL_GetError() << endl;
#endif
		return;
	}
	//SDL_RenderClear(sdlRenderer);
	//SDL_RenderPresent(sdlRenderer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Очищаем цвет
void graphics::ClearColor()
{
	if(!sdlRenderer)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set renderer: " << SDL_GetError() << endl;
#endif
		return;
	}
	//SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	glClearColor(0,0,0,0);
	//SetClearColor(cColor(1.0f, 1.0f));
}
// Устанавливаем цвет
void graphics::SetClearColor(cColor aCol)
{
	if(!sdlRenderer)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set renderer: " << SDL_GetError() << endl;
#endif
		return;
	}
	//SDL_SetRenderDrawColor(sdlRenderer, aCol.r, aCol.g, aCol.b, aCol.a);
	glColor4f(aCol.r, aCol.g, aCol.b, aCol.a);
}
void graphics::PushMatrix()
{
	glPushMatrix();
}
void graphics::PopMatrix()
{
	glPopMatrix();
}
// Отрисовываем буферы на экране
void graphics::SwapBuffers()
{
	SDL_GL_SwapWindow(sdlWindow);
}
void graphics::ToggleFullScreen()
{
	// Переключаем полноэкранный режим и включаем мышь
	// TODO: работает но с натягом - доделать
	mbFullScreen = !mbFullScreen;
	if(sdlWindow)
	{
		if(!SDL_SetWindowFullscreen(sdlWindow, 0))
		{
#ifdef DEBUG_ERRORS
		cout << "Error toggle fullscreen: " << SDL_GetError() << endl;
#endif
			return;
		}
		if(!mbFullScreen)
			SDL_ShowCursor(SDL_ENABLE);
		else
			SDL_ShowCursor(SDL_DISABLE);

	}
	// TODO: записывать изменения в файл с настройками
}
// Устанавливаем текущую забинженную текстуру
void graphics::SetCurrentTexture(GLuint texture)
{
	CurrentTexture = texture;
}
SDL_Window *graphics::Screen()
{
	return sdlWindow;
}
GLuint graphics::GetCurrentTexture()
{
	return CurrentTexture;
}

int graphics::GetScreenWidth()
{
	return mlScreenWidth;
}
int graphics::GetScreenHeigth()
{
	return mlScreenHeigth;
}
int graphics::GetScreenBpp()
{
	return mlScreenBpp;
}
camera *graphics::GetCamera()
{
	return Camera;
}
window *graphics::GetWindow()
{
	return _Window;
}
texture_manager *graphics::GetTextureManager()
{
	return mpTextureManager;
}
cFontManager *graphics::GetFontManager()
{
	return mpFontManager;
}
cTextManager *graphics::GetTextManager()
{
	return mpTextManager;
}
