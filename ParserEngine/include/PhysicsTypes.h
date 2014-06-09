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

enum eBoundingVolume{
	COLLISION_AABB,
	COLLISION_OBB,
	COLLISION_AABB_OBB
};

enum eBodyPosition{
	eBodyPosition_Inside,
	eBodyPosition_Outside,
	eBodyPosition_Intersect,
	eBodyPosition_LastEnum
};
enum eCollisionSystem{
	eCollisionSystem_Simple,
	eCollisionSystem_Hard1,
	eCollisionSystem_Hard2,
	eCollisionSystem_Hard3,
	eCollisionSystem_LastEnum
};
#endif /* PHYSICSTYPES_H_ */
