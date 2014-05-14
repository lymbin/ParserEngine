/*
 * PhysicsTypes.h
 *
 *  Created on: 14.05.2014
 *      Author: dmitry
 */

#ifndef PHYSICSTYPES_H_
#define PHYSICSTYPES_H_

enum ePassableConsist{
	COLLISION_UNPASSABLE,
	COLLISION_PASSABLE
};
enum eCollisionBodyType{
	COLLISION_BODY_UNKNOWN,
	COLLISION_BODY_GAME_OBJECT_DYNAMIC,
	COLLISION_BODY_GAME_OBJECT_STATIC,
	COLLISION_BODY_WALL,
};


#endif /* PHYSICSTYPES_H_ */
