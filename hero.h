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

const int HERO_ANIM_SPEED = 20;

struct sAnim
{
	animation *pAnim;
	int speed;

	float scaled_multiplier;
	float rotate_degrees;
};
struct sStaticTexture
{
	image 		*texture;
	int 		static_anim_speed;

	float scaled_multiplier;
	float rotate_degrees;

	float GetRealWidth();
	float GetRealHeigth();
};
class hero
{
	game 		*Game;

	sStaticTexture StaticTexture;

	std::map <int, sAnim > Anims;
	std::map <int, sAnim >::iterator AnimIter;
	int last_state;

	/*
	animation 	*moveright_animation;
	animation 	*moveleft_animation;
	animation 	*moveup_animation;
	animation 	*movedown_animation;

	animation 	*jump_animation;
	animation 	*sit_animation;

	bool jumped;
	*/

	PE_Rect 	Box;

	collision_body *body;

	std::string name;
	int hit_points;

	std::vector <inventory_item > items;
	inventory_item armor;
	inventory_item weapon;

public:
	hero(std::string nam = "Timmi", int hp = 100);
	~hero();

	void move(int direction, int animation = ANIM_UNKNOWN, int animpos = 0);
	void jump();
	void sit();
	void shoot();

	void update();
	void render();

	void SetTexture(image 	*texture);

	void SetStaticSpeed(int speed);

	void SetScaledAndRotate(float Scaled = 1, float Rotate = 0);

	void SetBox(float W = 0, float H = 0, float X = 0, float Y = 0);

	void SetAnim(int AnimType, image *texture, std::vector< PE_Rect > frames);
	void SetAnimSpeed(int AnimType, int speed);

	void SetGame(game *gm);

	//void SetHeroName();

	sAnim GetAnim(int AnimType);
	image *GetTexture();
	PE_Rect GetBox();
	collision_body *GetCollisionBody();

	int GetStaticSpeed();

	float GetScaledMultiplier();
	float GetRotateDegrees();

	std::string GetHeroName();
	int	GetHealth();
	inventory_item GetArmor();
	inventory_item GetWeapon();

};


#endif /* HERO_H_ */
