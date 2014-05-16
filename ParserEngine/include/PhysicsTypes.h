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

enum eCollision{
	COLLISION_INSIDE,
	COLLISION_OUTSIDE,
	COLLISION_INTERSECT
};

#endif /* PHYSICSTYPES_H_ */
