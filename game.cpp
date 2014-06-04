/*
 * game.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dmitry
 */

#include "game.h"

using namespace std;

bool game::quit = false;

void game::update()
{
	// Обновляем все координаты объектов, текстов и прочего
	// TODO:Обновляем все Updatable объекты

	if(Hero)
	{
		Hero->Update();

		if(dynamic_text)
		{
			std::stringstream sstream;
			sstream << "Hero Speed: " << Hero->GetVelocity() << " Hero Health: " << Hero->GetHealth();
			dynamic_text->SetText(sstream.str());
		}
	}
	if(StaticBox)
	{
		StaticBox->Update();
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

	static GLuint tex = 0;
	if(Hero)
	{
		Hero->OnDraw();
	}
	if(dynamic_text)
	{
		PE_Rect ScreenRect;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = Graphics->GetScreenHeigth();
		ScreenRect.Width = Graphics->GetScreenWidth();
		dynamic_text->Write(ScreenRect, eTextAlignment_Left | eTextAlignment_Bottom);
	}
	if(StaticBox)
	{
		StaticBox->OnDraw();
		dynamic_text->GetFont()->Write("DEMO!", StaticBox->GetBox(), eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex);
	}
	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
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
	//Graphics->SetClearColor(cColor());
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
		//Graphics->SetClearColor(cColor());

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

		PE_Rect ScreenRect;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = Graphics->GetScreenHeigth();
		ScreenRect.Width = Graphics->GetScreenWidth();

		if(simple_menu)
		{
			help_text->ResizeText(18);




			// Отрисовываем текст в центре
			help_text->SetText("Press 1 to Play demo");

			cColor col(1.0f, 0.0f, 1.0f, 1.0f);
			Graphics->DrawFilledRectangle(
					Graphics->GetScreenWidth()/2 - help_text->GetTextWidth()/2,
					Graphics->GetScreenHeigth()/2 - help_text->GetTextHeigth()/2,
					help_text->GetTextWidth(), help_text->GetTextHeigth()*2, col);

			help_text->Write(ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V);

			// Отрисовываем текст чуть ниже центра
			help_text->SetText("Press 2 to Exit demo");
			help_text->Write(ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V, 0, help_text->GetTextHeigth());


		}
		else
		{
			help_text->ResizeText(16);

			// Отрисовываем текст в центре
			help_text->SetText("Press ESC to Exit");
			help_text->Write(ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top);

			help_text->SetText("Press Arrows to Move");
			help_text->Write(ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top, 0, help_text->GetTextHeigth());

		}


		sstream << "PreAlpha ParserEngine Demo. Version " << SYS_TEST_VERSION << ".";
		help_text->SetText(sstream.str());
		help_text->Write(ScreenRect, eTextAlignment_Right | eTextAlignment_Bottom);

		sstream.str(string());
		// Выводим FPS поверх игры
#ifdef DEBUG_INFOS
		frame++;

		if(tex)
		{
			glDeleteTextures(1, &tex);
			tex = 0;
		}
		std::stringstream sstream;

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
		layer = new iCollisionLayer(PE_Rect {0, 0, Graphics->GetScreenWidth(), Graphics->GetScreenHeigth()});
		//layer = Collision->NewCollisionLayer(Graphics->GetScreenWidth(), Graphics->GetScreenHeigth(), 0, 0);
		Collision->AddCollisionLayer(layer);
	}

	if(!Hero)
	{
		Hero = new hero();
		Hero->SetStaticTexture(Mmenu.background);
		Hero->SetGame(this);
		Hero->SetVelocity(20);
		Hero->SetScaledAndRotate(0.3, 0);

		if(Hero->GetTexture())
			Hero->SetBox(Hero->GetTexture()->GetWidth()*Hero->GetScaledMultiplier(), Hero->GetTexture()->GetHeigth()*Hero->GetScaledMultiplier(), 0, 100);

		if(layer)
		{
			layer->AddCollisionBody(Hero);
			Hero->AddNewLayer(layer);

			Hero->SetCollisionsPointer(Collision);
			Collision->AddCollisionBody(Hero);
		}

	}
	if(!Gui)
	{
		Gui = new game_gui();
	}
	if(!dynamic_text)
	{
		dynamic_text = new text("", "data/fonts/non-free/Minecraftia.ttf", 14);
	}
	if(!StaticBox)
	{
		StaticBox = new cStaticBox("Box1", 100);
		StaticBox->SetGame(this);
		StaticBox->SetBox(100,100,200,200);
		if(layer)
		{
			layer->AddCollisionBody(StaticBox);
			StaticBox->AddNewLayer(layer);

			StaticBox->SetCollisionsPointer(Collision);
			Collision->AddCollisionBody(StaticBox);
		}
	}

	return 0;
}

//Загружаем текстуры для дальнейшей работы с ними
int game::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << "Load textures" << endl;
#endif
	Mmenu.background = new cTexture("data/graphics/test/test.png");
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
	if(StaticBox)
	{
		delete StaticBox;
	}
	delete Collision;
}
game::game()
{
	Mmenu.background = 0;
	Mmenu.title = 0;
	Gui = 0;
	Hero = 0;
	layer = 0;
	dynamic_text = 0;
	StaticBox = 0;
	Collision = new cCollision();
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
