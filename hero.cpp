/*
 * hero.cpp
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#include "hero.h"

using namespace std;

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
	static_anim_speed = HERO_ANIM_SPEED;
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

	body = new collision_body();
	//body->SetAABBBox(Box);

	//jumped = false;

	last_state = MOVE_NONE;
}
hero::~hero()
{
	if(static_texture)
	{
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
	if(body)
	{
		delete body;
		body = 0;
	}
	Game = 0;

#ifdef DEBUG_SYS
	cout << "One hero deleted..." << endl;
#endif
}

void hero::move(int direction, int animation, int animpos)
{
	if((animation == ANIM_UNKNOWN)||(Anims.empty()))
	{
		if(direction == MOVE_RIGHT)
			Box.X+=	static_anim_speed;
		else if(direction == MOVE_LEFT)
			Box.X-=static_anim_speed;
		else if(direction == MOVE_UP)
			Box.Y-=static_anim_speed;
		else if(direction == MOVE_DOWN)
			Box.Y+=static_anim_speed;

		// Пока блочим движение тела
		// 	но в дальнейшем нужно менять камеру при движении
		if(Game && Game->Graphics && static_texture)
		{
			if(Box.X > (Game->Graphics->GetScreenWidth()- static_texture->Width()))
				Box.X-=static_anim_speed;
			if(Box.X < 0)
				Box.X+=static_anim_speed;
			if(Box.Y > (Game->Graphics->GetScreenHeigth() - static_texture->Heigth()))
				Box.Y-=static_anim_speed;
			if(Box.Y < 0)
				Box.Y+=static_anim_speed;
		}
	}
	else
	{
		if(animpos==0)
		{
			AnimIter = Anims.begin();
			while(AnimIter != Anims.end())
			{
				if(animation == AnimIter->first)
				{
					if(animation == ANIM_MOVE_RIGHT)
						Box.X+=AnimIter->second.speed;
					else if(animation == ANIM_MOVE_LEFT)
						Box.X-=AnimIter->second.speed;
					else if(animation == ANIM_MOVE_UP)
						Box.Y-=AnimIter->second.speed;
					else if(animation == ANIM_MOVE_DOWN)
						Box.Y+=AnimIter->second.speed;
					else
						break;

					// Пока блочим движение тела
					// 	но в дальнейшем нужно менять камеру при движении
					if(Game && Game->Graphics)
					{
						if(Box.X > Game->Graphics->GetScreenWidth())
							Box.X-=AnimIter->second.speed;
						if(Box.X < 0)
							Box.X+=AnimIter->second.speed;
						if(Box.Y > Game->Graphics->GetScreenHeigth())
							Box.Y-=AnimIter->second.speed;
						if(Box.Y < 0)
							Box.Y+=AnimIter->second.speed;
					}

					AnimIter->second.pAnim->Update();
					break;
				}
				++AnimIter;
			}
		}
		else
		{
			if(animation == ANIM_MOVE_RIGHT)
				Box.X+=	Anims[animpos].speed;
			else if(animation == ANIM_MOVE_LEFT)
				Box.X-=Anims[animpos].speed;
			else if(animation == ANIM_MOVE_UP)
				Box.Y-=Anims[animpos].speed;
			else if(animation == ANIM_MOVE_DOWN)
				Box.Y+=Anims[animpos].speed;

			// Пока блочим движение тела
			// 	но в дальнейшем нужно менять камеру при движении
			if(Game && Game->Graphics)
			{
				if(Box.X > Game->Graphics->GetScreenWidth())
					Box.X-=Anims[animpos].speed;
				if(Box.X < 0)
					Box.X+=Anims[animpos].speed;
				if(Box.Y > Game->Graphics->GetScreenHeigth())
					Box.Y-=Anims[animpos].speed;
				if(Box.Y < 0)
					Box.Y+=Anims[animpos].speed;
			}
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

	PE_Rect OldBox = Box;
	int move_type = MOVE_NONE;	// Необходимо для движения
	last_state = MOVE_NONE;		// Необходимо для учёта порядка движений по приоритетам

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

	// Сначала проверяем нажате клавиш и забиваем переменную last_state
	if(Game->Input->IsKeyDown(KEY_RIGHT) || Game->Input->IsKeyHeld(KEY_RIGHT))
	{
		if(last_state < MOVE_RIGHT)
			last_state = MOVE_RIGHT;
		move_type = MOVE_RIGHT;
	}
	else if(Game->Input->IsKeyDown(KEY_LEFT) || Game->Input->IsKeyHeld(KEY_LEFT))
	{
		if(last_state < MOVE_LEFT)
			last_state = MOVE_LEFT;
		move_type = MOVE_LEFT;
	}
	else if(Game->Input->IsKeyDown(KEY_DOWN) || Game->Input->IsKeyHeld(KEY_DOWN))
	{
		if(last_state < MOVE_DOWN)
			last_state = MOVE_DOWN;
		move_type = MOVE_DOWN;
	}
	else if(Game->Input->IsKeyHeld(KEY_UP) || Game->Input->IsKeyHeld(KEY_UP))
	{
		if(last_state < MOVE_UP)
			last_state = MOVE_UP;
		move_type = MOVE_UP;
	}

	// Обрабатываем движение
	if(move_type > MOVE_NONE)
	{
		// Только если движение было
		if(!Anims.empty())
		{
			// Массив анимаций не пуст
			int loop = 0;
			bool animed = false;	// Движение анимировано

			for(AnimIter = Anims.begin(); AnimIter != Anims.end(); ++AnimIter)
			{
				if(AnimIter->first == move_type)
				{
					// Анимация найдена - двигаемся с нужной скоростью
					move(move_type, AnimIter->first, loop);
					animed = true;
					break;
				}
				loop++;
			}
			if(!animed)
			{
				// Анимация не найдена - двигаемся без анимации
				move(move_type);
			}
		}
		else
		{
			// Анимаций нет - двигаемся без анимации
			move(move_type);
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

	//body->SetAABBBox(Box);
	//body->
}
void hero::render()
{
	bool moved = false;

	if(!Anims.empty() && (last_state != MOVE_NONE))
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

// Устанавливаем статик текстуру и скорость текстуры, как четверть ширины
void hero::SetTexture(image *texture)
{
	static_texture = texture;
}
void hero::SetStaticSpeed(int speed)
{
	static_anim_speed = speed;
}
void hero::SetGame(game *gm)
{
	Game = gm;
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
int hero::GetStaticSpeed()
{
	return static_anim_speed;
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
collision_body *hero::GetCollisionBody()
{
	return body;
}
