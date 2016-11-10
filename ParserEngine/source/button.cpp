/*
 * button.cpp
 *
 *  Created on: 28.05.2014
 *      Author: dmitry
 */

#include "WidgetButton.h"

cWidgetButton::cWidgetButton(cGui *aGui):iWidget(eWidgetType_Button, 0, 0)
{
	mbPressed = false;
	mpTexture = 0;
	mImage = 0;
}

cWidgetButton::cWidgetButton(cGuiSet *aSet, cGuiSkin *aSkin):iWidget(eWidgetType_Button, aSet, aSkin)
{
	mbPressed = false;
	mpTexture = 0;
	mImage = 0;
}

cWidgetButton::~cWidgetButton()
{

}

void cWidgetButton::SetImage(cTexture *aTexture)
{
	mpTexture = aTexture;
}

void cWidgetButton::OnLoadGraphics()
{
	/*
	/////////////////////////
	//Up
	mpGfxBackgroundUp = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpBackground);

	mvGfxBordersUp[0] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpBorderRight);
	mvGfxBordersUp[1] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpBorderLeft);
	mvGfxBordersUp[2] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpBorderUp);
	mvGfxBordersUp[3] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpBorderDown);

	mvGfxCornersUp[0] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpCornerLU);
	mvGfxCornersUp[1] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpCornerRU);
	mvGfxCornersUp[2] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpCornerRD);
	mvGfxCornersUp[3] = mpSkin->GetGfx(eGuiSkinGfx_ButtonUpCornerLD);

	/////////////////////////
	//Down
	mpGfxBackgroundDown = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownBackground);

	mvGfxBordersDown[0] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownBorderRight);
	mvGfxBordersDown[1] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownBorderLeft);
	mvGfxBordersDown[2] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownBorderUp);
	mvGfxBordersDown[3] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownBorderDown);

	mvGfxCornersDown[0] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownCornerLU);
	mvGfxCornersDown[1] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownCornerRU);
	mvGfxCornersDown[2] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownCornerRD);
	mvGfxCornersDown[3] = mpSkin->GetGfx(eGuiSkinGfx_ButtonDownCornerLD);
	*/
}

void cWidgetButton::OnDraw(float afTimeStep)
{
	//cVector3f vOffset = 0;
	//if(mbPressed) vOffset = mpSkin->GetAttribute(eGuiSkinAttribute_ButtonPressedContentOffset);

	////////////////////////////////
	// Image
	if(mImage)
	{
		// TODO: DrawImage();
		/*
		mpSet->DrawGfx(	mpImage,GetGlobalPosition()+
						cVector3f(	mvSize.x/2 - mpImage->GetImageSize().x/2,
									mvSize.y/2 - mpImage->GetImageSize().y/2,0.2f) + vOffset
						);
						*/
	}

	////////////////////////////////
	// Text
	if(IsEnabled())
	{
		/*
		DrawDefaultText( msText,GetGlobalPosition()+
						cVector3f(mvSize.x/2, mvSize.y/2 - mvDefaultFontSize.y/2,0.5f)+vOffset,
						eFontAlign_Center);
						*/
	}
	else
	{
		/*
		DrawSkinText( msText,eGuiSkinFont_Disabled, GetGlobalPosition()+
					cVector3f(mvSize.x/2, mvSize.y/2 - mvDefaultFontSize.y/2,0.5f)+vOffset,
					eFontAlign_Center);
					*/
	}


	////////////////////////////////
	// Borders and background
	if(mbPressed)
	{
		/*DrawBordersAndCorners(	mpGfxBackgroundDown, mvGfxBordersDown, mvGfxCornersDown,
								GetGlobalPosition(), mvSize);
								*/
	}
	else
	{
		/*
		DrawBordersAndCorners(	mpGfxBackgroundUp, mvGfxBordersUp, mvGfxCornersUp,
								GetGlobalPosition(), mvSize);
								*/
	}
}

bool cWidgetButton::OnMouseMove(cGuiMessageData &aData)
{
	return true;
}

bool cWidgetButton::OnMouseDown(cGuiMessageData &aData)
{
	mbPressed = true;
	return true;
}

bool cWidgetButton::OnMouseUp(cGuiMessageData &aData)
{
	if(GetMouseIsOver()==false)
		return false;

	if(mbPressed)
		ProcessMessage(eGuiMessage_ButtonPressed,aData);

	mbPressed = false;


	return true;
}

bool cWidgetButton::OnMouseEnter(cGuiMessageData &aData)
{
	return false;
}

bool cWidgetButton::OnMouseLeave(cGuiMessageData &aData)
{
	mbPressed = false;

	return false;
}
