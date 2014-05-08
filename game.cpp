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

hero::hero(std::string nam, int hp)
{
	Game = 0;

	name = nam;
	hit_points = hp;

	armor.itemName = "Skin";
	armor.main_classification = TEST_ROBE;
	armor.sub_classification = TEST_ROBE_NONE;

	weapon.itemName = "Hand";
	weapon.main_classification = TEST_WEAPON;
	weapon.sub_classification = TEST_WEAPON_NONE;

	static_texture = 0;
	move_animation = 0;
	jump_animation = 0;
	sit_animation = 0;

	items.clear();

	Box.X = 0;
	Box.Y = 0;
	Box.Heigth = 0;
	Box.Width = 0;

	anim_step = 0;
	jumped = false;
}
hero::~hero()
{
	if(static_texture)
	{
		delete static_texture;
		static_texture = 0;
	}
	if(move_animation)
	{
		delete move_animation;
		move_animation = 0;
	}
	if(jump_animation)
	{
		delete jump_animation;
		jump_animation = 0;
	}
	if(sit_animation)
	{
		delete sit_animation;
		sit_animation = 0;
	}

	Game = 0;
}

void hero::move(int pos)
{
	move_animation->Update();

	if(pos == MOVE_RIGTH)
	{
		Box.X+=anim_step;
	}
	else if(pos == MOVE_LEFT)
	{
		Box.X-=anim_step;
	}
}
void hero::jump()
{
	jump_animation->SetRepeats(1);
	jumped = true;
	//if()
	jump_animation->Update();

	//Box.Y+
}
void hero::sit()
{
	if(sit_animation)
	{
		sit_animation->Update();
	}
}
void hero::shoot()
{

}
void hero::update()
{
	if((!Game)||(!Game->Input))
		return;

	bool Static = true;

	if(jumped)
	{
		if(!jump_animation->IsOver())
			jump_animation->Update();
	}
	if(move_animation && (Game->Input->IsKeyDown(KEY_RIGHT) || Game->Input->IsKeyHeld(KEY_RIGHT)))
	{
		Static = false;
		move(MOVE_RIGTH);
	}
	else if(move_animation && (Game->Input->IsKeyDown(KEY_LEFT) || Game->Input->IsKeyHeld(KEY_LEFT)))
	{
		Static = false;
		move(MOVE_LEFT);
	}
	else if(sit_animation && (Game->Input->IsKeyDown(KEY_DOWN) || Game->Input->IsKeyHeld(KEY_DOWN)))
	{
		Static = false;
		sit();
	}
	else if(jump_animation && (Game->Input->IsKeyHeld(KEY_UP) || Game->Input->IsKeyHeld(KEY_UP)))
	{
		if(!jumped)
		{
			Static = false;
			jump();
		}
	}

	if(Static)
		state_manager.push_back(MOVE_UNKNOWN);






}
void hero::render()
{
	while(!state_manager.empty())
	{
		if(state_manager[state_manager.size()] == MOVE_UNKNOWN)
		{
			static_texture->Draw(Box.X, Box.Y);
		}
		else if(state_manager[state_manager.size()] == MOVE_LEFT || state_manager[state_manager.size()] == MOVE_RIGTH)
		{
			move_animation->Draw(0, 200);
		}
		else if(state_manager[state_manager.size()] == MOVE_JUMP)
		{
			jump_animation->Draw(0, 200);
		}
		else if(state_manager[state_manager.size()] == MOVE_SIT)
		{
			sit_animation->Draw(0, 200);
		}
	}
}
