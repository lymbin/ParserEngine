/*
 * game.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dmitry
 */

#include "game.h"

game_gui::game_gui()
{
	bottom.dynamic_text = 0;
	bottom.static_text = 0;
	bottom.texture = 0;
}
game_gui::~game_gui()
{
	if(bottom.dynamic_text)
	{
		delete bottom.dynamic_text;
		bottom.dynamic_text = 0;
	}
	if(bottom.static_text)
	{
		delete bottom.static_text;
		bottom.static_text = 0;
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
