/*
 * game.h
 *
 *  Created on: 18.04.2014
 *      Author: dmitry
 */

#ifndef GAME_H_
#define GAME_H_

#include "engine.h"

//Нижние дефайны пока никак не используем
#define LEVEL_MAINMENU  0
#define LEVEL_GAMELEVEL 1

const int GAME_GUI_HEIGTH = 200;

const int GAME_HERO_HP = 100;
const std::string GAME_HERO_NAME = "Timmi";

struct MainMenu
{
	image 	*background;
	text 	*title;
	//image *button_start;
	//image *button_exit;
};




enum item_main_classification {
	TEST_ITEM ,
	TEST_WEAPON ,
	TEST_ROBE,
	TEST_AMMO
};
enum item_classification {
	TEST_ITEM_UNKNOWN ,
	TEST_ITEM2,
	TEST_ITEM3,
	TEST_ITEM_VLADISLAU_HAND
};
enum weapon_classification {
	TEST_WEAPON_NONE,
	TEST_WEAPON_GUN,
	TEST_WEAPON_PISTOL,
	TEST_WEAPON_ROCKET,
};
enum robe_classification {
	TEST_ROBE_NONE,
	TEST_ROBE_1 ,
	TEST_ROBE_2
};
enum ammo_classification {
	TEST_AMMO_1,
	TEST_AMMO_PIE,
	TEST_AMMO_BULLETS
};
enum move_type{
	MOVE_UNKNOWN,
	MOVE_RIGTH,
	MOVE_LEFT,
	MOVE_JUMP,
	MOVE_SIT
};
struct inventory_item
{
	int main_classification;
	int sub_classification;
	std::string itemName;

	unsigned int item_stack;
};

struct bottom_gui
{
	image *texture;
	text *static_text;
	text *dynamic_text;
};
class game_gui
{
	bottom_gui bottom;
public:
	game_gui();
	~game_gui();
	void update();
	void render();
};
class hero
{
	game 		*Game;

	image 		*static_texture;

	animation 	*move_animation;
	animation 	*jump_animation;
	animation 	*sit_animation;

	std::vector <int> state_manager;

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
class game: public engine
{
	static bool quit;
	static bool timer_running;
	MainMenu Mmenu;
	game_gui *Gui;
	hero *Hero;

public:
	int LoadTextures();
	void FreeTextures();
	void update();
	void render();
	void MainLoop();
	game();
	~game();
};



#endif /* GAME_H_ */
