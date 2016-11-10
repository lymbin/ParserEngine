/*
 * widget.h - base class for gui parts.
 * Widget realization taken from HPL Engine.
 *
 *  Created on: 28.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 *
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include "engine.h"


class cGui;
class cGuiClipRegion;
class cGuiSet;

//--------------------------------
//////////////////////

class iWidget;
class cWidgetCallback;

typedef std::list<iWidget *> tWidgetList;
typedef tWidgetList::iterator tWidgetListIt;

typedef std::list<cWidgetCallback> tWidgetCallbackList;
typedef tWidgetCallbackList::iterator tWidgetCallbackListIt;

typedef bool(*tCallbackFunc)(void *, iWidget *, cGuiMessageData&);

// TODO: To gui.h and gui.cpp. Also it is a manager for all gui parts.
/*
class cGuiOld //: public iUpdatable
{
public:
	cGui();
	~cGui();

	int Init();

	void Update();
	void DrawAll();
	void Delete();

	// Create widgets for this set
	cWidgetButton *CreateWidgetButton();

	void DestroyWidget(iWidget *apWidget);

	////////////////////////////////////
	// Properties
	void SetActive(bool active);
	bool IsActive(){return mActive;}

	void SetDrawMouse(bool drawMouse);
	bool GetDrawMouse(){ return mDrawMouse;}

	bool HasFocus();

private:
	void AddWidget(iWidget *apWidget,iWidget *apParent);

	bool OnMouseMove(cGuiMessageData &aData);
	bool OnMouseDown(cGuiMessageData &aData);
	bool OnMouseUp(cGuiMessageData &aData);
	bool OnMouseDoubleClick(cGuiMessageData &aData);

	bool OnKeyPress(cGuiMessageData &aData);

	bool DrawMouse(iWidget* apWidget,cGuiMessageData& aData);

	// TODO: add skin
	std::string mName;
	cSound *mSound;

	iWidget *mFocusedWidget;

	iWidget *mWidgetRoot; // TODO: move this to guiSet.
	tWidgetList mWidgetsList;
	bool mActive;
	bool mDrawMouse;
};
*/
// set of graphics, sounds and other for all gui elements.
/*
class cGuiSet
{
public:
	cGuiSet(const std::string &asName, cGui *apGui);
	~cGuiSet();

	//////////

	void Update();

	void DrawAll();

	bool SendMessage(eGuiMessage, cGuiMessageData &aData);
};
*/

// Used for callback for a widgets events.
struct cWidgetCallback
{
	cWidgetCallback(void *cbObject, tCallbackFunc cbFunc)
	{
		mpObject = cbObject;
		mpFunc = cbFunc;
	}
	void *mpObject;
	tCallbackFunc mpFunc;
};

typedef std::vector<tWidgetCallbackList> tCallbackLists;
typedef std::vector<tWidgetCallbackList>::iterator tCallbackListIt;

// Базовый класс GUI виджетов. Каждый виджет имеет коробку WidgetBox
// TODO: добавить iUpdatable как базовый?
class iWidget
{
	friend class cGui;
protected:
	PE_Rect WidgetBox;
	cVector2f mvPosition;
	cVector2f mvGlobalPosition;
	cVector2f mvSize;

	cGui *mpGui;
	cGuiSet *mpSet;
	cGuiSkin *mpSkin;
	tWidgetList mChildList;

	std::string msName;

	bool mbEnabled;
	bool mbVisible;
	eWidgetType mType; // TODO: удалить типы виджетов и сделать их универсальными.

	bool mbMouseIsOver;

	std::string msText;

	cTexture *mpTexture;


	tCallbackLists mCallbackLists;
	tCallbackListIt mCallbackListIt;

	virtual void OnChangeSize() {}
	virtual void OnChangePosition() {}
	virtual void OnChangeText() {}

	virtual void OnInit() {}
	virtual void OnDraw(float afTimeStep) {}
	virtual void OnUpdate(float afTimeStep) {}

	virtual bool OnMessage(eGuiMessage aMessage, cGuiMessageData &aData){return false;}
	virtual bool OnMouseMove(cGuiMessageData &aData){return false;}
	virtual bool OnMouseDown(cGuiMessageData &aData){return false;}
	virtual bool OnMouseUp(cGuiMessageData &aData){return false;}
	virtual bool OnMouseDoubleClick(cGuiMessageData &aData){return false;}
	virtual bool OnMouseEnter(cGuiMessageData &aData){return false;}
	virtual bool OnMouseLeave(cGuiMessageData &aData){return false;}

	virtual bool OnGotFocus(cGuiMessageData &aData);
	virtual bool OnLostFocus(cGuiMessageData &aData){return false;}

	virtual bool OnKeyPress(cGuiMessageData &aData){return false;}

	// Private Helper functions
	cVector2f WorldToLocalPosition(const cVector2f &avPos);
	cVector2f GetPosRelativeToMouse(cGuiMessageData &aData);

	//The order must be like this:
	//Borders: Right, Left, Up and Down
	//Corners: LEftUp, RightUp, RightDown and LEftDown.
/*
	void DrawBordersAndCorners(cGuiGfxElement *apBackground,
								cGuiGfxElement **apBorderVec,cGuiGfxElement **apCornerVec,
								const cVector3f &avPosition, const cVector2f &avSize);

	void DrawSkinText(const tWString& asText, eGuiSkinFont aFont,const cVector2f& avPosition,
			eTextAlignment aAlign = eFontAlign_Left);

	void DrawDefaultText(	const tWString& asText,
							const cVector3f& avPosition,eFontAlign aAlign);
*/
	void SetPositionUpdated();

	void LoadGraphics();
	bool GetMouseIsOver(){ return mbMouseIsOver;}

public:
	iWidget(eWidgetType aType, cGuiSet *aSet, cGuiSkin *aSkin,
			float W = 0, float H = 0, float X = 0, float Y = 0);
	virtual ~iWidget();

	//General
	void Update(float afTimeStep);

	void Draw(float afTimeStep);

	bool ProcessMessage(eGuiMessage aMessage, cGuiMessageData &aData);

	void AddCallback(eGuiMessage aMessage, void *cbObject, tCallbackFunc cbFunc);

	void Init();

	// Public Helper functions
	bool PointIsInside(const cVector2f& avPoint, bool abOnlyClipped);

	// Работа с наследниками.
	void AttachChild(iWidget *apChild);
	void RemoveChild(iWidget *apChild);

	//Properties
	bool IsVisible();
	void SetVisible(bool abx);
	bool IsEnabled();
	void SetEnabled(bool abX);

	bool HasFocus();

	void SetName(const std::string &asName);
	const std::string &GetName();

	void SetText(const std::string &asText);
	const std::string &GetText();

	const cColor &GetDefaultFontColor();
	void SetDefaultFontColot(const cColor &aColor);

	// TODO: add set and get font size

	void SetPosition(const cVector2f &avPos);
	void SetGlobalPosition(const cVector2f &aPos);
	const cVector2f &GetLocalPosition() { return mvPosition; }
	const cVector2f &GetGlobalPosition() { return mvGlobalPosition; }

	void SetSize(const cVector2f &avSize);
	cVector2f GetSize() { return mvSize; }

	bool IsMouseOver() { return mbMouseIsOver; }

	iWidget *mpParent;

private:
	void SetMouseIsOver(bool abX){ mbMouseIsOver = abX; }
	bool ProcessCallbacks(eGuiMessage aMessage, cGuiMessageData &aData);
};

/*
class cLabel: public cWidget
{
public:
	cLabel(float W, float H, float X, float Y);
	~cLabel();

	void OnInit();
	void OnDraw();
	void OnUpdate();
};
*/

#endif /* WIDGET_H_ */
