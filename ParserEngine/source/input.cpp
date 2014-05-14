/*
 * input.cpp
 *
 *  Created on: 07.05.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

input::input()
{
	MouseX = 0;
	MouseY = 0;
	KeyStates.clear();
	MouseButtons.clear();
	KeyHeldTime.clear();
}

//-----------------------------------------------------------------------

input::~input()
{
	MouseX = 0;
	MouseY = 0;
	KeyStates.clear();
	MouseButtons.clear();
	KeyHeldTime.clear();

#ifdef DEBUG_SYS
	cout << "Input clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

int input::init()
{
	return 0;
}

//-----------------------------------------------------------------------

Uint32 input::TimeHeld(eKey Key)
{
	if(KeyHeldTime.size() < (unsigned int)Key)
		return 0;

	if(KeyStates[Key] != 'h')
		return KeyHeldTime[Key];

	return (SDL_GetTicks() - KeyHeldTime[Key]);
}

//-----------------------------------------------------------------------

bool input::IsKeyHeld(eKey Key)
{
	if(KeyStates.size() < (unsigned int)Key)
		return false;

	if(KeyStates[Key] == 'h')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsKeyDown(eKey Key)
{
	if(KeyStates.size() < (unsigned int)Key)
		return false;

	if(KeyStates[Key] == 'd')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsKeyUp(eKey Key)
{
	if(KeyStates.size() < (unsigned int)Key)
		return false;

	if(KeyStates[Key] == 'u')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

int input::Update()
{
	SDL_Event event;
	std::vector< eKey> keys;

	while( SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return true;
				break;
			case SDL_KEYDOWN:
				KeyStates[event.key.keysym.sym] = 'd';
				keys.push_back((eKey)event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				KeyStates[event.key.keysym.sym] = 'u';
				keys.push_back((eKey)event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				MouseX = event.motion.x;
				MouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			default:
				break;
		}
	}

	for(std::map<int, char>::iterator it = KeyStates.begin(); it != KeyStates.end(); it++)
	{
		if(it->second == 'u')
		{
			bool keyFound = false;
			for(unsigned int loop = 0; loop < keys.size(); loop++)
			{
				if(keys[loop] == it->first)
				{
					keyFound = true;
					break;
				}
			}
			if(!keyFound)
			{
				it->second = 'n';
			}
		}
		else if(it->second == 'd')
		{
			bool keyFound = false;
			for(unsigned int loop = 0; loop < keys.size(); loop++)
			{
				if(keys[loop] == it->first)
				{
					keyFound = true;
					break;
				}
			}
			if(!keyFound)
			{
				KeyHeldTime[it->first] = SDL_GetTicks();
				it->second = 'h';
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------


