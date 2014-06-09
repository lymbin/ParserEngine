/*
 * hero.cpp
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#include "hero.h"

using namespace std;


float sStaticTexture::GetRealWidth()
{
	float result = mpTexture->GetWidth()*mfScaledMultiplier;
	// TODO: манипуляции с rotate_degrees
	return result;
}
float sStaticTexture::GetRealHeigth()
{
	float result = mpTexture->GetHeigth()*mfScaledMultiplier;
	// TODO: манипуляции с rotate_degrees
	return result;
}

iCharacter::iCharacter(eCharacterType aType, std::string asName, int alHP) : iGameObject(eGameObjectType_Character, asName, alHP)
{
	miBasicAttackDamage = 0;
	miBasicDefence = 0;

	mLastState = MOVE_NONE;
	mCharacterType = aType;


	mTexture.mpTexture = 0;
	mTexture.mfScaledMultiplier = 1.0;
	mTexture.mfRotateDegrees = 0.0;


	mAnims.clear();
	mItems.clear();

	mArmor.itemName = "Skin";
	mArmor.main_classification = TEST_ROBE;
	mArmor.sub_classification = TEST_ROBE_NONE;

	mWeapon.itemName = "Hand";
	mWeapon.main_classification = TEST_WEAPON;
	mWeapon.sub_classification = TEST_WEAPON_NONE;

}
iCharacter::~iCharacter()
{

}
void iCharacter::SetStaticTexture(cTexture *apTexture)
{
	mTexture.mpTexture = apTexture;
}
void iCharacter::SetBasicAttackDamage(int aiBasicAttackDamage)
{
	miBasicAttackDamage = aiBasicAttackDamage;
}
void iCharacter::SetBasicDefence(int aiBasicDefence)
{
	miBasicDefence = aiBasicDefence;
}

void iCharacter::AddAnim(int aiAnimType, cTexture *apTexture, std::vector< PE_Rect > aFrames)
{
	bool founded = false;

	if(!mAnims.empty())
	{
		mAnimIter = mAnims.begin();
		while(mAnimIter != mAnims.end())
		{
			if(mAnimIter->first == aiAnimType)
			{
				delete mAnimIter->second.mpAnim;
				mAnimIter->second.mpAnim = new animation();
				mAnimIter->second.mpAnim->SetTexture(apTexture);
				mAnimIter->second.mpAnim->ClearFrames();
				for(unsigned int loop = 0; loop < aFrames.size(); ++loop)
				{
					mAnimIter->second.mpAnim->AddNewFrame(aFrames[loop]);
				}
				founded = true;
				break;
			}
			++mAnimIter;
		}
	}

	if(!founded)
	{
		mAnims[aiAnimType].mpAnim = new animation;
		mAnims[aiAnimType].miSpeed = 0;
		mAnims[aiAnimType].mpAnim->SetTexture(apTexture);
		mAnims[aiAnimType].mpAnim->ClearFrames();
		for(unsigned int loop = 0; loop < aFrames.size(); ++loop)
		{
			mAnims[aiAnimType].mpAnim->AddNewFrame(aFrames[loop]);
		}
	}
}
void iCharacter::SetScaledAndRotate(float Scaled, float Rotate)
{
	mTexture.mfScaledMultiplier = Scaled;

	if(Rotate >= 360)
	{
		do
		{
			Rotate-=360;
		}while(Rotate >= 360);
	}
	else if(Rotate <= -360)
	{
		do
		{
			Rotate+=360;
		}while(Rotate <= -360);
	}

	mTexture.mfRotateDegrees = Rotate;
}
float iCharacter::GetScaledMultiplier()
{
	return mTexture.mfScaledMultiplier;
}
float iCharacter::GetRotateDegrees()
{
	return mTexture.mfRotateDegrees;
}
void iCharacter::SetAnimSpeed(int aiAnimType, int aiSpeed)
{
	if(!mAnims.empty())
	{
		mAnimIter = mAnims.begin();
		while(mAnimIter != mAnims.end())
		{
			if(mAnimIter->first == aiAnimType)
			{
				mAnimIter->second.miSpeed = aiSpeed;
				break;
			}
			++mAnimIter;
		}
	}
}

sAnim iCharacter::GetAnim(int aiAnimType)
{
	if(!mAnims.empty())
	{
		mAnimIter = mAnims.begin();
		while(mAnimIter != mAnims.end())
		{
			if(mAnimIter->first == aiAnimType)
			{
				return mAnimIter->second;
				break;
			}
			++mAnimIter;
		}
	}
	sAnim NullAnim;
	NullAnim.mpAnim = 0;
	NullAnim.miSpeed = 0;

	return NullAnim;
}
cTexture *iCharacter::GetTexture()
{
	return mTexture.mpTexture;
}

inventory_item iCharacter::GetArmor()
{
	return mArmor;
}
inventory_item iCharacter::GetWeapon()
{
	return mWeapon;
}

hero::hero(std::string asName, int aiHP):iCharacter(eCharacterType_Hero, asName, aiHP)
{
	mpGame = 0;
}
hero::~hero()
{

#ifdef DEBUG_SYS
	cout << "One hero deleted..." << endl;
#endif
}

void hero::Move(int direction, int animation, int animpos)
{
	PE_Rect Box = GetBox();

	if((animation == ANIM_UNKNOWN)||(mAnims.empty()))
	{
		if(direction == MOVE_RIGHT)
			Box.X+=mlVelocity;
		else if(direction == MOVE_LEFT)
			Box.X-=mlVelocity;
		else if(direction == MOVE_UP)
			Box.Y-=mlVelocity;
		else if(direction == MOVE_DOWN)
			Box.Y+=mlVelocity;

		// Пока блочим движение тела
		// 	но в дальнейшем нужно менять камеру при движении
		if(mpGame && mpGame->Graphics && mTexture.mpTexture)
		{
			if(Box.X > (mpGame->Graphics->GetScreenWidth()- mTexture.mpTexture->GetWidth()*mTexture.mfScaledMultiplier))
				Box.X-=mlVelocity;
			if(Box.X < 0)
				Box.X+=mlVelocity;
			if(Box.Y > (mpGame->Graphics->GetScreenHeigth() - mTexture.mpTexture->GetHeigth()*mTexture.mfScaledMultiplier))
				Box.Y-=mlVelocity;
			if(Box.Y < 0)
				Box.Y+=mlVelocity;
		}
	}
	else
	{
		if(animpos==0)
		{
			mAnimIter = mAnims.begin();
			while(mAnimIter != mAnims.end())
			{
				if(animation == mAnimIter->first)
				{
					if(animation == ANIM_MOVE_RIGHT)
						Box.X+=(mlVelocity+mAnimIter->second.miSpeed);
					else if(animation == ANIM_MOVE_LEFT)
						Box.X-=(mlVelocity+mAnimIter->second.miSpeed);
					else if(animation == ANIM_MOVE_UP)
						Box.Y-=(mlVelocity+mAnimIter->second.miSpeed);
					else if(animation == ANIM_MOVE_DOWN)
						Box.Y+=(mlVelocity+mAnimIter->second.miSpeed);
					else
						break;

					// Пока блочим движение тела
					// 	но в дальнейшем нужно менять камеру при движении
					if(mpGame && mpGame->Graphics)
					{
						if(Box.X > mpGame->Graphics->GetScreenWidth())
							Box.X-=(mlVelocity+mAnimIter->second.miSpeed);
						if(Box.X < 0)
							Box.X+=(mlVelocity+mAnimIter->second.miSpeed);
						if(Box.Y > mpGame->Graphics->GetScreenHeigth())
							Box.Y-=(mlVelocity+mAnimIter->second.miSpeed);
						if(Box.Y < 0)
							Box.Y+=(mlVelocity+mAnimIter->second.miSpeed);
					}

					mAnimIter->second.mpAnim->Update();
					break;
				}
				++mAnimIter;
			}
		}
		else
		{
			if(animation == ANIM_MOVE_RIGHT)
				Box.X+=	(mlVelocity+mAnims[animpos].miSpeed);
			else if(animation == ANIM_MOVE_LEFT)
				Box.X-=(mlVelocity+mAnims[animpos].miSpeed);
			else if(animation == ANIM_MOVE_UP)
				Box.Y-=(mlVelocity+mAnims[animpos].miSpeed);
			else if(animation == ANIM_MOVE_DOWN)
				Box.Y+=(mlVelocity+mAnims[animpos].miSpeed);

			// Пока блочим движение тела
			// 	но в дальнейшем нужно менять камеру при движении
			if(mpGame && mpGame->Graphics)
			{
				if(Box.X > mpGame->Graphics->GetScreenWidth())
					Box.X-=(mlVelocity+mAnims[animpos].miSpeed);
				if(Box.X < 0)
					Box.X+=(mlVelocity+mAnims[animpos].miSpeed);
				if(Box.Y > mpGame->Graphics->GetScreenHeigth())
					Box.Y-=(mlVelocity+mAnims[animpos].miSpeed);
				if(Box.Y < 0)
					Box.Y+=(mlVelocity+mAnims[animpos].miSpeed);
			}
		}
	}
	SetBox(Box);
}
void hero::Jump()
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
void hero::Sit()
{
	/*
	 * TODO: код для анимации приседа - 1 картинка
	if(sit_animation)
	{
		sit_animation->Update();
	}
	*/
}
void hero::Shoot()
{
	/*
	 * TODO: код для выстрела
	 */
}
void hero::Update()
{
	if((!mpGame)||(!mpGame->Input))
		return;

	int move_type = MOVE_NONE;	// Необходимо для движения
	mLastState = MOVE_NONE;		// Необходимо для учёта порядка движений по приоритетам

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

	if(mpGame->Input->IsKeyDown(KEY_EQUALS) || mpGame->Input->IsKeyHeld(KEY_EQUALS))
	{
		unsigned int speed = GetVelocity();
		if(speed < MAX_HERO_ANIM_SPEED)
		{
			speed+=1;
			SetVelocity(speed);
		}
	}
	else if(mpGame->Input->IsKeyDown(KEY_MINUS) || mpGame->Input->IsKeyHeld(KEY_MINUS))
	{
		unsigned int speed = GetVelocity();
		if(speed > 1)
		{
			speed-=1;
			SetVelocity(speed);
		}
	}

	// Сначала проверяем нажате клавиш и забиваем переменную last_state
	if(mpGame->Input->IsKeyDown(KEY_RIGHT) || mpGame->Input->IsKeyHeld(KEY_RIGHT))
	{
		if(mLastState < MOVE_RIGHT)
			mLastState = MOVE_RIGHT;
		move_type = MOVE_RIGHT;
	}
	else if(mpGame->Input->IsKeyDown(KEY_LEFT) || mpGame->Input->IsKeyHeld(KEY_LEFT))
	{
		if(mLastState < MOVE_LEFT)
			mLastState = MOVE_LEFT;
		move_type = MOVE_LEFT;
	}
	else if(mpGame->Input->IsKeyDown(KEY_DOWN) || mpGame->Input->IsKeyHeld(KEY_DOWN))
	{
		if(mLastState < MOVE_DOWN)
			mLastState = MOVE_DOWN;
		move_type = MOVE_DOWN;
	}
	else if(mpGame->Input->IsKeyHeld(KEY_UP) || mpGame->Input->IsKeyHeld(KEY_UP))
	{
		if(mLastState < MOVE_UP)
			mLastState = MOVE_UP;
		move_type = MOVE_UP;
	}

	// Обрабатываем движение
	if(move_type > MOVE_NONE)
	{
		// Только если движение было
		if(!mAnims.empty())
		{
			// Массив анимаций не пуст
			int loop = 0;
			bool animed = false;	// Движение анимировано

			mAnimIter = mAnims.begin();

			for( ; mAnimIter != mAnims.end(); ++mAnimIter)
			{
				if(mAnimIter->first == move_type)
				{
					// Анимация найдена - двигаемся с нужной скоростью
					Move(move_type, mAnimIter->first, loop);
					animed = true;
					break;
				}
				loop++;
			}
			if(!animed)
			{
				// Анимация не найдена - двигаемся без анимации
				Move(move_type);
			}
		}
		else
		{
			// Анимаций нет - двигаемся без анимации
			Move(move_type);
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
	//CALLBACK(hero::CollisionHandler, this, &OldBox);
}
void hero::OnDraw()
{
	bool moved = false;
	PE_Rect Box = GetBox();
	if(!mAnims.empty() && (mLastState != MOVE_NONE))
	{
		mAnimIter = mAnims.begin();
		while(mAnimIter != mAnims.end())
		{
			if(!moved)
			{
				// Обрабатываем движения
				if(mAnimIter->first == mLastState)
				{
					if(mAnimIter->second.mpAnim)
						mAnimIter->second.mpAnim->Draw(Box.X, Box.Y);
					else
					{
						if(mTexture.mpTexture)
							mTexture.mpTexture->Draw(Box.X, Box.Y, mTexture.mfScaledMultiplier, mTexture.mfRotateDegrees);
					}

					moved = true;
				}
			}

			// Обрабатываем что-то ещё


			++mAnimIter;
		}
	}
	else
	{
		if(mTexture.mpTexture)
			mTexture.mpTexture->Draw(Box.X, Box.Y, mTexture.mfScaledMultiplier, mTexture.mfRotateDegrees);
	}
}
void hero::Reset()
{

}
void hero::OnStart()
{

}
void hero::OnExit()
{

}
void hero::SetGame(game *apGame)
{
	mpGame = apGame;
}
void hero::CollisionHandler(iCollisionBody *thisBody, PE_Rect CollidedRect, void *CollidedObject, void *data)
{
	//PE_Rect *Rect = (PE_Rect *)data;

	//thisBody->SetBox(*Rect);


	cout << "collided!" << endl;
}
