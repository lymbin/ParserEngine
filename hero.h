/*
 * hero.h
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#ifndef HERO_H_
#define HERO_H_

#include "PE.h"
#include "Inventory.h"
#include "GameObject.h"

const unsigned int MAX_HERO_ANIM_SPEED = 30;

enum eCharacterType
{
	eCharacterType_Hero,
	eCharacterType_Ally,
	eCharacterType_NPC,
	eCharacterType_Enemy,
	eCharacterType_LastEnum
};

class iCharacter : public iGameObject
{
protected:
	eCharacterType mCharacterType;

	sStaticTexture mTexture;

	std::map <int, sAnim > mAnims;
	std::map <int, sAnim >::iterator mAnimIter;

	int miBasicAttackDamage;
	int miBasicDefence;

	std::vector <inventory_item > mItems;
	inventory_item mArmor;
	inventory_item mWeapon;

	int mLastState;

public:
	iCharacter(eCharacterType aType, std::string asName, int alHP);
	virtual ~iCharacter();

	void SetStaticTexture(cTexture *apTexture);

	void SetBasicAttackDamage(int alBasicAttackDamage);
	void SetBasicDefence(int alBasicDefence);

	void SetScaledAndRotate(float Scaled, float Rotate);

	void AddAnim(int alAnimType, cTexture *apTexture, std::vector< PE_Rect > aFrames);
	void SetAnimSpeed(int alAnimType, int alSpeed);

	virtual void Move(int alDirection, int alAnimation, int alAnimpos) {}
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}

	virtual void OnUpdate(){}
	virtual void OnRender(){}

	sAnim GetAnim(int alAnimType);

	cTexture *GetTexture();
	float GetScaledMultiplier();
	float GetRotateDegrees();

	inventory_item GetArmor();
	inventory_item GetWeapon();

};

class hero : public iCharacter
{

public:
	hero(std::string nam = "Timmi", int hp = 100);
	~hero();

	void Move(int aiDirection, int aiAnimation = ANIM_UNKNOWN, int aiAnimpos = 0);
	void Jump();
	void Sit();
	void Shoot();

	void OnUpdate();
	void OnRender();
};


#endif /* HERO_H_ */
