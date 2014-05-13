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


struct sAnim
{
	animation *pAnim;
	int speed;
};
class hero
{
	game 		*Game;

	image 		*static_texture;

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

	std::string name;
	int hit_points;

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
	void SetAnimSpeed(int AnimType, int speed);

	//void SetHeroName();

	sAnim GetAnim(int AnimType);
	image *GetTexture();
	PE_Rect GetBox();

	std::string GetHeroName();
	int	GetHealth();
	inventory_item GetArmor();
	inventory_item GetWeapon();

};


#endif /* HERO_H_ */
