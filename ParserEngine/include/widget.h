/*
 * widget.h
 *
 *  Created on: 28.05.2014
 *      Author: dmitry
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include "engine.h"

enum eWidgetType
{
	eWidgetType_Root,
	eWidgetType_Window,
	eWidgetType_Button,
	eWidgetType_Label,
	eWidgetType_TextBox,
	eWidgetType_Slider,
	eWidgetType_Image,

	eWidgetType_LastEnum
};

enum eGuiMessage
{
	eGuiMessage_MouseMove,
	eGuiMessage_MouseDown,
	eGuiMessage_MouseUp,
	eGuiMessage_MouseDoubleClick,
	eGuiMessage_MouseEnter,
	eGuiMessage_MouseLeave,

	eGuiMessage_GotFocus,
	eGuiMessage_LostFocus,

	eGuiMessage_OnDraw,

	eGuiMessage_ButtonPressed,

	eGuiMessage_TextChange,

	eGuiMessage_CheckChange,

	eGuiMessage_KeyPress,

	eGuiMessage_SliderMove,

	eGuiMessage_SelectionChange,

	eGuiMessage_LastEnum,
};

/*

class iWidget;

typedef void(*tCallbackFunc)(void *, iWidget *);
typedef std::list<iWidget *> tWidgetList;
typedef tWidgetList::iterator tWidgetListIt;

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

typedef std::list<cWidgetCallback> tWidgetCallbackList;
typedef tWidgetCallbackList::iterator tWidgetCallbackListIt;

class iWidget
{
	std::vector<tWidgetCallbackList> mvCallbackLists;

public:
	iWidget(eWidgetType aType);
	virtual ~iWidget();

	void AddCallback(void *cbObject, tCallbackFunc cbFunc);

	// Virtual methods
	virtual void OnDraw() {}

	virtual bool OnMouseMove() 	{return false;}
	virtual bool OnMouseDown() 	{return false;}
	virtual bool OnMouseUp() 	{return false;}
	virtual bool OnMouseEnter() {return false;}
	virtual bool OnMouseLeave() {return false;}
	//

	bool mbVisible;
	eWidgetType mType;
};

class cGuiGfxElement
{

};

class cWidgetButton: public iWidget
{
	bool mbPressed;

	bool mbDestroyImage;


public:
	cWidgetButton();
	~cWidgetButton();

	void SetImage();
	void OnDraw();

	bool OnMouseMove();
	bool OnMouseDown();
	bool OnMouseUp();
	bool OnMouseEnter();
	bool OnMouseLeave();

};*/

class cWidget;
typedef std::list<cWidget *> tWidgetList;
typedef tWidgetList::iterator tWidgetListIt;

typedef void(*tCallbackFunc)(std::string, void *, cWidget *);

class cGui
{
public:
	cGui();
	~cGui();

	int Init();

	void Update();
	void Draw();
	void Delete();

	tWidgetList mWidgetsList;
	input *mpInput;
};

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

class cWidget
{
protected:
	PE_Rect WidgetBox;

	cWidget *mpParent;
	cGui *mpGui;
	//tWidgetList mChildList;

	bool mbHasParent;
	std::string msName;

	bool mbVisible;
	eWidgetType mType;

	bool mbMouseIsOver;

	std::map <eGuiMessage, cWidgetCallback *> mCallbackList;
	std::map <eGuiMessage, cWidgetCallback *>::iterator mCallbackListIt;

public:
	cWidget(eWidgetType aType, float W, float H, float X, float Y);
	virtual ~cWidget();

	void AddCallback(eGuiMessage aMessage, void *cbObject, tCallbackFunc cbFunc);

	cWidget *GetParent();

	bool IsVisible();
	bool HasParent();

	virtual void OnInit() {}
	virtual void OnDraw() {}
	virtual void OnUpdate() {}

};
class cButton: public cWidget
{
	bool mbPressed;
	image *mpTexture;

public:
	cButton(float W, float H, float X, float Y);
	~cButton();

	void SetImage(image *aTexture);

	void OnInit();
	void OnDraw();
	void OnUpdate();
};


#endif /* WIDGET_H_ */
