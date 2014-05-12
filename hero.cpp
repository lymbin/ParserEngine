/*
 * hero.cpp
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#include "hero.h"


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
	rmove_animation = 0;
	lmove_animation = 0;
	jump_animation = 0;
	sit_animation = 0;

	items.clear();

	Box.X = 0;
	Box.Y = 0;
	Box.Heigth = 0;
	Box.Width = 0;

	anim_step = 0;
	jumped = false;

	last_state = MOVE_NONE;
}
hero::~hero()
{
	if(static_texture)
	{
		delete static_texture;
		static_texture = 0;
	}
	if(rmove_animation)
	{
		delete rmove_animation;
		rmove_animation = 0;
	}
	if(lmove_animation)
	{
		delete lmove_animation;
		lmove_animation = 0;
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
	if(pos == MOVE_RIGTH)
	{
		Box.X+=anim_step;
		rmove_animation->Update();
	}
	else if(pos == MOVE_LEFT)
	{
		Box.X-=anim_step;
		lmove_animation->Update();
	}
	if(Box.X > SYS_WIDTH)
		Box.X-=anim_step;
	else if(Box.X < 0)
		Box.X+=anim_step;
}
void hero::jump()
{
	if(!jumped)
	{
		jump_animation->Reset();
		jump_animation->SetRepeats(1);
		jumped = true;
	}

	jump_animation->Update();
	if(jump_animation->GetFramesSize()/2)
	if((jump_animation->GetFramesSize()/2 - (jump_animation->GetTrackNumber()+1)) < 0)
	{
		Box.Y-=anim_step/2;
		if((jump_animation->GetFramesSize()/2 - (jump_animation->GetTrackNumber()+1)) > -1)
		{
			// тестовая функция для отрисовки нечётного количества анимаций - стоим на месте
			Box.Y+=anim_step/2;
		}
	}
	else
	{
		Box.Y+=anim_step/2;
	}
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

	last_state = MOVE_NONE;

	if(jumped)
	{
		if(!jump_animation->IsOver())
		{
			if(last_state < MOVE_JUMP)
				last_state = MOVE_JUMP;
			jump();
		}
		else
		{
			jumped = false;
		}
	}
	if(rmove_animation && (Game->Input->IsKeyDown(KEY_RIGHT) || Game->Input->IsKeyHeld(KEY_RIGHT)))
	{
		if(last_state < MOVE_RIGTH)
			last_state = MOVE_RIGTH;
		move(MOVE_RIGTH);
	}
	else if(lmove_animation && (Game->Input->IsKeyDown(KEY_LEFT) || Game->Input->IsKeyHeld(KEY_LEFT)))
	{
		if(last_state < MOVE_LEFT)
			last_state = MOVE_LEFT;
		move(MOVE_LEFT);
	}
	if(sit_animation && (Game->Input->IsKeyDown(KEY_DOWN) || Game->Input->IsKeyHeld(KEY_DOWN)))
	{
		if(last_state < MOVE_SIT)
			last_state = MOVE_SIT;
		sit();
	}
	else if(jump_animation && (Game->Input->IsKeyHeld(KEY_UP) || Game->Input->IsKeyHeld(KEY_UP)))
	{
		if(!jumped)
		{
			jump();
			if(last_state < MOVE_JUMP)
				last_state = MOVE_JUMP;
		}
	}
}
void hero::render()
{
	switch (last_state) {
		case MOVE_NONE:
			static_texture->Draw(Box.X, Box.Y);
			break;
		case MOVE_RIGTH:
			rmove_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_LEFT:
			lmove_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_SIT:
			sit_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_JUMP:
			jump_animation->Draw(Box.X, Box.Y);
			break;
		default:
			static_texture->Draw(Box.X, Box.Y);
			break;
	}
}
