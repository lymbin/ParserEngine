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

class hero
{
	game 		*Game;

	image 		*static_texture;

	animation 	*rmove_animation;
	animation 	*lmove_animation;
	animation 	*jump_animation;
	animation 	*sit_animation;

	int last_state;

	int anim_step;
	bool jumped;

	PE_Rect 	Box;

	int hit_points;
	std::string name;
	std::vector <inventory_item > items;
	inventory_item armor;
	inventory_item weapon;

public:
	hero(std::string nam = "Timmi", int hp = 100);
	~hero();

	void move(int pos);
	void jump();
	void sit();
	void shoot();

	void update();
	void render();

	void SetTexture(image 	*texture);
	void LoadTexture(std::string file);

	void SetAnim(int AnimType, image *texture, std::vector< PE_Rect > frames);

	//void SetHeroName();
	std::string GetHeroName();
	int	GetHealth();
	inventory_item GetArmor();
	inventory_item GetWeapon();

};


#endif /* HERO_H_ */
