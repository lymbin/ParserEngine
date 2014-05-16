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
	/*
	moveright_animation = 0;
	moveleft_animation = 0;
	movedown_animation = 0;
	moveup_animation = 0;
	jump_animation = 0;
	sit_animation = 0;
	*/
	Anims.clear();

	items.clear();

	Box.X = 0;
	Box.Y = 0;
	Box.Heigth = 0;
	Box.Width = 0;

	body = new collision_body(COLLISION_UNPASSABLE);

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
	/*
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
	*/
	if(!Anims.empty())
	{
		for(AnimIter = Anims.begin(); AnimIter != Anims.end(); ++AnimIter)
		{
			if(AnimIter->second.pAnim)
			{
				delete AnimIter->second.pAnim;
				AnimIter->second.pAnim = 0;
			}
		}
		Anims.clear();
	}
	Game = 0;
}

void hero::move(int pos)
{
	// Двойная работа по поиску в карте - изменить
	if(!Anims.empty())
	{
		AnimIter = Anims.begin();
		while(AnimIter != Anims.end())
		{
			if(pos == AnimIter->first)
			{
				if(pos == ANIM_MOVE_RIGHT)
					Box.X+=AnimIter->second.speed;
				else if(pos == ANIM_MOVE_LEFT)
					Box.X-=AnimIter->second.speed;
				else if(pos == ANIM_MOVE_UP)
					Box.Y+=AnimIter->second.speed;
				else if(pos == ANIM_MOVE_DOWN)
					Box.Y-=AnimIter->second.speed;
				else
					break;

				// Пока блочим движение тела
				// 	но в дальнейшем нужно менять камеру при движении
				if(Box.X > SYS_WIDTH)
					Box.X-=AnimIter->second.speed;
				else if(Box.X < 0)
					Box.X+=AnimIter->second.speed;
				if(Box.Y > SYS_HEIGTH)
					Box.Y-=AnimIter->second.speed;
				else if(Box.Y < 0)
					Box.Y+=AnimIter->second.speed;

				AnimIter->second.pAnim->Update();
				break;
			}
			++AnimIter;
		}
	}


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
	if(!Anims.empty())
	{
		for(AnimIter = Anims.begin(); AnimIter != Anims.end(); ++AnimIter)
		{
			if((AnimIter->first == ANIM_MOVE_RIGHT) && (Game->Input->IsKeyDown(KEY_RIGHT) || Game->Input->IsKeyHeld(KEY_RIGHT)))
			{
				if(last_state < ANIM_MOVE_RIGHT)
					last_state = ANIM_MOVE_RIGHT;
				move(ANIM_MOVE_RIGHT);
			}
			else if((AnimIter->first == ANIM_MOVE_LEFT) && (Game->Input->IsKeyDown(KEY_LEFT) || Game->Input->IsKeyHeld(KEY_LEFT)))
			{
				if(last_state < ANIM_MOVE_LEFT)
					last_state = ANIM_MOVE_LEFT;
				move(ANIM_MOVE_LEFT);
			}
			else if((AnimIter->first == ANIM_MOVE_DOWN) && (Game->Input->IsKeyDown(KEY_DOWN) || Game->Input->IsKeyHeld(KEY_DOWN)))
			{
				if(last_state < ANIM_MOVE_DOWN)
					last_state = ANIM_MOVE_DOWN;
				move(ANIM_MOVE_DOWN);
			}
			else if((AnimIter->first == ANIM_MOVE_UP) && (Game->Input->IsKeyHeld(KEY_UP) || Game->Input->IsKeyHeld(KEY_UP)))
			{
				if(last_state < ANIM_MOVE_UP)
					last_state = ANIM_MOVE_UP;
				move(ANIM_MOVE_UP);
			}
		}
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

	bool moved = false;

	if(!Anims.empty() || (last_state == MOVE_NONE))
	{
		AnimIter = Anims.begin();
		while(AnimIter != Anims.end())
		{
			if(!moved)
			{
				// Обрабатываем движения
				if(AnimIter->first == last_state)
				{
					if(AnimIter->second.pAnim)
						AnimIter->second.pAnim->Draw(Box.X, Box.Y);
					else
					{
						if(static_texture)
							static_texture->Draw(Box.X, Box.Y);
					}

					moved = true;
				}
			}

			// Обрабатываем что-то ещё


			++AnimIter;
		}
	}
	else
	{
		if(static_texture)
			static_texture->Draw(Box.X, Box.Y);
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
	bool founded = false;

	if(!Anims.empty())
	{
		AnimIter = Anims.begin();
		while(AnimIter != Anims.end())
		{
			if(AnimIter->first == AnimType)
			{
				delete AnimIter->second.pAnim;
				AnimIter->second.pAnim = new animation();
				AnimIter->second.pAnim->SetTexture(texture);
				AnimIter->second.pAnim->ClearFrames();
				for(unsigned int loop = 0; loop < frames.size(); ++loop)
				{
					AnimIter->second.pAnim->AddNewFrame(frames[loop]);
				}
				founded = true;
				break;
			}
			++AnimIter;
		}
	}

	if(!founded)
	{
		Anims[AnimType].pAnim = new animation;
		Anims[AnimType].speed = 0;
		Anims[AnimType].pAnim->SetTexture(texture);
		Anims[AnimType].pAnim->ClearFrames();
		for(unsigned int loop = 0; loop < frames.size(); ++loop)
		{
			Anims[AnimType].pAnim->AddNewFrame(frames[loop]);
		}
	}
}
void hero::SetAnimSpeed(int AnimType, int speed)
{
	if(!Anims.empty())
	{
		AnimIter = Anims.begin();
		while(AnimIter != Anims.end())
		{
			if(AnimIter->first == AnimType)
			{
				AnimIter->second.speed = speed;
				break;
			}
			++AnimIter;
		}
	}
}
sAnim hero::GetAnim(int AnimType)
{
	if(!Anims.empty())
	{
		AnimIter = Anims.begin();
		while(AnimIter != Anims.end())
		{
			if(AnimIter->first == AnimType)
			{
				return AnimIter->second;
			}
			++AnimIter;
		}
	}
	sAnim nullec;
	nullec.pAnim = 0;
	nullec.speed = 0;

	return nullec;
}
image *hero::GetTexture()
{
	return static_texture;
}
PE_Rect hero::GetBox()
{
	return Box;
}

std::string hero::GetHeroName()
{
	return name;
}
int	hero::GetHealth()
{
	return hit_points;
}
inventory_item hero::GetArmor()
{
	return armor;
}
inventory_item hero::GetWeapon()
{
	return weapon;
}
