/*
 * graphics.cpp
 *
 *  Created on: 29.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

int graphics::init()
{
	// Инициализация основных компонентов графики

	// Инициализация SDL-графики
	screen = SDL_SetVideoMode(SYS_WIDTH, SYS_HEIGTH, SYS_BPP, SDL_OPENGL|SDL_RESIZABLE);
	if(!screen)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set screen: " << SDL_GetError() << endl;
#endif
		return -1;
	}

	// Задаём двойную буфферизацию
		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Задаём текст для заголовка окна
	std::stringstream title;
	title << "FireFly project " << SYS_VERSION << " prealpha test. Build " << SYS_BUILD << ".";
	SDL_WM_SetCaption(title.str().c_str(), "test icon");


	// Инициализация шрифтов
	if(font_manager::FontsInit() < 0)
		return -1;

	// Инициализация OpenGL
	if(initGL() < 0)
		return -1;

	return 0;
}
int graphics::initGL()
{
	glViewport(0.0f, 0.0f, SYS_WIDTH, SYS_HEIGTH);

	//Инициируем матрице проекции
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SYS_WIDTH, SYS_HEIGTH, 0.0, -1.0, 1.0);

	//Инициирцуем матрицу вида
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	glPushMatrix();

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClearDepth(1.0f);

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

	if(screen)
		SDL_FreeSurface(screen);
	screen = 0;

	if(TextureManager)
		delete TextureManager;

	if(FontManager)
		delete FontManager;

	TextureManager = 0;
	FontManager = 0;
	CurrentTexture = 0;
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
void graphics::ToggleFullScreen()
{
	// Переключаем полноэкранный режим и включаем мышь
	// TODO: работает но с натягом - доделать
	FullScreen = !FullScreen;
	if(screen)
	{
		if(!SDL_WM_ToggleFullScreen(screen))
		{
#ifdef DEBUG_ERRORS
		cout << "Error toggle fullscreen: " << SDL_GetError() << endl;
#endif
			return;
		}
		if(!FullScreen)
			SDL_ShowCursor(SDL_ENABLE);
		else
			SDL_ShowCursor(SDL_DISABLE);

	}
	// TODO: записывать изменения в файл с настройками
}

// Рисуем каркас прямоугольника цветными линиями
void graphics::DrawRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
								GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	//TODO: доделать работу с камерой

	glColor4f(red, green, blue, alpha);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	glEnd();

}

// Рисуем заполненный цветом прямоугольник
void graphics::DrawFilledRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
									GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glLoadIdentity();

	glBegin(GL_QUADS);
		glColor4f(red, green, blue, alpha);
		glVertex3f(x, y, 0);
		glVertex3f(x + width, y, 0);
		glVertex3f(x + width, y + height, 0);
        glVertex3f(x, y + height, 0);
    glEnd();

    glLoadIdentity();
}

// Рисуем линию
void graphics::DrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
						GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glColor4f(red, green, blue, alpha);

	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

// Устанавливаем текущую забинженную текстуру
void graphics::SetCurrentTexture(GLuint texture)
{
	CurrentTexture = texture;
}
graphics::graphics()
{
	FontManager = new font_manager();
	FontManager->SetGraphics(this);

	TextureManager = new texture_manager();
	TextureManager->SetGraphics(this);
	FullScreen = SYS_FULLSCREEN;
	CurrentTexture = 0;
	screen = 0;
}
graphics::~graphics()
{
	CleanUp();

#ifdef DEBUG_SYS
	cout << "Graphics clean up - success" << endl;
#endif
}

