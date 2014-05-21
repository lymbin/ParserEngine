/*
 * game.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dmitry
 */

#include "game.h"

using namespace std;

bool game::quit = false;

somebox::somebox()
{
	body = new collision_body();
}
somebox::~somebox()
{

}

collision_body *somebox::GetCollisionBody()
{
	return body;
}

void game::update()
{
	// Обновляем все координаты объектов, текстов и прочего

	if(Hero)
	{
		Hero->update();
	}
}
void game::render()
{
	// Отрисовываем все объекты

	Graphics->ClearScreen();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glPushMatrix();

    //Graphics->ClearColor();
    //Graphics->DrawFilledRectangle(10, 10, 200, 100, 0.0f, 0.0f, 1.0f, 1.0f);
   // Graphics->DrawFilledRectangle(10, 110, 200, 100, 1.0f, 0.0f, 0.0f, 1.0f);

	if(Hero)
	{
		Hero->render();
	}
/*
    if(Mmenu.background)
    {
    	PE_Rect Section;
    	Section.X = 0;
    	Section.Y = 0;
    	Section.Width = 1024;
    	Section.Heigth = 300;

    	Mmenu.background->Draw(0.0, 100.0, 0.2);
    	//Mmenu.background->Draw(0, 300, &Section);

    	Section.X = 0;
    	Section.Y = 300;
    	Section.Width = 300;
    	Section.Heigth = 400;
    	//Mmenu.background->Draw(0.0, 0.0);
    	//Mmenu.background->Draw(500, 200, &Section, 1.4, 180);
    }
    */
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
	LoadTextures();
	CreatingObjects();

	Graphics->ClearScreen();
	Graphics->SetColor(0.f, 0.f, 0.f, 1.f);
	Graphics->SwapBuffers();

	fps.start();

#ifdef DEBUG_SYS
	cout << "Game start!" << endl;
#endif

	text *help_text = new text("hlp", "data/fonts/non-free/Minecraftia.ttf", 16);

#ifdef DEBUG_INFOS
	font *fps_font = new font("data/fonts/non-free/Minecraftia.ttf", 16);
	std::string fps_rate;
	GLuint tex = 0;

#endif
	std::stringstream sstream;
	bool simple_menu = true;
	//Главный цикл приложения
	while(!game::quit)
	{
		sstream.str(string());
		Graphics->ClearScreen();
		Graphics->SetColor(0.f, 0.f, 0.f, 1.f);

		if(Events->handle_events())
		{
			//Выходим
			quit = true;
			break;
		}

		Input->Update();

		if(Input->IsKeyDown(KEY_ESCAPE)|| Input->IsKeyHeld(KEY_ESCAPE))
		{
			//Выходим
			quit = true;
			break;
		}

		if(simple_menu)
		{
			if(Input->IsKeyDown(KEY_1) || Input->IsKeyHeld(KEY_1))
			{
				// Входим в игру
				simple_menu = false;
			}
			else if(Input->IsKeyDown(KEY_2)|| Input->IsKeyHeld(KEY_2))
			{
				//Выходим
				quit = true;
			}
		}
		else
		{
			//Здесь уже сама игра
			update();
			render();
		}


		if(simple_menu)
		{
			help_text->ResizeText(18);
			// Отрисовываем текст в центре
			help_text->SetText("Press 1 to Play demo");
			help_text->Write(Graphics->GetScreenWidth()/2 - help_text->GetFont()->CalcTextWidth(help_text->GetText())/2,
							Graphics->GetScreenHeigth()/2 - help_text->GetFont()->CalcTextHeigth(help_text->GetText())/2);

			// Отрисовываем текст чуть ниже центра
			help_text->SetText("Press 2 to Exit demo");
			help_text->Write(Graphics->GetScreenWidth()/2 - help_text->GetFont()->CalcTextWidth(help_text->GetText())/2,
							Graphics->GetScreenHeigth()/2 - help_text->GetFont()->CalcTextHeigth(help_text->GetText())/2 + help_text->GetFont()->CalcTextHeigth(help_text->GetText()));

		}
		else
		{
			help_text->ResizeText(16);
			// Отрисовываем текст в центре
			help_text->SetText("Press ESC to Exit");
			help_text->Write(Graphics->GetScreenWidth()/2 - help_text->GetFont()->CalcTextWidth(help_text->GetText())/2,
							help_text->GetFont()->CalcTextHeigth(help_text->GetText()));

			help_text->SetText("Press Arrows to Move");
			help_text->Write(Graphics->GetScreenWidth()/2 - help_text->GetFont()->CalcTextWidth(help_text->GetText())/2,
							help_text->GetFont()->CalcTextHeigth(help_text->GetText())*2);

		}


		sstream << "PreAlpha ParserEngine Demo. Version " << SYS_TEST_VERSION << ".";
		help_text->SetText(sstream.str());
		help_text->Write(Graphics->GetScreenWidth() - help_text->GetFont()->CalcTextWidth(help_text->GetText()) - SYS_FRAME_PIXELS,
						Graphics->GetScreenHeigth() - help_text->GetFont()->CalcTextHeigth(help_text->GetText()) - SYS_FRAME_PIXELS);

		sstream.str(string());
		// Выводим FPS поверх игры
#ifdef DEBUG_INFOS
		frame++;

		if(tex)
		{
			glDeleteTextures(1, &tex);
			tex = 0;
		}
		//std::stringstream sstream;

		if(fps.get_ticks() > 1000)
		{
			// Обезопасим от деления на нуль
			sstream << " FPS: " << frame / (fps.get_ticks() / 1000) ;
		}
		else
		{
			sstream << " FPS: 0";
		}

		fps_font->Write(sstream.str(), Graphics->GetScreenWidth() - fps_font->CalcTextWidth(sstream.str()) - SYS_FRAME_PIXELS, 0 + SYS_FRAME_PIXELS, &tex);
#endif

		Graphics->SwapBuffers();
		SDL_Delay(10);
	}
#ifdef DEBUG_INFOS
	delete fps_font;
#endif
	delete help_text;
	FreeTextures();
	FreeObjects();
}

int game::CreatingObjects()
{
#ifdef DEBUG_SYS
	cout << "Creating objects" << endl;
#endif

	if(Collision)
	{
		layer = Collision->NewCollisionLayer(Graphics->GetScreenWidth(), Graphics->GetScreenHeigth(), 0, 0);
	}

	if(!Hero)
	{
		Hero = new hero();
		Hero->SetTexture(Mmenu.background);
		Hero->SetGame(this);
		Hero->SetStaticSpeed(20);

		if(layer)
		{
			layer->AddCollisionBody(Hero->GetCollisionBody());
			Hero->GetCollisionBody()->AddNewLayer(layer);
		}
	}
	if(!Gui)
	{
		Gui = new game_gui();
	}

	return 0;
}

//Загружаем текстуры для дальнейшей работы с ними
int game::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << "Load textures" << endl;
#endif
	Mmenu.background = new image("data/graphics/test/test.png");
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
void game::FreeObjects()
{
#ifdef DEBUG_SYS
	cout << "Freeing objects" << endl;
#endif
	if(Hero)
	{
		delete Hero;
	}
	if(Gui)
	{
		delete Gui;
	}
	if(layer)
	{
		delete layer;
	}
}
game::game()
{
	Mmenu.background = 0;
	Mmenu.title = 0;
	Gui = 0;
	Hero = 0;
	layer = 0;
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}



game_gui::game_gui()
{
	bottom.dynamic_text = 0;
	bottom.static_text = 0;
	bottom.texture = 0;
}
game_gui::~game_gui()
{
	if(bottom.dynamic_text)
	{
		delete bottom.dynamic_text;
		bottom.dynamic_text = 0;
	}
	if(bottom.static_text)
	{
		delete bottom.static_text;
		bottom.static_text = 0;
	}
	if(bottom.texture)
	{
		delete bottom.texture;
		bottom.texture = 0;
	}
}
void game_gui::update()
{

}
void game_gui::render()
{
	if(bottom.texture)
	{
		bottom.texture->Draw(SYS_HEIGTH - GAME_GUI_HEIGTH, 0);
	}
}
