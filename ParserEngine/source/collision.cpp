/*
 * collision.cpp
 *
 *  Created on: 14.05.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

collision_AABB::collision_AABB()
{
	AABBBodyBox.Heigth = SYS_HEIGTH;
	AABBBodyBox.Width = SYS_WIDTH;
	AABBBodyBox.X = 0;
	AABBBodyBox.Y = 0;

	MinPoint.x = 0;
	MinPoint.y = 0;
	MaxPoint.x = SYS_WIDTH;
	MaxPoint.y = SYS_HEIGTH;
}

//-----------------------------------------------------------------------

collision_AABB::~collision_AABB()
{

}

//-----------------------------------------------------------------------

collision_OBB::collision_OBB()
{

}

//-----------------------------------------------------------------------

collision_OBB::~collision_OBB()
{

}

//-----------------------------------------------------------------------

collision_body::collision_body()
{
	Collision = 0;
	CurrentLayer = 0;
	BodyType = COLLISION_BODY_UNKNOWN;
	CollisionPass = COLLISION_UNPASSABLE;
	CollisionType = COLLISION_AABB;

}

//-----------------------------------------------------------------------

collision_body::collision_body(int ColPass, int ColType, int Type)
{
	Collision = 0;
	CurrentLayer = 0;

	BodyType = Type;
	CollisionPass = ColPass;
	CollisionType = ColType;
	//BodyBox = Box;
}

//-----------------------------------------------------------------------

collision_body::~collision_body()
{
	if(CurrentLayer)
	{

	}
	CurrentLayer = 0;
	Collision = 0;
}

//-----------------------------------------------------------------------

collision_layer::collision_layer(PE_Rect Box)
{
	LayerBorder = Box;
	bodies.clear();
}

//-----------------------------------------------------------------------

collision_layer::collision_layer(GLfloat W, GLfloat H, GLfloat X, GLfloat Y)
{
	LayerBorder.Width = W;
	LayerBorder.Heigth = H;
	LayerBorder.X = X;
	LayerBorder.Y = Y;

	bodies.clear();
}

//-----------------------------------------------------------------------

collision_layer::~collision_layer()
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop])
			{
				delete bodies[loop];
			}
		}
		bodies.clear();
	}
}

//-----------------------------------------------------------------------

collision::collision()
{
#ifdef DEBUGGING
	IsEnabled = true;
#endif
	layers.clear();
}

//-----------------------------------------------------------------------

collision::~collision()
{
	DeleteAll();
#ifdef DEBUG_SYS
	cout << "Collision clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

bool collision_AABB::OverlapsAABB(collision_AABB aabb)
{
	// TODO: упростить с помощью векторов

	GLfloat leftBox, leftChecked;
	GLfloat rightBox, rightChecked;
	GLfloat	topBox, topChecked;
	GLfloat bottomBox, bottomChecked;

	leftBox = aabb.AABBBodyBox.X;
	rightBox = aabb.AABBBodyBox.X + aabb.AABBBodyBox.Width;
	topBox = aabb.AABBBodyBox.Y;
	bottomBox = aabb.AABBBodyBox.Y + aabb.AABBBodyBox.Heigth;

	leftChecked = AABBBodyBox.X;
	rightChecked = AABBBodyBox.X + AABBBodyBox.Width;
	topChecked = AABBBodyBox.Y;
	bottomChecked = AABBBodyBox.Y + AABBBodyBox.Heigth;

	if(bottomBox <= topChecked)
	{
		return false;
	}

	if(topBox >= bottomChecked)
	{
		return false;
	}

	if(rightBox <= leftChecked)
	{
		return false;
	}

	if(leftBox >= rightChecked)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------

bool collision_AABB::OverlapsOBB(collision_OBB obb)
{

	return true;
}

//-----------------------------------------------------------------------
/*
bool collision_OBB::OverlapsAABB(collision_AABB aabb)
{
	return true;
}

//-----------------------------------------------------------------------

bool collision_OBB::OverlapsOBB(collision_OBB obb)
{
	return true;
}
*/
//-----------------------------------------------------------------------

void collision_body::SetBodyType(int Type)
{
	BodyType = Type;
}

//-----------------------------------------------------------------------

void collision_body::SetCollisionType(int Type)
{
	CollisionType = Type;
}

//-----------------------------------------------------------------------

// Устанавливаем тип столкновения
void collision_body::SetCollisionPass(unsigned int Pass)
{
	CollisionPass = Pass;
}

//-----------------------------------------------------------------------

// Устанавливем слой
void collision_body::SetLayer(collision_layer *layer)
{
	if(CurrentLayer)
	{
		CurrentLayer->EraseBody(this);
	}
	CurrentLayer = layer;
}

//-----------------------------------------------------------------------

int collision_body::GetBodyType()
{
	return BodyType;
}

//-----------------------------------------------------------------------

unsigned int collision_body::IsPassable()
{
	return CollisionPass;
}

//-----------------------------------------------------------------------

int collision_body::GetCollisionType()
{
	return CollisionType;
}

//-----------------------------------------------------------------------

collision_layer *collision_body::GetCollisionLayer()
{
	return CurrentLayer;
}

//-----------------------------------------------------------------------

void collision_layer::AddCollisionBody(collision_body *body)
{
	if(!CheckBodyInLayer(body))
	{
		bodies.push_back(body);
	}
}

//-----------------------------------------------------------------------

bool collision_layer::CheckBodyInLayer(collision_body *body)
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop] == body)
				return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------

void collision_layer::EraseBody(collision_body *body)
{
	if(CheckBodyInLayer(body))
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop] == body)
			{
				bodies.erase(bodies.begin() + loop);
			}
		}
	}
}

//-----------------------------------------------------------------------

void collision_layer::DeleteAll()
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			delete bodies[loop];
		}
		bodies.clear();
	}
}

//-----------------------------------------------------------------------

void collision_layer::DeleteCollisionBody(collision_body *body)
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop] == body)
			{
				delete bodies[loop];
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------

void collision_layer::SetLayerBorder(PE_Rect Border)
{
	LayerBorder.Heigth = Border.Heigth;
	LayerBorder.Width = Border.Width;
	LayerBorder.X = Border.X;
	LayerBorder.Y = Border.Y;
}

//-----------------------------------------------------------------------

PE_Rect collision_layer::GetLayerBorder()
{
	return LayerBorder;
}

//-----------------------------------------------------------------------

int collision::init()
{
	return 0;
}

//-----------------------------------------------------------------------

void collision::NewCollisionBody(unsigned int LayerId, int ColPass, int ColType, int Type)
{
	collision_body *newcollisionbody = new collision_body(ColPass, ColType, Type);
	layers[LayerId]->bodies.push_back(newcollisionbody);
}

//-----------------------------------------------------------------------

void collision::DeleteAll()
{
	if(!layers.empty())
	{
		for(unsigned int loop = 0; loop < layers.size(); ++loop)
		{
			if(layers[loop])
			{
				delete layers[loop];
			}
		}
		layers.clear();
	}
}

//-----------------------------------------------------------------------

// Запускаем или отключаем систему столкновений
#ifdef DEBUGGING
void collision::SwitchCollision()
{
	IsEnabled = !IsEnabled;
}
#endif

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

// Оптимизация массива тел столкновений
void collision_layer::OptimizeCollisions()
{
	// TODO: проверить
	if(bodies.empty())
		return;

	SortCollisions();
	/*
	GLfloat leftA, 	leftB;
	GLfloat rightA, rightB;
	GLfloat topA, 	topB;
	GLfloat bottomA, bottomB;

	for(unsigned int loop1 = 0; loop1 < bodies.size(); ++loop1)
	{
		if(bodies[loop1]->BodyType == COLLISION_PASSABLE)
		{
			continue;
		}

		leftA = bodies[loop1]->BodyBox.X;
		rightA = bodies[loop1]->BodyBox.X + bodies[loop1]->BodyBox.Width;
		topA= bodies[loop1]->BodyBox.Y;
		bottomA = bodies[loop1]->BodyBox.Y + bodies[loop1]->BodyBox.Heigth;

		for(unsigned int loop2 = loop1; loop2 < bodies.size(); ++loop2)
		{
			leftB = bodies[loop2]->BodyBox.X;
			rightB = bodies[loop2]->BodyBox.X + bodies[loop2]->BodyBox.Width;
			topB = bodies[loop2]->BodyBox.Y;
			bottomB = bodies[loop2]->BodyBox.Y + bodies[loop2]->BodyBox.Heigth;

			if((bottomB <= bottomA) && (topB >= topA) && (leftB >= leftA) && (rightB <= rightA))
			{
				DeleteCollisionBody(loop2);
				loop2--;
			}
		}
	}
		*/
}

//-----------------------------------------------------------------------

void collision_layer::SortCollisions()
{
	// TODO: проверить
	/*
	if(!bodies.empty())
	{
		GLdouble max_sq = bodies[0]->BodyBox.Heigth*bodies[0]->BodyBox.Heigth;
		for(unsigned int loop = 1; loop < bodies.size(); ++loop)
		{
			GLdouble cur_sq = bodies[loop]->BodyBox.Heigth*bodies[loop]->BodyBox.Heigth;
			if(max_sq < cur_sq)
			{
				collision_body *temp_body = 0;
				max_sq = cur_sq;
				temp_body =  bodies[0];
				bodies[0] = bodies[loop];
				bodies[loop] = temp_body;
			}
		}
	}
	*/
}

//-----------------------------------------------------------------------
