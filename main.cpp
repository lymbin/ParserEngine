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
	Graphics->ClearScreen();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glPushMatrix();

    //Graphics->ClearColor();
    //Graphics->DrawFilledRectangle(10, 10, 200, 100, 0.0f, 0.0f, 1.0f, 1.0f);
   // Graphics->DrawFilledRectangle(10, 110, 200, 100, 1.0f, 0.0f, 0.0f, 1.0f);


    if(Mmenu.background)
    {
    	PE_Rect Section;
    	Section.X = 0;
    	Section.Y = 0;
    	Section.Width = 1024;
    	Section.Heigth = 300;

    	Mmenu.background->Draw(0.0, 0.0, 0.2);
    	//Mmenu.background->Draw(0, 300, &Section);

    	Section.X = 0;
    	Section.Y = 300;
    	Section.Width = 300;
    	Section.Heigth = 400;
    	//Mmenu.background->Draw(0.0, 0.0);
    	//Mmenu.background->Draw(500, 200, &Section, 1.4, 180);
    }
/*
    if(Mmenu.title)
    {
    	Mmenu.title->Write(700, 0);
    	Mmenu.title->Write(700, Mmenu.title->textFont->CalcTextHeigth(Mmenu.title->textString), 20);
    }
*/

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

	Graphics->ClearScreen();
	Graphics->SetColor(0.f, 0.f, 0.f, 1.f);
/*
    if(Mmenu.background)
    	//Mmenu.background->Draw(0.0, 0.0, 800, 600);
    	Mmenu.background->Draw(500, 0);
*/

	Graphics->SwapBuffers();

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
		Graphics->SwapBuffers();
		SDL_Delay(100);

		if(Mmenu.title)
		{
			delete Mmenu.title;
			Mmenu.title = 0;
		}

	}
	FreeTextures();
}

int game::LoadTextures()
{
	//Загружаем текстуры для дальнейшей работы с ними

	Mmenu.background = new image("data/graphics/test/test1.png");
	//Mmenu.background = new image("foo.png");
	Mmenu.title = new text("FireFly", "data/fonts/non-free/Minecraftia.ttf", 30);
	//Mmenu.background->SetColorKey();
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
	Mmenu.background = 0;
	if(Mmenu.title)
		delete Mmenu.title;
	Mmenu.title = 0;
}
game::game()
{
	Mmenu.background = 0;
	Mmenu.title = 0;
	Gui = 0;
	Hero = 0;
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}



