/*
 * hero.h
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#ifndef HERO_H_
#define HERO_H_

#include "game.h"
#include "Inventory.h"

const unsigned int MAX_HERO_ANIM_SPEED = 30;

enum eCharacterType
{
	eCharacterType_Hero,
	eCharacterType_Ally,
	eCharacterType_NPC,
	eCharacterType_Enemy,
	eCharacterType_LastEnum
};


struct sAnim
{
	animation *mpAnim;
	unsigned int miSpeed;

	float mfScaledMultiplier;
	float mfRotateDegrees;
};
struct sStaticTexture
{
	cTexture	*mpTexture;
	unsigned int miAnimSpeed;

	float mfScaledMultiplier;
	float mfRotateDegrees;

	float GetRealWidth();
	float GetRealHeigth();
};
class iCharacterBody
{
protected:
	PE_Rect 		Box;
	collision_body *body;

public:
	iCharacterBody();
	virtual ~iCharacterBody();

	void SetBox(float W, float H, float X, float Y);
	void SetBody(collision_body *apBody);

	PE_Rect 		GetBox();
	collision_body *GetCollisionBody();
};

class iCharacter : public iCharacterBody
{
protected:
	game *mpGame;
	eCharacterType mType;

	sStaticTexture mTexture;
	unsigned int miVelocity;

	std::map <int, sAnim > mAnims;
	std::map <int, sAnim >::iterator mAnimIter;

	std::string msName;
	int miHitPoints;

	int miBasicAttackDamage;
	int miBasicDefence;

	std::vector <inventory_item > mItems;
	inventory_item mArmor;
	inventory_item mWeapon;

	int mLastState;

public:
	iCharacter(eCharacterType aType, std::string asName, int aiHP);
	virtual ~iCharacter();

	void Update();
	void Render();

	void SetStaticTexture(cTexture *apTexture);
	void SetVelocity(unsigned int aiVelocity);
	void SetName(std::string asName);
	void SetHitPoints(int aiHP);
	void SetBasicAttackDamage(int aiBasicAttackDamage);
	void SetBasicDefence(int aiBasicDefence);
	void SetGame(game *apGame);

	void SetScaledAndRotate(float Scaled, float Rotate);

	void AddAnim(int aiAnimType, cTexture *apTexture, std::vector< PE_Rect > aFrames);
	void SetAnimSpeed(int aiAnimType, int aiSpeed);

	virtual void Move(int aiDirection, int aiAnimation, int aiAnimpos) {}
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}

	virtual void OnUpdate(){}
	virtual void OnRender(){}


	sAnim GetAnim(int aiAnimType);

	cTexture *GetTexture();
	float GetScaledMultiplier();
	float GetRotateDegrees();

	std::string GetHeroName();
	int	GetHealth();
	inventory_item GetArmor();
	inventory_item GetWeapon();

	unsigned int GetVelocity();

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
