/*
 * main.cpp
 *
 *  Created on: 17.04.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "game.h"

using namespace std;

int main(int argc, char *argv[])
{
	game *Game = game::Instance();

	if (Game->init()<0)
	{
		game::DeleteInstance();
		return 0;
	}
#ifdef DEBUG_SYS
	cout << "Game initialization - success" << endl;
#endif

	Game->MainLoop();


	Game->CleanUp();
	game::DeleteInstance();

#ifdef DEBUG_SYS
	cout << "Game cleanup - success" << endl;
#endif
	return 0;
}


