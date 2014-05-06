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

	//Забиваем графику
	Graphics = new graphics();
	if(Graphics->init() < 0)
		return -1;

#ifdef DEBUG_SYS
	cout << "All system initialization - success" << endl;
	cout << "Engine initialization - success" << endl;
#endif
	return 0;
}
void engine::CleanUp()
{
	//Освобождаем ненужную память
	if(Graphics)
		delete Graphics;
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
	Graphics = 0;
	Audio = 0;
	//Другие компоненты
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
