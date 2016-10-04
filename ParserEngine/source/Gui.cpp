/*
 * Gui.cpp
 *
 *  Created on: 30 сент. 2016 г.
 *      Author: dmitry
 */

#include "Gui.h"

cGui::cGui()
{
	mSetInFocus = NULL;
	mLastRenderTime = 0;
}

cGui::~cGui()
{

}

void cGui::Init()
{

}

void cGui::Update(float aTimeStep)
{
	tGuiSetMapIt it = mMapSets.begin();
	for (; it != mMapSets.end(); ++it)
	{
		//cGuiSet *pSet = it->second;
		//pSet->Update(aTimeStep);
	}

	// Update gfx
}

// Work with set
cGuiSet *cGui::CreateSet(const std::string aName)
{

}

cGuiSet *cGui::GetSetFromName(const std::string aName)
{

}

void cGui::SetFocus(cGuiSet *aSet)
{

}

void cGui::SetFocusByName(const std::string aName)
{

}
cGuiSet *cGui::GetFocusedSet()
{

}
void cGui::DestroySet(cGuiSet *aSet)
{

}

bool cGui::SendMousePos(const cVector2f &avPos, const cVector2f &avRel)
{

}
bool cGui::SendMouseClickDown(eGuiMouseButton aButton)
{

}
bool cGui::SendMouseClickUp(eGuiMouseButton aButton)
{

}
bool cGui::SendMouseDoubleClick(eGuiMouseButton aButton)
{

}

bool SendKeyPress(const cKeyPress& keyPress)
{

}
