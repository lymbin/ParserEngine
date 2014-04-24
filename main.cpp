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
void game::render()
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, Mmenu.background->GetTXT().tex);
    glLoadIdentity();
    if(Mmenu.background)
    	//Mmenu.background->Draw(0.0, 0.0, 800, 600);
    	Mmenu.background->Draw(1024, 1024, 0, 0, 300, 300, 0, 0, 300, 300);

    if(Mmenu.background)
    	//Mmenu.background->Draw(0.0, 0.0, 800, 600);
    	Mmenu.background->Draw(1024, 1024, 0, 300, 300, 300, 310, 0, 300, 300);

    if(Mmenu.title)
    	Mmenu.title->Write(700, 0, 0);

	SDL_GL_SwapBuffers();

	fps.start();
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
		render();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}
	FreeTextures();
}

int game::LoadTextures()
{
	//Загружаем текстуры для дальнейшей работы с ними

	Mmenu.background = new image("data/graphics/test/test1.png");
	Mmenu.title = new text("FireFly", "data/fonts/non-free/Minecraftia.ttf", 30);
	//Mmenu.title = new text("Кириллица", "data/fonts/PTS55F.ttf", 30);
	//Mmenu.button_start = new image();
	//Mmenu.button_exit = new image();

	//Mmenu.background->Open();

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
	Mmenu.background = 0;
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}
