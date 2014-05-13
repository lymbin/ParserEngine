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
	moveright_animation = 0;
	moveleft_animation = 0;
	movedown_animation = 0;
	moveup_animation = 0;
	/*
	jump_animation = 0;
	sit_animation = 0;
	*/
	items.clear();

	Box.X = 0;
	Box.Y = 0;
	Box.Heigth = 0;
	Box.Width = 0;

	anim_Xstep = anim_Ystep = 0;

	//jumped = false;

	last_state = MOVE_NONE;
}
hero::~hero()
{
	if(static_texture)
	{
		delete static_texture;
		static_texture = 0;
	}
	if(moveright_animation)
	{
		delete moveright_animation;
		moveright_animation = 0;
	}
	if(moveleft_animation)
	{
		delete moveleft_animation;
		moveleft_animation = 0;
	}
	if(movedown_animation)
	{
		delete movedown_animation;
		movedown_animation = 0;
	}
	if(moveup_animation)
	{
		delete moveup_animation;
		moveup_animation = 0;
	}

	Game = 0;
}

void hero::move(int pos)
{
	if(pos == MOVE_RIGHT)
	{
		Box.X+=anim_Xstep;
		moveright_animation->Update();
	}
	else if(pos == MOVE_LEFT)
	{
		Box.X-=anim_Xstep;
		moveleft_animation->Update();
	}
	if(pos == MOVE_UP)
	{
		Box.Y-=anim_Ystep;
		moveup_animation->Update();
	}
	else if(pos == MOVE_DOWN)
	{
		Box.Y+=anim_Ystep;
		movedown_animation->Update();
	}

	// Пока блочим движение тела
	// 	но в дальнейшем нужно менять камеру при движении
	if(Box.X > SYS_WIDTH)
		Box.X-=anim_Xstep;
	else if(Box.X < 0)
		Box.X+=anim_Xstep;
	if(Box.Y > SYS_HEIGTH)
		Box.Y-=anim_Ystep;
	else if(Box.Y < 0)
		Box.Y+=anim_Ystep;
}
void hero::jump()
{
	/*
	 * TODO: код для простого прыжка
	if(!jumped)
	{
		jump_animation->Reset();
		jump_animation->SetRepeats(1);
		jumped = true;
	}

	jump_animation->Update();

	if((jump_animation->GetFramesSize()/2 - (jump_animation->GetTrackNumber()+1)) < 0)
	{
		Box.Y-=anim_step/2;
		if(((signed int)jump_animation->GetFramesSize()/2 - (jump_animation->GetTrackNumber()+1)) > -1)
		{
			// тестовая функция для отрисовки нечётного количества анимаций - стоим на месте
			Box.Y+=anim_step/2;
		}
	}
	else
	{
		Box.Y+=anim_step/2;
	}
	*/
}
void hero::sit()
{
	/*
	 * TODO: код для анимации приседа - 1 картинка
	if(sit_animation)
	{
		sit_animation->Update();
	}
	*/
}
void hero::shoot()
{
	/*
	 * TODO: код для выстрела
	 */
}
void hero::update()
{
	if((!Game)||(!Game->Input))
		return;

	last_state = MOVE_NONE;

	/*
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
	*/

	if(moveright_animation && (Game->Input->IsKeyDown(KEY_RIGHT) || Game->Input->IsKeyHeld(KEY_RIGHT)))
	{
		if(last_state < MOVE_RIGHT)
			last_state = MOVE_RIGHT;
		move(MOVE_RIGHT);
	}
	else if(moveleft_animation && (Game->Input->IsKeyDown(KEY_LEFT) || Game->Input->IsKeyHeld(KEY_LEFT)))
	{
		if(last_state < MOVE_LEFT)
			last_state = MOVE_LEFT;
		move(MOVE_LEFT);
	}
	if(movedown_animation && (Game->Input->IsKeyDown(KEY_DOWN) || Game->Input->IsKeyHeld(KEY_DOWN)))
	{
		if(last_state < MOVE_DOWN)
			last_state = MOVE_DOWN;
		move(MOVE_DOWN);
	}
	else if(moveup_animation && (Game->Input->IsKeyHeld(KEY_UP) || Game->Input->IsKeyHeld(KEY_UP)))
	{
		if(last_state < MOVE_UP)
			last_state = MOVE_UP;
		move(MOVE_UP);
	}
	/*
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
	*/
}
void hero::render()
{

	// Можно сделать карту мувов и анимаций - возможно упростит программирование
	switch (last_state) {
		case MOVE_NONE:
			if(static_texture)
				static_texture->Draw(Box.X, Box.Y);
			break;
		case MOVE_RIGHT:
			if(moveright_animation)
				moveright_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_LEFT:
			if(moveleft_animation)
				moveleft_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_DOWN:
			if(movedown_animation)
				movedown_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_UP:
			if(moveup_animation)
				moveup_animation->Draw(Box.X, Box.Y);
			break;
			/*
		case MOVE_SIT:
			if(sit_animation)
				sit_animation->Draw(Box.X, Box.Y);
			break;
		case MOVE_JUMP:
			if(jump_animation)
				jump_animation->Draw(Box.X, Box.Y);
			break;
			*/
		default:
			if(static_texture)
				static_texture->Draw(Box.X, Box.Y);
			break;
	}
}
void hero::SetTexture(image *texture)
{
	static_texture = texture;
}
void hero::LoadTexture(std::string file)
{
	if(static_texture)
	{
		delete static_texture;
		static_texture = 0;
	}
	static_texture = new image(file);
}
void hero::SetAnim(int AnimType, image *texture, std::vector< PE_Rect > frames)
{
	switch (AnimType) {
		case ANIM_UNKNOWN:
			// ERROR
			break;
		case ANIM_MOVE_RIGHT:
			if(moveright_animation)
			{
				delete moveright_animation;
				moveright_animation = 0;
			}
			moveright_animation = new animation();
			moveright_animation->SetTexture(texture);
			moveright_animation->ClearFrames();
			for(unsigned int loop = 0; loop < frames.size(); ++loop)
			{
				moveright_animation->AddNewFrame(frames[loop]);
			}
			break;
		case ANIM_MOVE_LEFT:
			if(moveleft_animation)
			{
				delete moveleft_animation;
				moveleft_animation = 0;
			}
			moveleft_animation = new animation();
			moveleft_animation->SetTexture(texture);
			moveleft_animation->ClearFrames();
			for(unsigned int loop = 0; loop < frames.size(); ++loop)
			{
				moveleft_animation->AddNewFrame(frames[loop]);
			}
			break;
		case ANIM_MOVE_DOWN:
			if(movedown_animation)
			{
				delete movedown_animation;
				movedown_animation = 0;
			}
			movedown_animation = new animation();
			movedown_animation->SetTexture(texture);
			movedown_animation->ClearFrames();
			for(unsigned int loop = 0; loop < frames.size(); ++loop)
			{
				movedown_animation->AddNewFrame(frames[loop]);
			}
			break;
		case ANIM_MOVE_UP:
			if(moveup_animation)
			{
				delete moveup_animation;
				moveup_animation = 0;
			}
			moveup_animation = new animation();
			moveup_animation->SetTexture(texture);
			moveup_animation->ClearFrames();
			for(unsigned int loop = 0; loop < frames.size(); ++loop)
			{
				moveup_animation->AddNewFrame(frames[loop]);
			}
			break;
		case ANIM_MOVE_SIT:
			// TODO: добавить анимацию приседа
			break;
		case ANIM_MOVE_JUMP:
			// TODO: добавить анимацию прыжка
			break;
		case ANIM_HOLDING:
			// TODO: добавить анимацию ожидания
			break;
		case ANIM_SHOOT_RIGHT:
			// TODO: добавить анимацию выстрела вправо
			break;
		case ANIM_SHOOT_LEFT:
			// TODO: добавить анимацию выстрела влево
			break;
		case ANIM_SHOOT_DOWN:
			// TODO: добавить анимацию выстрела вниз
			break;
		case ANIM_SHOOT_UP:
			// TODO: добавить анимацию выстрела вверх
			break;
		case ANIM_WORKING_RIGHT:
			// TODO: добавить анимацию работы вправо
			break;
		case ANIM_WORKING_LEFT:
			// TODO: добавить анимацию работы влево
			break;
		case ANIM_WORKING_DOWN:
			// TODO: добавить анимацию работы вниз
			break;
		case ANIM_WORKING_UP:
			// TODO: добавить анимацию работы вверх
			break;
		default:
			break;
	}
}
