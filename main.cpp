/*
 * main.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

int main(int argc, char *argv[])
{
	game *Game = new game();

	if(Game->init()<0)
	{
		delete Game;
		return 0;
	}
#ifdef DEBUG_SYS
	cout << "Game initialization - success" << endl;
#endif

	Game->MainLoop();


	Game->CleanUp();
	delete Game;

	return 0;
}
bool game::quit = false;

void game::MainLoop()
{
#ifdef DEBUG_SYS
	cout << "Game start!" << endl;
#endif
	//Главный цикл приложения
	while(!game::quit)
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
game::game()
{
	//engine *Engine = new engine();
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}
