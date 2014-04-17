/*
 * engine.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

bool engine::quit = false;

void engine::MainLoop()
{
	//Главный цикл приложения
	while(!engine::quit)
	{
		while( SDL_PollEvent(&event))
		{
			//Ищем событие на закрытие окна
			if( event.type == SDL_QUIT)
			{
				//Выходим
				quit = true;
			}
		}
	}
}
int engine::init()
{
	//TODO: доделать инициализацию без аудио
	//Инициируем все системы и создаём элементы движка
	if(SDL_Init(SDL_INIT_EVERYTHING)<0) //Инициализация SDL
	{
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
		return -1;
	}

	//Задаём двойную буфферизацию
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_SetVideoMode(SYS_WIDTH, SYS_HEIGTH, SYS_BPP, SDL_OPENGL|SDL_RESIZABLE);
	if(!screen)
	{
		cout << "Unable to set screen: " << SDL_GetError() << endl;
	}

	//Задаём текст для заголовка окна
	std::stringstream title;
	title << "FireFly project " << SYS_VERSION << " prealpha test. Build " << SYS_BUILD << ".";
	SDL_WM_SetCaption(title.str().c_str(), "test icon");

	//Забиваем графику
	Graphics = new graphics();
	Graphics->init();


	cout << "All system initialization - success" << endl;
	cout << "Engine initialization - success" << endl;
	return 0;
}
void engine::CleanUp()
{
	//Освобождаем ненужную память
	if(screen)
		SDL_FreeSurface(screen);

	SDL_Quit(); //Выходим
}
engine::engine()
{
	//Конструктор
	screen = 0;
	Graphics = 0;

}
engine::~engine()
{
	//Деструктор

}

int graphics::init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SYS_WIDTH, SYS_HEIGTH, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cout << "Graphics initialization - success" << endl;
	return 0;
}
void graphics::CleanUp()
{

}
graphics::graphics()
{

}
graphics::~graphics()
{

}
