/*
 * engine.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

int engine::init()
{
	//TODO: доделать инициализацию без аудио
	//Инициируем все системы и создаём элементы движка
	if(SDL_Init(SDL_INIT_EVERYTHING)<0) //Инициализация SDL
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
#endif
		return -1;
	}

	//Задаём двойную буфферизацию
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_SetVideoMode(SYS_WIDTH, SYS_HEIGTH, SYS_BPP, SDL_OPENGL|SDL_RESIZABLE);
	if(!screen)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set screen: " << SDL_GetError() << endl;
#endif
	}

	//Задаём текст для заголовка окна
	std::stringstream title;
	title << "FireFly project " << SYS_VERSION << " prealpha test. Build " << SYS_BUILD << ".";
	SDL_WM_SetCaption(title.str().c_str(), "test icon");

	//Забиваем графику
	Graphics = new graphics();
	Graphics->init();

#ifdef DEBUG_SYS
	cout << "All system initialization - success" << endl;
	cout << "Engine initialization - success" << endl;
#endif
	return 0;
}
void engine::CleanUp()
{
	//Освобождаем ненужную память
	if(screen)
		SDL_FreeSurface(screen);
	screen = 0;

	if(Graphics)
	{
		Graphics->CleanUp();
		delete Graphics;
	}
	Graphics = 0;

#ifdef DEBUG_SYS
	cout << "Engine clean up - success" << endl;
#endif

	SDL_Quit(); //Выходим

#ifdef DEBUG_SYS
	cout << "SDL Quit!" << endl;
#endif

}
engine::engine()
{
	//Конструктор
	screen = 0;
	Graphics = 0;
	//Другие компоненты
	//
	//
	//
	//
}
engine::~engine()
{
	//Деструктор
#ifdef DEBUG_SYS
	cout << "Engine completely clean up - success" << endl;
#endif
}

int graphics::init()
{
	//Инициализация шрифтов
	if(font::FontInit() < 0)
		return -1;

	//Инициализация OpenGL
	glClearColor( 0, 0, 0, 0 );
	glClearDepth(1.0f);

	glViewport(0, 0, SYS_WIDTH, SYS_HEIGTH);

	//Инициируем матрице проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SYS_WIDTH, SYS_HEIGTH, 0.0, -1.0, 1.0);

	//Инициирцуем матрицу вида
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	//Включаем альфа-канал и блендинг
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

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
	if(TTF_WasInit())
		TTF_Quit();
}

void graphics::ResizeWin(int width, int heigth)
{
	//Изменяем рамер окна
	//TODO: доделать тут
	GLfloat ratio;
	if(!heigth) heigth = 1;

	ratio = (GLfloat) width/(GLfloat)heigth;
	glViewport(0, 0, (GLsizei) width, (GLsizei) heigth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.5f, ratio, 0.1f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Другая версия - пока тестовая
	/*
	if(!heigth) heigth = 1;
	screen = SDL_SetVideoMode(SYS_WIDTH, SYS_HEIGTH, SYS_BPP, SDL_OPENGL|SDL_RESIZABLE);
	if(!screen)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Resize win " << SDL_GetError() << endl;
#endif
		exit(1);
	}

	//Инициализация всех возможностей OpenGL
	InitGL();

	//Перезагружаем все хранящиеся в памяти текстуры
	 * из менеджера текстур
	 * TODO: нужно переделать менеджер и задать его глобально или через синглетон
	ReloadTextures();


	 */
}
// Очистка экрана
void graphics::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Отрисовываем буферы на экране
void graphics::SwapBuffers()
{
	SDL_GL_SwapBuffers();
}
graphics::graphics()
{

}
graphics::~graphics()
{
#ifdef DEBUG_SYS
	cout << "Graphics clean up - success" << endl;
#endif
}


