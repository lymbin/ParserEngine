/*
 * window.cpp
 *
 *  Created on: 19.05.2014
 *      Author: dmitry
 */

#include "Graphics.h"

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

window::window(graphics *gfx)
{
	if(SYS_FULLSCREEN)
		windowed = true;
	else
		windowed = false;

	Graphics = gfx;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

int window::handle_event(SDL_Event event)
{
	if(Graphics)
	{
		switch(event.type)
		{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					//Get new dimensions and repaint on window size change
					case SDL_WINDOWEVENT_RESIZED:
						Graphics->ResizeWin(event.window.data1, event.window.data2);
						return 1;
						break;
					case SDL_WINDOWEVENT_EXPOSED:
					case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						// TODO: add events handlers
						return 1;
						break;
				}
				break;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------

void window::ToggleFullScreen()
{
	if(Graphics)
	{
		Graphics->ToggleFullScreen();
	}
}

//-----------------------------------------------------------------------
