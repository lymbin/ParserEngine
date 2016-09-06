/*
 * input.cpp
 *
 *  Created on: 07.05.2014
 *      Author: dmitry
 */

#include "Input.h"

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
	Engine = 0;
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
	KeyStates[SDL_SCANCODE_ESCAPE] = 'n';
	for(int loop = 0; loop <= BUTTON_LASTENUM; loop++)
	{
		MouseButtons.push_back('n');
	}

#ifdef DEBUG_SYS
	cout << "Input system initialization - success" << endl;
#endif
	return 0;
}

//-----------------------------------------------------------------------

Uint32 input::TimeHeld(eKey Key)
{
	if(KeyStates[Key] != 'h')
		return KeyHeldTime[Key];

	return (SDL_GetTicks() - KeyHeldTime[Key]);
}

//-----------------------------------------------------------------------

bool input::IsKeyHeld(eKey Key)
{
	if(KeyStates[Key] == 'h')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsKeyDown(eKey Key)
{
	if(KeyStates[Key] == 'd')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsKeyUp(eKey Key)
{
	if(KeyStates[Key] == 'u')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsButtonDown(eButton button)
{
	if(MouseButtons[button] == 'd')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsButtonUp(eButton button)
{
	if(MouseButtons[button] == 'u')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

bool input::IsButtonHeld(eButton button)
{
	if(MouseButtons[button] == 'h')
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------

int input::handle_event(SDL_Event event)
{
	std::vector< eKey> keys;
	switch(event.type)
	{
		case SDL_KEYDOWN:
			KeyStates[event.key.keysym.scancode] = 'd';
			keys.push_back((eKey)event.key.keysym.scancode);
			cout << "Key down:" << event.key.keysym.scancode << endl;
			return 1;
			break;
		case SDL_KEYUP:
			KeyStates[event.key.keysym.scancode] = 'u';
			keys.push_back((eKey)event.key.keysym.scancode);
			return 1;
			break;
		case SDL_MOUSEMOTION:
			MouseX = event.motion.x;
			MouseY = event.motion.y;
			return 1;
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseButtons[event.button.button] = 'd';
			break;
		case SDL_MOUSEBUTTONUP:
			MouseButtons[event.button.button] = 'u';
			break;
	}

	return 0;
}

//-----------------------------------------------------------------------

int input::Update()
{
	std::vector< eKey> keys;

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

	static int stMouse = 0;

	for(std::vector <char>::iterator it = MouseButtons.begin(); it != MouseButtons.end(); ++it)
	{
		if((*it) == 'u')
		{
			if(stMouse == 1)
			{
				stMouse--;
			}
			else
			{
				(*it) = 'n';
			}
		}
		else if((*it) == 'd')
		{
			if(stMouse == 0)
			{
				stMouse++;
			}
			if(stMouse == 1)
			{
				(*it) = 'h';
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------


