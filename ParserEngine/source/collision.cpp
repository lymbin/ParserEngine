/*
 * collision.cpp
 *
 *  Created on: 14.05.2014
 *      Author: dmitry
 */

#include "Physics.h"
#include "Collision.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cCollision::cCollision()
{
#ifdef DEBUGGING
	IsEnabled = true;
#endif
	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}

//-----------------------------------------------------------------------

cCollision::~cCollision()
{
	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}

//-----------------------------------------------------------------------

cCollisionBody::cCollisionBody()
{
	mBox.Heigth = 0;
	mBox.Width = 0;
	mBox.X = 0;
	mBox.Y = 0;
}

//-----------------------------------------------------------------------

cCollisionBody::~cCollisionBody()
{

}

//-----------------------------------------------------------------------

iCollisionLayer::iCollisionLayer(PE_Rect aBorder)
{
	Border.Heigth = aBorder.Heigth;
	Border.Width = aBorder.Width;
	Border.X = aBorder.X;
	Border.Y = aBorder.Y;
}

//-----------------------------------------------------------------------

iCollisionLayer::iCollisionLayer(GLfloat W, GLfloat H, GLfloat X, GLfloat Y)
{
	Border.Heigth = H;
	Border.Width = W;
	Border.X = X;
	Border.Y = Y;
}

//-----------------------------------------------------------------------

iCollisionLayer::~iCollisionLayer()
{
	mCollisionLayer.clear();
}

//-----------------------------------------------------------------------

iCollisionBody::iCollisionBody()
{
	mpCollision = 0;
	mpCallBackData = 0;
	handler = 0;
}

//-----------------------------------------------------------------------

iCollisionBody::~iCollisionBody()
{

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

// Основная функция проверки столкновений двух тел
bool cCollision::CheckCollision(cCollisionBody A, cCollisionBody B)
{
	return CheckCollision(A.mBox, B.mBox);
}

//-----------------------------------------------------------------------

// Функция проверки столкновений двух прямоугольников
bool cCollision::CheckCollision(PE_Rect A, PE_Rect B)
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

PE_Rect cCollision::GetCollisionPoints(cCollisionBody A, cCollisionBody B)
{
	return GetCollisionPoints(A.mBox, B.mBox);
}

//-----------------------------------------------------------------------

PE_Rect cCollision::GetCollisionPoints(PE_Rect A, PE_Rect B)
{
	PE_Rect CrossedRect;

	float leftA, leftB, leftC;
	float rightA, rightB, rightC;
	float topA, topB, topC;
	float bottomA, bottomB, bottomC;

	leftA = A.X;
	rightA = A.X + A.Width;
	topA = A.Y;
	bottomA = A.Y + A.Heigth;

	leftB = B.X;
	rightB = B.X + B.Width;
	topB = B.Y;
	bottomB = B.Y + B.Heigth;

	if(topA <= topB)
		topC = topB;
	else
		topC = topA;

	if(bottomA <= bottomB)
		bottomC = bottomA;
	else
		bottomC = bottomB;

	if(rightA <= rightB)
		rightC = rightA;
	else
		rightC = rightB;

	if(leftA <= leftB)
		leftC = leftB;
	else
		leftC = leftA;

	CrossedRect.X = leftC;
	CrossedRect.Y = topC;
	CrossedRect.Heigth = bottomC - topC;
	CrossedRect.Width = rightC - leftC;

/*
	if(topA <= topB || leftA <= leftB)
	{
		// Производим поиск по B
		leftC = B.X;
		rightC = B.X + B.Width;
		topC = B.Y;
		bottomC = B.Y + B.Heigth;

		while(cuted)
		{
			//Начинаем отсекать части у B
			NewRightC = B.X + B.Width/2.0;
			if(NewRightC > rightA)
			{

			}

		}
	}
	else if(topA > topB || leftA > leftB)
	{
		// Производим поиск по A
		leftC = A.X;
		rightC = A.X + A.Width;
		topC = A.Y;
		bottomC = A.Y + A.Heigth;

		while()
		{
			//Начинаем отсекать части у A

		}
	}
*/
	return CrossedRect;
}

//-----------------------------------------------------------------------

void iCollisionBody::SetBox(float W, float H, float X, float Y)
{
	mBox.Heigth = H;
	mBox.Width = W;
	mBox.X = X;
	mBox.Y = Y;
}

//-----------------------------------------------------------------------

void iCollisionBody::SetBox(PE_Rect aBox)
{
	mBox.Heigth = aBox.Heigth;
	mBox.Width = aBox.Width;
	mBox.X = aBox.X;
	mBox.Y = aBox.Y;
}

//-----------------------------------------------------------------------

void iCollisionBody::SetCollisionsPointer(cCollision *apCollision)
{
	mpCollision = apCollision;
}

//-----------------------------------------------------------------------

PE_Rect iCollisionBody::GetBox()
{
	return mBox;
}

//-----------------------------------------------------------------------

cCollision *iCollisionBody::GetCollisionPointer()
{
	return mpCollision;
}

//-----------------------------------------------------------------------

void iCollisionBody::CALLBACK(void (*callback)(iCollisionBody *thisBody, PE_Rect CollidedRect, void *CollidedObject, void *data), void *apCollidedObject, void *apCallBackData)
{
	handler = callback;
	mpCallBackData = apCallBackData;
	mpCollidedObject = apCollidedObject;
}

//-----------------------------------------------------------------------

bool iCollisionBody::HandleCollisions()
{
	bool Result = false;
	tPointList PointList;

	UpdateLayers();

	tpCollisionLayersIt LayersIt = mCollisionLayers.begin();
	for( ; LayersIt != mCollisionLayers.end(); ++LayersIt)
	{
		tpCollisionLayerIt BodiesIt = (*LayersIt)->mCollisionLayer.begin();
		for( ; BodiesIt != (*LayersIt)->mCollisionLayer.end(); ++BodiesIt)
		{
			if(this == (*BodiesIt))
				continue;
			if(cCollision::CheckCollision(*this, **BodiesIt))
			{
				Result = true;
				if(handler)
				{
					handler(this, cCollision::GetCollisionPoints(*this, **BodiesIt), (*BodiesIt)->mpCollidedObject, mpCallBackData); // Вызываем обработчик столкновения заданного класса
				}
			}
		}
	}
	return Result;
}

//-----------------------------------------------------------------------

// Устанавливем слой
void iCollisionBody::AddNewLayer(iCollisionLayer *apLayer)
{
	mCollisionLayers.push_back(apLayer);
}

//-----------------------------------------------------------------------

void iCollisionBody::UpdateLayers()
{
	if(mpCollision)
	{
		mCollisionLayers.clear();
		mpCollision->UpdateBodyLayers(this);
	}
	else
	{
		tpCollisionLayersIt It = mCollisionLayers.begin();
		for( ; It != mCollisionLayers.end(); ++It)
		{
			if((*It)->CheckBodyInLayer(this) == eBodyPosition_Outside)
			{
				(*It)->EraseBody(this);
				EraseLayer((*It));
			}
		}
	}
}

//-----------------------------------------------------------------------

void iCollisionBody::EraseLayer(iCollisionLayer *apLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mCollisionLayers.begin(), mCollisionLayers.end(), apLayer);

	if(It != mCollisionLayers.end())
	{
		mCollisionLayers.erase(It);
	}
}

//-----------------------------------------------------------------------

// Добавляем тело в слой
void iCollisionLayer::AddCollisionBody(iCollisionBody *body)
{
	tpCollisionLayerIt It = mCollisionLayer.begin();
	It = std::find(mCollisionLayer.begin(), mCollisionLayer.end(), body);

	if(It == mCollisionLayer.end())
	{
		mCollisionLayer.push_back(body);
	}
}

//-----------------------------------------------------------------------

// Проверяем где именно находится заданное тело, относительно слоя
int iCollisionLayer::CheckBodyInLayer(iCollisionBody *body)
{
	float leftBox, leftChecked;
	float rightBox, rightChecked;
	float	topBox, topChecked;
	float bottomBox, bottomChecked;

	// Параметры границы слоя
	leftBox = Border.X;
	rightBox = Border.X + Border.Width;
	topBox = Border.Y;
	bottomBox = Border.Y + Border.Heigth;

		// Простое сравнение AABB
		leftChecked = body->GetBox().X;
		rightChecked = body->GetBox().X + body->GetBox().Width;
		topChecked = body->GetBox().Y;
		bottomChecked = body->GetBox().Y + body->GetBox().Heigth;

		if((bottomBox >= bottomChecked) && (topBox <= topChecked) && (rightBox >= rightChecked) && (leftBox <= leftChecked))
		{
			// Тело внутри слоя
			return eBodyPosition_Inside;
		}
		else
		{
			// Ищем столкновения - можно сделать какую-нибудь общую функцию для поиска столкновений 2-х прямоугольников
			if(!cCollision::CheckCollision(Border, body->GetBox()))
			{
				return eBodyPosition_Outside;
			}
			else
			{
				// Тело соприкасается со слоем и входит одновременно в несколько слоёв и надо искать столкновения во всех
				return eBodyPosition_Intersect;
			}
		}

	return eBodyPosition_Outside;
}

//-----------------------------------------------------------------------

// Удаляем тело из слоя
void iCollisionLayer::EraseBody(iCollisionBody *body)
{
	tpCollisionLayerIt It;
	It = std::find(mCollisionLayer.begin(), mCollisionLayer.end(), body);

	if(It != mCollisionLayer.end())
	{
		mCollisionLayer.erase(It);
	}
}

//-----------------------------------------------------------------------

// Задаём границы слоя
void iCollisionLayer::SetLayerBorder(PE_Rect aBorder)
{
	Border.Heigth = aBorder.Heigth;
	Border.Width = aBorder.Width;
	Border.X = aBorder.X;
	Border.Y = aBorder.Y;
}

//-----------------------------------------------------------------------

// Получаем границы слоя
PE_Rect iCollisionLayer::GetLayerBorder()
{
	return Border;
}

//-----------------------------------------------------------------------

void cCollision::AddCollisionBody(iCollisionBody *aBody)
{
	tpCollisionLayerIt It;
	It = std::find(mAllCollisionBodies.begin(), mAllCollisionBodies.end(), aBody);

	if(It == mAllCollisionBodies.end())
	{
		mAllCollisionBodies.push_back(aBody);
	}
}

//-----------------------------------------------------------------------

void cCollision::AddCollisionLayer(iCollisionLayer *aLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mAllCollisionLayers.begin(), mAllCollisionLayers.end(), aLayer);

	if(It == mAllCollisionLayers.end())
	{
		mAllCollisionLayers.push_back(aLayer);
	}
}

//-----------------------------------------------------------------------

void cCollision::EraseCollisionBody(iCollisionBody *aBody)
{
	tpCollisionLayerIt It;
	It = std::find(mAllCollisionBodies.begin(), mAllCollisionBodies.end(), aBody);

	if(It != mAllCollisionBodies.end())
	{
		mAllCollisionBodies.erase(It);
	}
}

//-----------------------------------------------------------------------

void cCollision::EraseCollisionLayer(iCollisionLayer *aLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mAllCollisionLayers.begin(), mAllCollisionLayers.end(), aLayer);

	if(It != mAllCollisionLayers.end())
	{
		mAllCollisionLayers.erase(It);
	}
}

//-----------------------------------------------------------------------

void cCollision::DeleteAll()
{
	tpCollisionLayersIt LayerIt = mAllCollisionLayers.begin();
	tpCollisionLayerIt BodyIt = mAllCollisionBodies.begin();

	for( ; LayerIt != mAllCollisionLayers.end(); ++LayerIt)
	{
		delete (*LayerIt);
	}

	for( ; BodyIt != mAllCollisionBodies.end(); ++BodyIt)
	{
		delete (*BodyIt);
	}

	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}

//-----------------------------------------------------------------------

void cCollision::UpdateBodyLayers(iCollisionBody *aBody)
{
	AddCollisionBody(aBody);

	tpCollisionLayersIt It = mAllCollisionLayers.begin();
	for( ; It != mAllCollisionLayers.end(); ++It)
	{
		if((*It)->CheckBodyInLayer(aBody) != eBodyPosition_Outside)
		{
			(*It)->AddCollisionBody(aBody);
			aBody->AddNewLayer((*It));
		}
	}
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// DEPRECATED METHODS
//////////////////////////////////////////////////////////////////////////
/*
//-----------------------------------------------------------------------

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
				if(CheckBodyInLayer(bodies[loop]) == eBodyPosition_Inside)
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

void collision_AABB::SetAABBBox(PE_Rect NewBox)
{
	AABBBodyBox.Heigth = NewBox.Heigth;
	AABBBodyBox.Width = NewBox.Width;
	AABBBodyBox.X = NewBox.X;
	AABBBodyBox.Y = NewBox.Y;
}

//-----------------------------------------------------------------------


bool collision_OBB::OverlapsAABB(collision_AABB aabb)
{
	return true;
}

//-----------------------------------------------------------------------

bool collision_OBB::OverlapsOBB(collision_OBB obb)
{
	return true;
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

void collision_body::ClearLayers()
{
	if(!CurrentLayers.empty())
	{
		for(unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
		{
			CurrentLayers[loop]->EraseBody(this);
		}
		CurrentLayers.clear();
	}
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
		CurrentLayers[CurrentLayers.size()-1]->EraseBody(this);
		CurrentLayers.pop_back();
	}
	else
	{
		CurrentLayers[LayerID]->EraseBody(this);
		CurrentLayers.erase(CurrentLayers.begin() + LayerID);
	}
}

//-----------------------------------------------------------------------

void collision_body::UpdateLayers()
{
	// Если указатель на collision указан, то указываем новые слоя для тела, а старые удаляем
	//	Если не указан, то проверяем содержание в текужих слоях(удалить этот бред, потому что нам обязательно нужно проверять все слои, а не только те что храним в body)
	if(Collision)
	{
		// Очищаем массив слоёв и обнуляем тело внутри них
		ClearLayers();

		// Добавляем новые слои в тело
		Collision->NewLayersInBody(this);

		// Добавляем тело к слоям
		if(!CurrentLayers.empty())
		{
			for(unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
			{
				CurrentLayers[loop]->AddCollisionBody(this);
			}
		}
	}
	else
	{
		if(!CurrentLayers.empty())
		{
			for(unsigned int loop = 0; loop < CurrentLayers.size(); ++loop)
			{
				int result = CurrentLayers[loop]->CheckBodyInLayer(this);
				if(result == eBodyPosition_Outside)
				{
					RemoveLayer(loop);
					--loop;
				}
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
				if(CurrentLayers[loop]->CheckCollision(this))
					return true;
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
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

void collision_layer::AddCollisionBody(collision_body *body)
{
	bodies.push_back(body);
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
			return eBodyPosition_Inside;
		}
		else
		{
			// Ищем столкновения - можно сделать какую-нибудь общую функцию для поиска столкновений 2-х прямоугольников
			if(!collision::CheckCollision(LayerBorder, body->AABBBodyBox))
			{
				return eBodyPosition_Outside;
			}
		}

		// Тело соприкасается со слоем и входит одновременно в несколько слоёв и надо искать столкновения во всех
		return eBodyPosition_Intersect;
	}
	else
	{
		// TODO: доделать проверку
		return eBodyPosition_Outside;
	}

	return eBodyPosition_Outside;
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
//-----------------------------------------------------------------------
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

void collision::NewLayersInBody(collision_body *body)
{
	if(!layers.empty())
	{
		for(unsigned int loop = 0; loop < layers.size(); ++loop)
		{
			int result = layers[loop]->CheckBodyInLayer(body);
			if(result == eBodyPosition_Inside || result == eBodyPosition_Intersect)
			{
				body->AddNewLayer(layers[loop]);
			}
		}
	}
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

void collision_layer::SortCollisions()
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
*/
