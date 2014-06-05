/*
 * Character.h
 *
 *  Created on: 02.06.2014
 *      Author: dmitry
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

class iCharacter;

#include "Inventory.h"
#include "GameObject.h"

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

	virtual void OnDraw() {}	// Перерисовка
	virtual void Reset() {}		// Сброс настроек
	virtual void OnStart() {}	// Установка начальных данных
	virtual void OnExit() {}	// Выполняем при выходе, на последней итерации фреймов или при удалении
	virtual void Update() {}	// Обновление данных

	sAnim GetAnim(int alAnimType);

	cTexture *GetTexture();
	float GetScaledMultiplier();
	float GetRotateDegrees();

	inventory_item GetArmor();
	inventory_item GetWeapon();

	eCharacterType mCharacterType;

};


#endif /* CHARACTER_H_ */
