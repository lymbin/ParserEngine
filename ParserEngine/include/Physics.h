/*
 * Physics.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "SystemConstants.h"
#include "GraphicTypes.h"
#include "PhysicsTypes.h"
#include "Collision.h"

class cPhysics
{
public:
	cPhysics(){}
	~cPhysics(){}

	// Инициализация системы
	int Init(){return 0;}
};

#endif /* PHYSICS_H_ */
