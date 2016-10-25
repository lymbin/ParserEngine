/*
 * GameLevels.cpp
 *
 *  Created on: 23.10.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "GameLevels.h"
#include "game.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cGameLevel::cGameLevel(std::string aLevelName):cLevel(aLevelName)
{
	Gui = 0;
	Player = 0;
	CollisionLayer = 0;
	DynamicTextFont = 0;
	StaticBox = 0;
	Collision = new cCollision();
	LevelPlaylist = 0;
	PlayerStaticTexture = 0;
}

//-----------------------------------------------------------------------

cGameLevel::~cGameLevel()
{
#ifdef DEBUG_SYS
	cout << "Game level cleanup - success" << endl;
#endif
}

//-----------------------------------------------------------------------

cMainMenu::cMainMenu():cLevel("MainMenu")
{
	MMenu.background = 0;
	MMenu.MainMenuFont = 0;
}

//-----------------------------------------------------------------------

cMainMenu::~cMainMenu()
{

}

//-----------------------------------------------------------------------

int cGameLevel::Init()
{
	LoadTextures();
	CreatingObjects();
	return 0;
}

int cGameLevel::CleanUp()
{
	FreeObjects();
	FreeTextures();
	return 0;
}

// Определяем объекты
int cGameLevel::CreatingObjects()
{
#ifdef DEBUG_SYS
	cout << "Creating objects" << endl;
#endif

	if (game::Instance()->Collision)
	{
		CollisionLayer = new iCollisionLayer(PE_Rect {0, 0, (float)game::Instance()->Graphics->GetScreenWidth(), (float)game::Instance()->Graphics->GetScreenHeigth()});
		//layer = Collision->NewCollisionLayer(Graphics->GetScreenWidth(), Graphics->GetScreenHeigth(), 0, 0);
		game::Instance()->Collision->AddCollisionLayer(CollisionLayer);
	}

	if (!Player)
	{
		Player = new hero();
		Player->SetStaticTexture(PlayerStaticTexture);
		Player->SetVelocity(20);
		Player->SetScaledAndRotate(0.3, 0);

		if (Player->GetTexture())
			Player->SetBox(Player->GetTexture()->GetWidth()*Player->GetScaledMultiplier(), Player->GetTexture()->GetHeigth()*Player->GetScaledMultiplier(), 0, 100);

		if (CollisionLayer)
		{
			// TODO: заменить 2 строки одной?
			CollisionLayer->AddCollisionBody(Player);
			Player->AddNewLayer(CollisionLayer);

			Player->SetCollisionsPointer(game::Instance()->Collision);
			game::Instance()->Collision->AddCollisionBody(Player);

			Player->CALLBACK(hero::CollisionHandler, Player, 0);
		}

	}
	if (!Gui)
	{
		Gui = new game_gui();
	}
	if (!DynamicTextFont)
	{
		DynamicTextFont = new font("data/fonts/non-free/Minecraftia.ttf", 14);
		if (game::Instance()->Graphics)
			DynamicTextFont->SetFontManager(game::Instance()->Graphics->GetFontManager());
	}
	if(!StaticBox)
	{
		StaticBox = new cStaticBox("Box1", 100);
		StaticBox->SetBox(100,100,200,200);
		if (CollisionLayer)
		{
			CollisionLayer->AddCollisionBody(StaticBox);
			StaticBox->AddNewLayer(CollisionLayer);

			StaticBox->SetCollisionsPointer(game::Instance()->Collision);
			game::Instance()->Collision->AddCollisionBody(StaticBox);

			StaticBox->CALLBACK(cStaticBox::CollisionHandler, StaticBox, 0);
		}
	}
	if (!LevelPlaylist && SYS_AUDIO)
	{
		LevelPlaylist = game::Instance()->Audio()->CreatePlaylist();
		//MainPlaylist->AddMusic("data/sounds/test.ogg"); // TODO: for test playlist
		LevelPlaylist->AddMusic("data/sounds/Zhenya_Sazonov-Flying.ogg");
	}

	return 0;
}

// Загружаем текстуры
int cGameLevel::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << mLevelName << " : Load textures" << endl;
#endif
	PlayerStaticTexture = new cTexture("data/graphics/test/test.png");
	return 0;
}

// Освобождаем текстуры
void cGameLevel::FreeTextures()
{
#ifdef DEBUG_SYS
	cout << mLevelName << " : Freeing textures" << endl;
#endif

	if (PlayerStaticTexture)
		delete PlayerStaticTexture;
	PlayerStaticTexture = 0;
}

// Освобождаем объекты
void cGameLevel::FreeObjects()
{
#ifdef DEBUG_SYS
	cout << "Freeing objects" << endl;
#endif
	if (Player)
	{
		delete Player;
	}
	if (Gui)
	{
		delete Gui;
	}
	if (CollisionLayer)
	{
		delete CollisionLayer;
	}
	if(StaticBox)
	{
		delete StaticBox;
	}
	delete Collision;
}

// Обновляем координаты текстур и объектов
void cGameLevel::Update()
{
	if (LevelPlaylist && !Mix_PlayingMusic())
		LevelPlaylist->Play();
	// Обновляем все координаты объектов, текстов и прочего
	// TODO: Обновляем все Updatable объекты
	if (Player)
	{
		PE_Rect OldBox = Player->GetBox();// Запоминаем старый бокс для случая столкновения с каким-либо телом и возвратом в исходное положение
		Player->Update();
		Player->CALLBACK(hero::CollisionHandler, Player, &OldBox); // TODO: другое использование callback
	}
	if (StaticBox)
		StaticBox->Update();

	if (Player)
		Player->HandleCollisions();

	PostUpdate();
}

// Обновление данных объекта в конце выполнения общего update
void cGameLevel::PostUpdate()
{
	if (Player && Player->GetHealth() > 0)
	{
		Player->PostUpdate();
		StaticBox->PostUpdate();
	}
}

// Отрисовываем текстуры и объекты
void cGameLevel::Render()
{
	// Отрисовываем все объекты

	static GLuint tex = 0;
	static GLuint texD = 0;

	int GuiAlignment = eTextAlignment_Left | eTextAlignment_Bottom;
	static bool killed = false;
	if (Player)
	{
		if (Player->GetHealth() > 0)
		{
			Player->OnDraw();
		}
		else
		{
			delete Player;
			Player = 0;
			killed = true;
		}
	}

	if (DynamicTextFont)
	{
		PE_Rect ScreenRect;
		std::stringstream sstream;
		ScreenRect.X = ScreenRect.Y = 0;
		ScreenRect.Heigth = game::Instance()->Graphics->GetScreenHeigth();
		ScreenRect.Width = game::Instance()->Graphics->GetScreenWidth();

		if (killed)
		{
			static unsigned int frames = 0;
			if (frames < 180)
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
			if (Player)
				sstream << "Hero Speed: " << Player->GetVelocity() << " Hero Health: " << Player->GetHealth();
			else
				sstream << "Mind Walkers Production 2014-2016.";

			DynamicTextFont->Write(sstream.str(), ScreenRect, GuiAlignment, &texD);

			sstream.str(string());

			if (LevelPlaylist)
				sstream << "Current music: " << LevelPlaylist->GetCurrentMusic()->GetName();

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

void cGameLevel::PostRender()
{
	GLuint tex = 0;

	PE_Rect ScreenRect;
	ScreenRect.X = ScreenRect.Y = 0;
	ScreenRect.Heigth = game::Instance()->Graphics->GetScreenHeigth();
	ScreenRect.Width = game::Instance()->Graphics->GetScreenWidth();

	// Отрисовываем текст в центре
	//help_text->SetText("Press ESC to Exit");
	game::Instance()->textFont->Write("Press ESC to Exit", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top, &tex);

	//help_text->SetText("Press Arrows to Move");
	game::Instance()->textFont->Write("Press Arrows to Move", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Top, &tex,
			0, game::Instance()->textFont->CalcTextHeigth("Press ESC to Exit"));

	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
	}
}

int cMainMenu::Init()
{
	LoadTextures();
	CreatingObjects();
	return 0;
}

int cMainMenu::CleanUp()
{
	FreeObjects();
	FreeTextures();
	return 0;
}

// Определяем объекты
int cMainMenu::CreatingObjects()
{
	return 0;
}

// Загружаем текстуры
int cMainMenu::LoadTextures()
{
#ifdef DEBUG_SYS
	cout << mLevelName << " : Load textures" << endl;
#endif
	MMenu.background = new cTexture("data/graphics/test/test.png");
	//Mmenu.background = new image("foo.png");
	MMenu.MainMenuFont = new font("data/fonts/non-free/Minecraftia.ttf", 30);
	if(game::Instance()->Graphics)
		game::Instance()->Graphics->GetFontManager()->ManageFont(MMenu.MainMenuFont);
	//Mmenu.background->SetColorKey();
	//Mmenu.title = new text("Кириллица", "data/fonts/PTS55F.ttf", 30);
	//Mmenu.button_start = new image();
	//Mmenu.button_exit = new image();

	//Mmenu.background->Open();

	return 0;
}

// Освобождаем текстуры
void cMainMenu::FreeTextures()
{
#ifdef DEBUG_SYS
	cout << mLevelName << " : Freeing textures" << endl;
#endif

	if(MMenu.background)
		delete MMenu.background;
	MMenu.background = 0;
	if(MMenu.MainMenuFont)
		delete MMenu.MainMenuFont;
	MMenu.MainMenuFont = 0;
}

// Освобождаем объекты
void cMainMenu::FreeObjects()
{

}

// Обновляем координаты текстур и объектов
void cMainMenu::Update()
{
	if (game::Instance()->Input->IsKeyDown(KEY_1))
	{
		// Входим в игру
		cLevelManager::Instance()->NextLevel();
		game::Instance()->mIsLevelChanged = true; // render last level cause error
	}
	else if (game::Instance()->Input->IsKeyDown(KEY_2))
	{
		//Выходим
		game::Instance()->MainQuit();
	}
}

// Обновление данных объекта в конце выполнения общего update
void cMainMenu::PostUpdate()
{

}

// Отрисовываем текстуры и объекты
void cMainMenu::Render()
{

}

void cMainMenu::PostRender()
{
	GLuint tex = 0;

	PE_Rect ScreenRect;
	ScreenRect.X = ScreenRect.Y = 0;
	ScreenRect.Heigth = game::Instance()->Graphics->GetScreenHeigth();
	ScreenRect.Width = game::Instance()->Graphics->GetScreenWidth();

	// Отрисовываем текст в центре


	cColor col(1.0f, 0.0f, 1.0f, 1.0f);
	game::Instance()->Graphics->DrawFilledRectangle(
			game::Instance()->Graphics->GetScreenWidth()/2 - game::Instance()->textFont->CalcTextWidth("Press 1 to Play demo")/2,
			game::Instance()->Graphics->GetScreenHeigth()/2 - game::Instance()->textFont->CalcTextHeigth("Press 1 to Play demo")/2,
			game::Instance()->textFont->CalcTextWidth("Press 1 to Play demo"), game::Instance()->textFont->CalcTextHeigth("Press 1 to Play demo")*2, col);

	game::Instance()->textFont->Write("Press 1 to Play demo", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex);

	// Отрисовываем текст чуть ниже центра
	game::Instance()->textFont->Write("Press 2 to Exit demo", ScreenRect, eTextAlignment_Centered_H | eTextAlignment_Centered_V, &tex, 0,
			game::Instance()->textFont->CalcTextHeigth("Press 1 to Play demo"));

	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
	}
}
