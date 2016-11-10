/*
 * Gui.cpp
 *
 *  Created on: 30 сент. 2016 г.
 *      Author: dmitry
 */

#include "Gui.h"
#include "GuiSet.h"

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
	cGuiSet *aSet = new cGuiSet(aName, this);
	mMapSets.insert(tGuiSetMap::value_type(aName, aSet));
}

cGuiSet *cGui::GetSetFromName(const std::string aName)
{
	tGuiSetMapIt it = mMapSets.find(aName);
	if (it == mMapSets.end())
		return 0;

	return it->second;
}

void cGui::SetFocus(cGuiSet *aSet)
{
	if (mSetInFocus == aSet)
		return;

	mSetInFocus = aSet;
}

void cGui::SetFocusByName(const std::string aName)
{
	cGuiSet *aSet = GetSetFromName(aName);
	if (aSet)
		SetFocus(aSet);
}

cGuiSet *cGui::GetFocusedSet()
{
	return mSetInFocus;
}

void cGui::DestroySet(cGuiSet *aSet)
{
	if (!aSet)
		return;

	tGuiSetMapIt it = mMapSets.begin();
	for(; it != mMapSets.end();)
	{
		if(aSet == it->second)
		{
			delete aSet;
			mMapSets.erase(it++);
			break;
		}
		else
		{
			++it;
		}
	}
}

bool cGui::SendMousePos(const cVector2f &avPos, const cVector2f &avRel)
{
	if (!mSetInFocus)
		return false;

	cGuiMessageData data = cGuiMessageData(avPos, avRel);
	return mSetInFocus->SendMessage(eGuiMessage_MouseMove, data);
}

bool cGui::SendMouseClickDown(eGuiMouseButton aButton)
{
	if (!mSetInFocus)
		return false;

	cGuiMessageData data = cGuiMessageData(aButton);
	return mSetInFocus->SendMessage(eGuiMessage_MouseDown, data);
}

bool cGui::SendMouseClickUp(eGuiMouseButton aButton)
{
	if (!mSetInFocus)
		return false;

	cGuiMessageData data = cGuiMessageData(aButton);
	return mSetInFocus->SendMessage(eGuiMessage_MouseUp, data);
}

bool cGui::SendMouseDoubleClick(eGuiMouseButton aButton)
{
	if (!mSetInFocus)
		return false;

	cGuiMessageData data = cGuiMessageData(aButton);
	return mSetInFocus->SendMessage(eGuiMessage_MouseDoubleClick, data);
}

bool cGui::SendKeyPress(const cKeyPress& keyPress)
{
	if (!mSetInFocus)
		return false;

	cGuiMessageData data = cGuiMessageData(keyPress);
	return mSetInFocus->SendMessage(eGuiMessage_MouseMove, data);
}
