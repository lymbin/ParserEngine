/*
 * window.cpp
 *
 *  Created on: 19.05.2014
 *      Author: dmitry
 */

#include "engine.h"

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
			case SDL_VIDEORESIZE:
				Graphics->ResizeWin(event.resize.w, event.resize.h);

				return 1;
				break;
			case SDL_ACTIVEEVENT:
				return 1;
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
