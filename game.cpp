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

	PostUpdate();
}

void game::PostUpdate()
{
	LevelManager()->PostUpdate();
}

void game::render()
{
	// Отрисовываем все объекты
	Graphics->ClearScreen();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glPushMatrix();

	LevelManager()->Render();
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
	//Главный цикл приложения
	while (!game::quit)
	{
NextFrame:
		mIsLevelChanged = false;

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

		update();
		if (mIsLevelChanged)
			goto NextFrame;
		render();

		LevelManager()->PostRender();


		PE_Rect ScreenRect;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = Graphics->GetScreenHeigth();
		ScreenRect.Width = Graphics->GetScreenWidth();

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
	cout << "Creating game objects" << endl;
#endif
	return 0;
}

//Загружаем текстуры для дальнейшей работы с ними
int game::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << "Load game textures" << endl;
#endif
	return 0;
}

void game::FreeTextures()
{
#ifdef DEBUG_SYS
	cout << "Freeing game textures" << endl;
#endif
}

void game::FreeObjects()
{
#ifdef DEBUG_SYS
	cout << "Freeing game objects" << endl;
#endif
	delete Collision;
	delete textFont;
}

game::game()
{
	Gui = 0;
	DynamicTextFont = 0;
	Collision = new cCollision();
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
