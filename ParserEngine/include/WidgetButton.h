/*
 * WidgetButton.h
 *
 *  Created on: 	19.09.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef PARSERENGINE_INCLUDE_WIDGETBUTTON_H_
#define PARSERENGINE_INCLUDE_WIDGETBUTTON_H_

#include "widget.h"

class cGui;

class cWidgetButton: public iWidget
{
public:
	cWidgetButton(cGui *aGui);
	//cWidgetButton(cGuiSet) TODO: add another way to construct gui via one guiSet.
	~cWidgetButton();

	void SetImage(cTexture *aTexture);
	cTexture *GetImage();

protected:
	void OnLoadGraphics();

	void OnDraw();

	bool OnMouseMove(cGuiMessageData &aData);
	bool OnMouseDown(cGuiMessageData &aData);
	bool OnMouseUp(cGuiMessageData &aData);
	bool OnMouseEnter(cGuiMessageData &aData);
	bool OnMouseLeave(cGuiMessageData &aData);

	// Data
	bool mbPressed;

	cTexture *mImage; // TODO: to special cGuiGfxElement class

	/*cGuiGfxElement *mpGfxBackgroundUp;
	cGuiGfxElement *mvGfxBordersUp[4];
	cGuiGfxElement *mvGfxCornersUp[4];

	cGuiGfxElement *mpGfxBackgroundDown;
	cGuiGfxElement *mvGfxBordersDown[4];
	cGuiGfxElement *mvGfxCornersDown[4];*/
	//
};



#endif /* PARSERENGINE_INCLUDE_WIDGETBUTTON_H_ */
