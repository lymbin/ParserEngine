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

collision_body::collision_body()
{
	BodyType = COLLISION_BODY_UNKNOWN;
	CollisionType = COLLISION_UNPASSABLE;
}
collision_body::collision_body(PE_Rect Box, int ColType, int Type)
{
	BodyType = Type;
	CollisionType = ColType;
	BodyBox = Box;
}

//-----------------------------------------------------------------------

collision_body::~collision_body()
{

}

//-----------------------------------------------------------------------

collision::collision()
{
#ifdef DEBUGGING
	IsEnabled = true;
#endif
	bodies.clear();
}

//-----------------------------------------------------------------------

collision::~collision()
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
#ifdef DEBUG_SYS
	cout << "Collision clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

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

void collision_body::SetBodyBox(PE_Rect Box)
{
	BodyBox.Heigth = Box.Heigth;
	BodyBox.Width = Box.Width;
	BodyBox.X = Box.X;
	BodyBox.Y = Box.Y;
}

//-----------------------------------------------------------------------

int collision_body::GetBodyType()
{
	return BodyType;
}

//-----------------------------------------------------------------------

int collision_body::GetCollisionType()
{
	return CollisionType;
}

//-----------------------------------------------------------------------

PE_Rect collision_body::GetBodyBox()
{
	return BodyBox;
}

//-----------------------------------------------------------------------

int collision::init()
{
	return 0;
}

//-----------------------------------------------------------------------

void collision::NewCollisionBody(PE_Rect Box, int ColType, int BodyType)
{
	collision_body *newcollisionbody = new collision_body(Box, ColType, BodyType);
	bodies.push_back(newcollisionbody);
}

//-----------------------------------------------------------------------

bool collision::CheckCollision(PE_Rect Box)
{
#ifdef DEBUGGING
	if(!IsEnabled)
		return false;
#endif

	if(bodies.empty())
		return false;

	GLfloat leftBox, leftChecked;
	GLfloat rightBox, rightChecked;
	GLfloat	topBox, topChecked;
	GLfloat bottomBox, bottomChecked;

	leftBox = Box.X;
	rightBox = Box.X + Box.Width;
	topBox = Box.Y;
	bottomBox = Box.Y + Box.Heigth;

	int unPassableID = -1;
	for(unsigned int loop = 0; loop < bodies.size(); ++loop)
	{
		leftChecked = bodies[loop]->BodyBox.X;
		rightChecked = bodies[loop]->BodyBox.X + bodies[loop]->BodyBox.Width;
		topChecked = bodies[loop]->BodyBox.Y;
		bottomChecked = bodies[loop]->BodyBox.Y + bodies[loop]->BodyBox.Heigth;

		if((bottomBox <= topChecked) || (topBox >= bottomChecked) || (rightBox <= leftChecked) || (leftBox >= rightChecked))
		{
			continue;
		}
		else
		{
			if(bodies[loop]->CollisionType == COLLISION_UNPASSABLE)
			{
				if(unPassableID < 0)
					unPassableID = loop;
			}
			else //PASSABLE
			{
				if(unPassableID >= 0)
					unPassableID = -1;
				return false;
			}
		}
	}
	if(unPassableID < 0)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------

// Расширенный метод проверки
bool collision::CheckCollisionExtended(PE_Rect Box)
{
#ifdef DEBUGGING
	if(!IsEnabled)
		return false;
#endif

	// TODO: дописать код
	CheckCollision(Box);
	return true;
}

//-----------------------------------------------------------------------

void collision::DeleteAll()
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

void collision::DeleteCollisionBody(PE_Rect Box)
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop] && (bodies[loop]->BodyBox.Heigth == Box.Heigth) && (bodies[loop]->BodyBox.Width == Box.Width) && (bodies[loop]->BodyBox.X == Box.X) && (bodies[loop]->BodyBox.Y == Box.Y))
			{
				delete bodies[loop];
			}
		}
	}
}

//-----------------------------------------------------------------------

void collision::DeleteCollisionBody(int BodyId)
{
	if(!bodies.empty())
	{
		if(bodies[BodyId])
		{
			delete bodies[BodyId];
			bodies.erase(bodies.begin() + BodyId);
		}
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
void collision::OptimizeCollisions()
{
	// TODO: проверить
	if(bodies.empty())
		return;

	SortCollisions();

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
}

//-----------------------------------------------------------------------

void collision::SortCollisions()
{
	// TODO: проверить
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
}

//-----------------------------------------------------------------------
