/*
 * window.cpp
 *
 *  Created on: 19.05.2014
 *      Author: dmitry
 */

#include "engine.h"

window::window(graphics *gfx)
{
	if(SYS_FULLSCREEN)
		windowed = true;
	else
		windowed = false;

	Graphics = gfx;
}

void window::handle_events()
{

}
void window::ToggleFullScreen()
{

}


