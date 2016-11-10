/*
 * GuiSet.h
 *
 *  Created on: 30.09.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine.
 *	Used HPL Engine.
 */

#ifndef PARSERENGINE_INCLUDE_GUISET_H_
#define PARSERENGINE_INCLUDE_GUISET_H_

#include "Gui.h"

class cGuiClipRegion;
class cWidgetButton;
class iWidget;
struct cGuiMessageData;

typedef std::list<cGuiClipRegion*> tGuiClipRegionList;
typedef tGuiClipRegionList::iterator tGuiClipRegionListIt;

class cGuiClipRegion
{
public:
	cGuiClipRegion() : mRect(0,0,-1,-1){}
	~cGuiClipRegion(){}

	void Clear(){}
	cGuiClipRegion* CreateChild(const cVector2f &avPos, const cVector2f &avSize);

	//tGuiRenderObjectSet m_setObjects;
	cRect2f mRect;

	tGuiClipRegionList mlstChildren;
};

class cGuiSet
{
public:
	cGuiSet(const std::string &asName, cGui *aGui);
	~cGuiSet();

	void Update(float aTimeStep);

	void DrawAll(float afTimeStep);

	bool SendMessage(eGuiMessage aMessage, cGuiMessageData &aData);

	////////////////////////////////////
	// Rendering
	void Render();

	//void SetDrawOffset(const cVector3f& avOffset){ mvDrawOffset = avOffset;}
	void SetCurrentClipRegion(cGuiClipRegion *apRegion){ mpCurrentClipRegion = apRegion;}

	/*
	void DrawGfx(	cGuiGfxElement* apGfx,
					const cVector3f &avPos,
					const cVector2f &avSize=-1,
					const cColor& aColor=cColor(1,1),
					eGuiMaterial aMaterial = eGuiMaterial_LastEnum);
	void DrawFont(	const tWString &asText,
					iFontData *apFont, const cVector3f &avPos,
					const cVector2f &avSize, const cColor& aColor,
					eFontAlign aAlign = eFontAlign_Left,
					eGuiMaterial aMaterial = eGuiMaterial_FontNormal);
	*/
	cWidgetButton *CreateWidgetButton ( const cVector2f &avLocalPos=0,
										const cVector2f &avSize=0,
										const std::string &asText="",
										iWidget *apParent=NULL,
										const std::string &asName = "");

	iWidget *GetWidgetFromName(const std::string& asName);

	void DestroyWidget(iWidget *apWidget);

	////////////////////////////////////
	// Properties
	void SetActive(bool abX);
	bool IsActive(){return mbActive;}

	cGui* GetGui(){ return mpGui;}

	void SetDrawMouse(bool abX);
	bool GetDrawMouse(){ return mbDrawMouse;}

	void SetMouseZ(float afZ){mfMouseZ = afZ;}
	float GetMouse(){ return mfMouseZ;}

	void SetRootWidgetClips(bool abX);
	bool GetRootWidgetClips();

	void SetVirtualSize(const cVector2f& avSize, float afMinZ, float afMaxZ);
	const cVector2f& GetVirtualSize(){return mvVirtualSize;}

	void SetFocusedWidget(iWidget* apWidget);
	iWidget* GetFocusedWidget(){return mpFocusedWidget;}

	//void SetAttentionWidget(iWidget *apWidget);
	//iWidget* GetAttentionWidget(){ return mpAttentionWidget;}

	//int GetDrawPriority(){ return mlDrawPrio;}
	//void SetDrawPriority(int alPrio){ mlDrawPrio = alPrio;}

	//void SetCurrentPointer(cGuiGfxElement *apGfx);
	//cGuiGfxElement* GetCurrentPointer(){ return mpGfxCurrentPointer;}

	//void SetCullBackface(bool abX){ mbCullBackface = abX;}
	//bool GetCullBackface(){ return mbCullBackface;}

	bool HasFocus();

	void SetSkin(cGuiSkin* apSkin);
	//cGuiSkin* GetSkin(){ return mpSkin;}

	//cResources* GetResources(){ return mpResources;}

	bool IsDestroyingSet(){ return mbDestroyingSet;}

private:
	void RenderClipRegion();

	void AddWidget(iWidget *apWidget,iWidget *apParent);

	bool OnMouseMove(cGuiMessageData &aData);
	bool OnMouseDown(cGuiMessageData &aData);
	bool OnMouseUp(cGuiMessageData &aData);
	bool OnMouseDoubleClick(cGuiMessageData &aData);

	bool OnKeyPress(cGuiMessageData &aData);

	bool DrawMouse(iWidget* apWidget,cGuiMessageData& aData);
	kGuiCalllbackDeclarationEnd(DrawMouse);

	cGui *mpGui;
	cGuiSkin *mpSkin;

	std::string msName;

	//cResources *mpResources;
	//cGraphics* mpGraphics;
	//cSound *mpSound;
	//cScene *mpScene;

	iWidget *mpAttentionWidget;

	iWidget *mpFocusedWidget;

	iWidget* mpWidgetRoot;
	std::list<iWidget *> mlstWidgets;

	//tGuiRenderObjectSet m_setRenderObjects;


	cVector2f mvVirtualSize;
	float mfVirtualMinZ;
	float mfVirtualMaxZ;

	//cVector3f mvDrawOffset;

	bool mbActive;
	bool mbDrawMouse;
	float mfMouseZ;
	cGuiGfxElement *mpGfxCurrentPointer;

	bool mvMouseDown[3];
	cVector2f mvMousePos;

	//tGuiPopUpList mlstPopUps;

	cGuiClipRegion mBaseClipRegion;
	cGuiClipRegion* mpCurrentClipRegion;

	bool mbDestroyingSet;
};



#endif /* PARSERENGINE_INCLUDE_GUISET_H_ */
