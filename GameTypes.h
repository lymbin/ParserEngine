/*
 * GameTypes.h
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#ifndef GAMETYPES_H_
#define GAMETYPES_H_

#include "game.h"

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
	TEST_AMMO_2,
	TEST_AMMO_BULLETS
};
enum move_type{
	MOVE_NONE,
	MOVE_RIGTH,
	MOVE_LEFT,
	MOVE_SIT,
	MOVE_JUMP,
	MOVE_UNKNOWN
};



#endif /* GAMETYPES_H_ */
