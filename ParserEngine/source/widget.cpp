/*
 * widget.cpp
 *
 *  Created on: 28.05.2014
 *      Author: dmitry
 */

#include "widget.h"

iWidget::iWidget(eWidgetType aType, float W, float H, float X, float Y)
{
	mbMouseIsOver = true;
	mbVisible = true;
	mbEnabled = true;

	mpGui = 0;
	mpParent = 0;

	mType = aType;
	msName = "";

	WidgetBox.Width = W;
	WidgetBox.Heigth = H;
	WidgetBox.X = X;
	WidgetBox.Y = Y;

	mpTexture = 0;

	mCallbackLists.resize(eGuiMessage_LastEnum);
	mChildList.clear();
}
iWidget::~iWidget()
{
	tWidgetListIt it = mChildList.begin();
	while (it != mChildList.end())
	{
		RemoveChild(*it);
		it = mChildList.begin();
	}

	if (mpParent)
		mpParent->RemoveChild(this);
}

void iWidget::Update(float afTimeStep)
{
	OnUpdate(afTimeStep);
}

void iWidget::Draw(float afTimeStep)
{
	if (!mbVisible)
		return;

	OnDraw(afTimeStep);

	cGuiMessageData data;
	data.mfVal = afTimeStep;
	data.mpData = 0;
	ProcessMessage(eGuiMessage_OnDraw, data);

	// Draw children
	tWidgetListIt it = mChildList.begin();
	for (; it != mChildList.end(); ++it)
	{
		iWidget *pChild = *it;
		pChild->Draw(afTimeStep);
	}
}

bool iWidget::ProcessMessage(eGuiMessage aMessage, cGuiMessageData &aData)
{
	if(!IsEnabled())
		return false;

	aData.mMessage = aMessage;

	bool bRet = false;
	bRet = OnMessage(aMessage,aData); //This can override any message.

	/////////////////////////////////////////
	//Call the correct virtual function
	if (!bRet)
	{
		switch (aMessage)
		{
			case eGuiMessage_MouseMove:			bRet = OnMouseMove(aData); break;
			case eGuiMessage_MouseDown:			bRet = OnMouseDown(aData); break;
			case eGuiMessage_MouseUp:			bRet = OnMouseUp(aData); break;
			case eGuiMessage_MouseDoubleClick:	bRet = OnMouseDoubleClick(aData); break;
			case eGuiMessage_MouseEnter:		bRet = OnMouseEnter(aData); break;
			case eGuiMessage_MouseLeave:		bRet = OnMouseLeave(aData); break;
			case eGuiMessage_KeyPress:			bRet = OnKeyPress(aData); break;
			case eGuiMessage_GotFocus:			bRet = OnGotFocus(aData); break;
			case eGuiMessage_LostFocus:			bRet = OnLostFocus(aData); break;
		}
	}

	/////////////////////////////////////////
	//Process user callbacks for the event.
	if (ProcessCallbacks(aMessage, aData))
		bRet = true;

	return bRet;
}

void iWidget::Init()
{
	OnInit();
	LoadGraphics();
}

void iWidget::AddCallback(eGuiMessage aMessage, void *cbObject, tCallbackFunc cbFunc)
{
	mCallbackLists[aMessage].push_back(cWidgetCallback(cbObject, cbFunc));
}

bool iWidget::IsVisible()
{
	return mbVisible;
}

bool iWidget::PointIsInside(const cVector2f& avPoint, bool abOnlyClipped)
{
	if (mpParent )//&& mpParent->ClipsGraphics())
	{
		if (!mpParent->PointIsInside(avPoint, true))
		{
			return false;
		}
	}

	//if(abOnlyClipped && !mbClipsGraphics)
	//	return true;

	cVector2f vGlobalPos = GetGlobalPosition();

	if(	avPoint.x < vGlobalPos.x || avPoint.x > vGlobalPos.x + mvSize.x ||
		avPoint.y < vGlobalPos.y || avPoint.y > vGlobalPos.y + mvSize.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Работа с наследниками.
void iWidget::AttachChild(iWidget *apChild)
{
	if (apChild-->mpParent)
	{
		iWidget *parent = apChild->mpParent;
		parent->RemoveChild(apChild);
		apChild->SetPosition(apChild->mvPosition + parent->GetGlobalPosition());
		apChild->SetPosition(apChild->mvPosition - GetGlobalPosition());
	}

	apChild->mpParent = this;
	apChild->SetPositionUpdated();
	mChildList.push_back(apChild);
}

void iWidget::RemoveChild(iWidget *apChild)
{
	tWidgetListIt it = mChildList.begin();
	for(; it != mChildList.end(); ++it)
	{
		iWidget *pChild = *it;

		if(pChild == apChild)
		{
			mChildList.erase(it);

			pChild->mpParent = NULL;
			pChild->SetPositionUpdated();
			pChild->SetPosition(pChild->mvPosition + GetGlobalPosition());

			break;
		}
	}
}

void iWidget::SetEnabled(bool abX)
{
	if (mbEnabled == abX) return;

	mbEnabled = abX;
}

void iWidget::SetVisible(bool abX)
{
	if (mbVisible == abX) return;

	mbVisible = abX;
}

bool iWidget::IsEnabled()
{
	if (mpParent)
	{
		if (mpParent->IsEnabled())
			return mbEnabled;
		else
			return false;
	}

	return mbEnabled;
}

bool iWidget::HasFocus()
{
	//return mpSet->GetFocusedWidget() == this;
	return true;
}

void iWidget::SetName(const std::string &asName)
{
	msName = asName;
}

const std::string &iWidget::GetName()
{
	return msName;
}

void iWidget::SetText(const std::string &asText)
{
	if (asText == msText)
		return;

	msText = asText;

	OnChangeText();
	cGuiMessageData data = cGuiMessageData();
	ProcessMessage(eGuiMessage_TextChange,data);
}

const cColor &iWidget::GetDefaultFontColor()
{
	//return
}

void iWidget::SetDefaultFontColot(const cColor &aColor)
{

}

void iWidget::SetPosition(const cVector2f &avPos)
{
	mvPosition = avPos;

	SetPositionUpdated();
}

void iWidget::SetGlobalPosition(const cVector2f &avPos)
{
	SetPosition(avPos - mpParent->GetGlobalPosition());
}

void iWidget::SetSize(const cVector2f &avSize)
{
	mvSize = avSize;

	OnChangeSize();
}

cVector2f iWidget::WorldToLocalPosition(const cVector2f &avPos)
{
	return avPos - GetGlobalPosition();
}

cVector2f iWidget::GetPosRelativeToMouse(cGuiMessageData &aData)
{
	cVector2f vTemp = GetGlobalPosition() - aData.mvPos;
	return cVector2f(vTemp.x, vTemp.y);
}

bool iWidget::ProcessCallbacks(eGuiMessage aMessage, cGuiMessageData &aData)
{
	tWidgetCallbackList &lstCallbacks = mCallbackLists[aMessage];

	if (lstCallbacks.empty())
		return false;

	bool bRet = false;
	tWidgetCallbackListIt it = lstCallbacks.begin();
	for (; it != lstCallbacks.end(); ++it)
	{
		cWidgetCallback &callback = *it;

		bool bX = (callback.mpFunc)(callback.mpObject, this, aData);
		if (bX)
			bRet = true;
	}

	return bRet;
}

bool iWidget::OnGotFocus(cGuiMessageData &aData)
{
	return mbEnabled;
}

void iWidget::SetPositionUpdated()
{

}

void iWidget::LoadGraphics()
{

}
