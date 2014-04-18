/*
 * main.cpp
 *
 *  Created on: 17.04.2014
 *      Author: dmitry
 */

#include "game.h"

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

void game::update()
{

}
void game::MainLoop()
{
#ifdef DEBUG_SYS
	cout << "Load textures" << endl;
#endif
	LoadTextures();
#ifdef DEBUG_SYS
	cout << "Game start!" << endl;
#endif
	//Главный цикл приложения
	while(!game::quit)
	{
		//При каждом цикле сначала обрабатываем сигналы
		while( SDL_PollEvent(&event))
		{
			//Ищем событие на закрытие окна
			if( event.type == SDL_QUIT)
			{
				//Выходим
				quit = true;
			}
		}
		//Здесь уже сама игра
		update();



	}
	FreeTextures();
}

int game::LoadTextures()
{
	//Загружаем текстуры для дальнейшей работы с ними

	Mmenu.background = new image();
	//Mmenu.button_start = new image();
	//Mmenu.button_exit = new image();

	Mmenu.background->Open("data/graphics/test/test.png");


	return 0;
}
void game::FreeTextures()
{
#ifdef DEBUG_SYS
	cout << "Freeing textures" << endl;
#endif

	if(Mmenu.background)
		delete Mmenu.background;
}
game::game()
{
	//engine *Engine = new engine();
	Mmenu.background = 0;
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}
