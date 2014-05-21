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

collision_body::collision_body(int ColPass, int ColType, int BVType)
{
	Collision = 0;
	CurrentLayers.clear();

	BoundingVolumeType = BVType;
	CollisionPass = ColPass;
	CollisionType = ColType;

}

//-----------------------------------------------------------------------

collision_body::~collision_body()
{
	if(!CurrentLayers.empty())
	{
		for( unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
		{
			CurrentLayers[loop]->EraseBody(this);
		}
	}

	Collision = 0;
	CurrentLayers.clear();
}

//-----------------------------------------------------------------------

collision_layer::collision_layer(PE_Rect Box)
{
	Collision = 0;

	LayerBorder = Box;
	bodies.clear();
}

//-----------------------------------------------------------------------

collision_layer::collision_layer(GLfloat W, GLfloat H, GLfloat X, GLfloat Y)
{
	Collision = 0;

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
				if(CheckBodyInLayer(bodies[loop]) == COLLISION_INSIDE)
				{
					delete bodies[loop];
				}
			}
		}
		bodies.clear();
	}
	if(Collision)
	{
		Collision->EraseCollisionLayer(this);
		Collision = 0;
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
	return collision::CheckCollision(AABBBodyBox, aabb.AABBBodyBox);
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

void collision_body::SetBoundingVolumeType(int BVType)
{
	BoundingVolumeType = BVType;
}

//-----------------------------------------------------------------------

// Устанавливем слой
void collision_body::AddNewLayer(collision_layer *layer)
{
	CurrentLayers.push_back(layer);
}

//-----------------------------------------------------------------------

void collision_body::RemoveLayer(unsigned int LayerID)
{
	if(CurrentLayers.empty())
	{
		return;
	}

	if(LayerID < 0)
	{
		CurrentLayers.pop_back();
	}
	else
	{
		CurrentLayers.erase(CurrentLayers.begin() + LayerID);
	}
}

//-----------------------------------------------------------------------

void collision_body::UpdateLayers()
{
	if(!CurrentLayers.empty())
	{
		for(unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
		{
			int result = CurrentLayers[loop]->CheckBodyInLayer(this);
			if(result == COLLISION_OUTSIDE)
			{
				RemoveLayer(loop);
				--loop;
			}
		}
	}
}

//-----------------------------------------------------------------------

bool collision_body::CheckCollision()
{
	UpdateLayers();

	if(!CurrentLayers.empty())
	{
		for(unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
		{
			if(CurrentLayers[loop])
			{
				CurrentLayers[loop]->CheckCollision(this);
			}
		}
	}

	return false;
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

int collision_body::GetBoundingVolumeType()
{
	return BoundingVolumeType;
}

//-----------------------------------------------------------------------

collision_layer *collision_body::GetCollisionLayer(unsigned int LayerID)
{
	if(!CurrentLayers.empty())
	{
		if(LayerID <= CurrentLayers.size())
		{
			return CurrentLayers[LayerID];
		}
	}
	return 0;
}

//-----------------------------------------------------------------------

int collision_body::GetLayersSize()
{
	return CurrentLayers.size();
}

//-----------------------------------------------------------------------

void collision_layer::AddCollisionBody(collision_body *body)
{
	int BVPos = CheckBodyInLayer(body);

	if((BVPos == COLLISION_INSIDE) || (BVPos == COLLISION_INTERSECT))
	{
		bodies.push_back(body);
	}
}

//-----------------------------------------------------------------------

// Проверяем что тело находится в слое
int collision_layer::CheckBodyInLayer(collision_body *body)
{
	float leftBox, leftChecked;
	float rightBox, rightChecked;
	float	topBox, topChecked;
	float bottomBox, bottomChecked;

	// Параметры границы слоя
	leftBox = LayerBorder.X;
	rightBox = LayerBorder.X + LayerBorder.Width;
	topBox = LayerBorder.Y;
	bottomBox = LayerBorder.Y + LayerBorder.Heigth;

	if(body->BoundingVolumeType != COLLISION_OBB)
	{
		// Простое сравнение AABB
		leftChecked = body->AABBBodyBox.X;
		rightChecked = body->AABBBodyBox.X + body->AABBBodyBox.Width;
		topChecked = body->AABBBodyBox.Y;
		bottomChecked = body->AABBBodyBox.Y + body->AABBBodyBox.Heigth;

		if((bottomBox >= bottomChecked) && (topBox <= topChecked) && (rightBox >= rightChecked) && (leftBox <= leftChecked))
		{
			// Тело внутри слоя
			return COLLISION_INSIDE;
		}
		else
		{
			// Ищем столкновения - можно сделать какую-нибудь общую функцию для поиска столкновений 2-х прямоугольников
			if(!collision::CheckCollision(LayerBorder, body->AABBBodyBox))
			{
				return COLLISION_OUTSIDE;
			}
		}

		// Тело соприкасается со слоем и входит одновременно в несколько слоёв и надо искать столкновения во всех
		return COLLISION_INTERSECT;
	}
	else
	{
		// TODO: доделать проверку
		return COLLISION_OUTSIDE;
	}

	return COLLISION_OUTSIDE;
}

//-----------------------------------------------------------------------

bool collision_layer::CheckCollision(collision_body *body)
{
	if(!bodies.empty())
	{
		for(unsigned int loop = 0; loop < bodies.size(); ++loop)
		{
			if(bodies[loop]!=body)
			{
				switch (body->GetBoundingVolumeType()) {
					case COLLISION_AABB:
						collision_AABB *AABBbody;
						AABBbody = body;
						bodies[loop]->OverlapsAABB(*AABBbody);
						break;
					case COLLISION_AABB_OBB:
						//collision_AABB *AABBbody = body;
						bodies[loop]->OverlapsAABB(*AABBbody);
						// TODO: Доделать OBB
						break;
					case COLLISION_OBB:
						// TODO: Доделать OBB
						break;
					default:
						break;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------

void collision_layer::EraseBody(collision_body *body)
{
	if(!bodies.empty())
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

collision_body *collision::NewCollisionBody(unsigned int LayerId, int ColPass, int ColType, int BVType)
{
	collision_body *NewCollisionBody = new collision_body(ColPass, ColType, BVType);
	layers[LayerId]->bodies.push_back(NewCollisionBody);
	NewCollisionBody->Collision = this;
	NewCollisionBody->AddNewLayer(layers[LayerId]);
	return NewCollisionBody;
}

//-----------------------------------------------------------------------

collision_layer *collision::NewCollisionLayer(GLfloat W, GLfloat H, GLfloat X, GLfloat Y)
{
	collision_layer *NewLayer = new collision_layer(W, H, X, Y);
	layers.push_back(NewLayer);
	NewLayer->Collision = this;
	return NewLayer;
}

//-----------------------------------------------------------------------

void collision::AddBodyToLayer(collision_body *body, unsigned int LayerId)
{
	if(layers.size() >= LayerId)
	{
		if(layers[LayerId])
		{
			layers[LayerId]->AddCollisionBody(body);
		}
	}
}

//-----------------------------------------------------------------------

void collision::EraseBodyFromLayer(collision_body *body, unsigned int LayerId)
{
	if(layers.size() >= LayerId)
	{
		if(layers[LayerId])
		{
			layers[LayerId]->EraseBody(body);
		}
	}
}

//-----------------------------------------------------------------------

void collision::EraseCollisionLayer(unsigned int LayerId)
{
	if(layers.size() >= LayerId)
	{
		layers.erase(layers.begin() + LayerId);
	}
}

//-----------------------------------------------------------------------

void collision::EraseCollisionLayer(collision_layer *layer)
{
	if(!layers.empty())
	{
		for(unsigned int loop = 0; loop <= layers.size(); ++loop)
		{
			if(layers[loop] == layer)
			{
				layers.erase(layers.begin() + loop);
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------

bool collision::CheckCollision(PE_Rect A, PE_Rect B)
{
	float leftBox, leftChecked;
	float rightBox, rightChecked;
	float	topBox, topChecked;
	float bottomBox, bottomChecked;

	leftBox = A.X;
	rightBox = A.X + A.Width;
	topBox = A.Y;
	bottomBox = A.Y + A.Heigth;

	leftChecked = B.X;
	rightChecked = B.X + B.Width;
	topChecked = B.Y;
	bottomChecked = B.Y + B.Heigth;

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
