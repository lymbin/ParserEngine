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
	engine *Engine = new engine();
	if(Engine->init()<0)
	{
		delete Engine;
		return 0;
	}

	Engine->MainLoop();


	Engine->CleanUp();
	delete Engine;

	return 0;
}
