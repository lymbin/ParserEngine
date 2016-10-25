/*
 * game.cpp
 *
 *  Created on: 08.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "game.h"

using namespace std;

bool game::quit = false;

game *game::self = 0;

void game::update()
{
	LevelManager()->Update();
	// Обновляем все координаты объектов, текстов и прочего
	// TODO: Обновляем все Updatable объекты

	/*
	if(Hero)
	{
		PE_Rect OldBox = Hero->GetBox();// Запоминаем старый бокс для случая столкновения с каким-либо телом и возвратом в исходное положение
		Hero->Update();
		Hero->CALLBACK(hero::CollisionHandler, Hero, &OldBox);
	}
	if (StaticBox)
		StaticBox->Update();

	if (Hero)
		Hero->HandleCollisions();
	 */
	PostUpdate();
}

void game::PostUpdate()
{
	LevelManager()->PostUpdate();
	/*
	if(Hero && Hero->GetHealth() > 0)
	{
		Hero->PostUpdate();
		StaticBox->PostUpdate();
	}*/
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
    //Graphics->DrawFilledRectangle(10, 110, 200, 100, 1.0f, 0.0f, 0.0f, 1.0f);

	LevelManager()->Render();

	/*
	static GLuint tex = 0;
	static GLuint texD = 0;

	int GuiAlignment = eTextAlignment_Left | eTextAlignment_Bottom;
	static bool killed = false;
	if (Hero)
	{
		if(Hero->GetHealth() > 0)
		{
			Hero->OnDraw();
		}
		else
		{
			delete Hero;
			Hero = 0;
			killed = true;
		}
	}

	if (DynamicTextFont)
	{
		PE_Rect ScreenRect;
		std::stringstream sstream;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = Graphics->GetScreenHeigth();
		ScreenRect.Width = Graphics->GetScreenWidth();

		if (killed)
		{
			static int frames = 0;
			if(frames < 180)
			{
				sstream << "Game Over! Timmi has been killed. Please leave this game now! :(";
				GuiAlignment = eTextAlignment_Centered_H | eTextAlignment_Centered_V;
				frames++;
			}
			else
			{
				sstream << "Mind Walkers Production 2014-2016.";
			}

			DynamicTextFont->Write(sstream.str(), ScreenRect, GuiAlignment, &texD);
			goto EndRender;
		}

		if (StaticBox)
		{
			StaticBox->OnDraw();
			DynamicTextFont->Write("DEMO!", StaticBox->GetBox(), eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex);
		}

		if (!killed)
		{
			if (Hero)
				sstream << "Hero Speed: " << Hero->GetVelocity() << " Hero Health: " << Hero->GetHealth();
			else
				sstream << "Mind Walkers Production 2014-2016.";

			DynamicTextFont->Write(sstream.str(), ScreenRect, GuiAlignment, &texD);

			sstream.str(string());

			if (MainPlaylist)
				sstream << "Current music: " << MainPlaylist->GetCurrentMusic()->GetName();

			DynamicTextFont->Write(sstream.str(), ScreenRect, GuiAlignment, &texD,
					0, -DynamicTextFont->CalcTextHeigth("Hero Speed:"));
		}
	}

EndRender:
	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
	}
	if(texD)
	{
		glDeleteTextures(1, &texD);
		texD = 0;
	}
	*/
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
	InitLevels();

	if (!LevelManager()->LoadLevel())
	{
#ifdef DEBUG_ERRORS
	cout << "Cannot load levels!" << endl;
#endif
		return;
	}

	//LoadTextures();
	//CreatingObjects();

	Graphics->ClearScreen();
	//Graphics->SetClearColor(cColor());
	Graphics->SwapBuffers();

	fps.start();

#ifdef DEBUG_SYS
	cout << "Game start!" << endl;
#endif

	font *fps_font = new font("data/fonts/non-free/Minecraftia.ttf", 18);
	if (Graphics)
		Graphics->GetFontManager()->ManageFont(fps_font);

	std::string fps_rate;
	GLuint tex = 0;

	std::stringstream sstream;
	static int aCurrentLevel = LevelManager()->GetCurrentLevel();
	//bool simple_menu = true;
	//Главный цикл приложения
	while (!game::quit)
	{
NextFrame:
		mIsLevelChanged = false;
		//if (MainPlaylist && !Mix_PlayingMusic())
		//	MainPlaylist->Play();

		sstream.str(string());
		Graphics->ClearScreen();
		//Graphics->SetClearColor(cColor());

		if (Events->handle_events())
		{
			//Выходим
			quit = true;
			break;
		}

		if (Input->IsKeyDown(KEY_ESCAPE))
		{
			//Выходим
			quit = true;
			break;
		}
		if (Input->IsKeyDown(KEY_p))
		{
			if(!Audio()->IsPaused())
			{
				Audio()->PauseMusic();
			}
			else
			{
				Audio()->ResumeMusic();
			}
		}
		else if (Input->IsKeyDown(KEY_n))
		{
			Audio()->NextMusic();
		}
		else if (Input->IsKeyDown(KEY_b))
		{
			Audio()->PrevMusic();
		}

		/*
		if(simple_menu)
		{
			if(Input->IsKeyDown(KEY_1))
			{
				// Входим в игру
				simple_menu = false;
			}
			else if(Input->IsKeyDown(KEY_2))
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
		*/

		update();
		if (mIsLevelChanged)
			goto NextFrame;
		render();

		LevelManager()->PostRender();


		PE_Rect ScreenRect;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = Graphics->GetScreenHeigth();
		ScreenRect.Width = Graphics->GetScreenWidth();

		/*
		if(simple_menu)
		{
			// Отрисовываем текст в центре


			cColor col(1.0f, 0.0f, 1.0f, 1.0f);
			Graphics->DrawFilledRectangle(
					Graphics->GetScreenWidth()/2 - fps_font->CalcTextWidth("Press 1 to Play demo")/2,
					Graphics->GetScreenHeigth()/2 - fps_font->CalcTextHeigth("Press 1 to Play demo")/2,
					fps_font->CalcTextWidth("Press 1 to Play demo"), fps_font->CalcTextHeigth("Press 1 to Play demo")*2, col);

			fps_font->Write("Press 1 to Play demo", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex);

			// Отрисовываем текст чуть ниже центра
			fps_font->Write("Press 2 to Exit demo", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex, 0,
					fps_font->CalcTextHeigth("Press 1 to Play demo"));


		}
		else
		{
			// Отрисовываем текст в центре
			//help_text->SetText("Press ESC to Exit");
			fps_font->Write("Press ESC to Exit", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top, &tex);

			//help_text->SetText("Press Arrows to Move");
			fps_font->Write("Press Arrows to Move", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top, &tex,
					0, fps_font->CalcTextHeigth("Press ESC to Exit"));

		}
		*/

		sstream << "Parser Engine Demo. Version " << SYS_TEST_VERSION << "." << SYS_BUILD;
		//help_text->SetText(sstream.str());
		fps_font->Write(sstream.str(), ScreenRect, eTextAlignment_Right | eTextAlignment_Bottom, &tex);

		sstream.str(string());
		// Выводим FPS поверх игры

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

		Graphics->SwapBuffers();
		SDL_Delay(10);
		Input->Update();
	}

	delete fps_font;
#ifdef DEBUG_INFOS
#endif
	LevelManager()->UnloadLevel();
	//FreeTextures();
	//FreeObjects();
}

void game::InitLevels()
{
	textFont = new font("data/fonts/non-free/Minecraftia.ttf", 18);
	if (game::Instance()->Graphics)
		game::Instance()->Graphics->GetFontManager()->ManageFont(textFont);

	mMainMenuLevel = new cMainMenu();
	mGameLevel = new cGameLevel("Test level");
}

void game::MainQuit()
{
	game::quit = true;
}

int game::CreatingObjects()
{
#ifdef DEBUG_SYS
	cout << "Creating objects" << endl;
#endif

	/*
	if(Collision)
	{
		layer = new iCollisionLayer(PE_Rect {0, 0, (float)Graphics->GetScreenWidth(), (float)Graphics->GetScreenHeigth()});
		//layer = Collision->NewCollisionLayer(Graphics->GetScreenWidth(), Graphics->GetScreenHeigth(), 0, 0);
		Collision->AddCollisionLayer(layer);
	}

	if(!Hero)
	{
		Hero = new hero();
		Hero->SetStaticTexture(Mmenu.background);
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

			Hero->CALLBACK(hero::CollisionHandler, Hero, 0);
		}

	}
	if(!Gui)
	{
		Gui = new game_gui();
	}
	if(!DynamicTextFont)
	{
		DynamicTextFont = new font("data/fonts/non-free/Minecraftia.ttf", 14);
		if(Graphics)
			DynamicTextFont->SetFontManager(Graphics->GetFontManager());
	}

	if(!StaticBox)
	{
		StaticBox = new cStaticBox("Box1", 100);
		StaticBox->SetBox(100,100,200,200);
		if(layer)
		{
			layer->AddCollisionBody(StaticBox);
			StaticBox->AddNewLayer(layer);

			StaticBox->SetCollisionsPointer(Collision);
			Collision->AddCollisionBody(StaticBox);

			StaticBox->CALLBACK(cStaticBox::CollisionHandler, StaticBox, 0);
		}
	}
	if (!MainPlaylist && SYS_AUDIO)
	{
		MainPlaylist = Audio()->CreatePlaylist();
		//MainPlaylist->AddMusic("data/sounds/test.ogg"); // TODO: for test playlist
		MainPlaylist->AddMusic("data/sounds/Zhenya_Sazonov-Flying.ogg");
	}
	*/
	return 0;
}

//Загружаем текстуры для дальнейшей работы с ними
int game::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << "Load textures" << endl;
#endif
	/*
	Mmenu.background = new cTexture("data/graphics/test/test.png");
	//Mmenu.background = new image("foo.png");
	Mmenu.MainMenuFont = new font("data/fonts/non-free/Minecraftia.ttf", 30);
	if(Graphics)
		Graphics->GetFontManager()->ManageFont(Mmenu.MainMenuFont);
		*/
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
/*
	if(Mmenu.background)
		delete Mmenu.background;
	Mmenu.background = 0;
	if(Mmenu.MainMenuFont)
		delete Mmenu.MainMenuFont;
	Mmenu.MainMenuFont = 0;
*/
}

void game::FreeObjects()
{
#ifdef DEBUG_SYS
	cout << "Freeing objects" << endl;
#endif
	/*
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
	*/
	delete Collision;
	delete textFont;
}

game::game()
{
	//Mmenu.background = 0;
	//Mmenu.MainMenuFont = 0;
	Gui = 0;
	//Hero = 0;
	//layer = 0;
	DynamicTextFont = 0;
	//StaticBox = 0;
	Collision = new cCollision();
	//MainPlaylist = 0;
	textFont = 0;
	mGameLevel = 0;
	mMainMenuLevel = 0;
	mIsLevelChanged = false;
}
game::~game()
{
#ifdef DEBUG_SYS
	cout << "Game clean up - success" << endl;
#endif
}

game_gui::game_gui()
{
	bottom.DynamicTextFont = 0;
	bottom.StaticTextFont = 0;
	bottom.texture = 0;
}
game_gui::~game_gui()
{
	if(bottom.DynamicTextFont)
	{
		delete bottom.DynamicTextFont;
		bottom.DynamicTextFont = 0;
	}
	if(bottom.StaticTextFont)
	{
		delete bottom.StaticTextFont;
		bottom.StaticTextFont = 0;
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
