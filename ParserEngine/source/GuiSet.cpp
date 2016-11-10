/*
 * GuiSet.cpp
 *
 *  Created on: 30 сент. 2016 г.
 *      Author: dmitry
 */

#include "GuiSet.h"
#include "widget.h"
#include "WidgetButton.h"

cGuiSet::cGuiSet(const std::string &asName, cGui *aGui)
{
	mpGui = aGui;
	mpSkin = 0;

	msName = asName;

	mpFocusedWidget = 0;
	mpAttentionWidget = 0;

	//mvVirtualSize =

	mfVirtualMinZ = -1000;
	mfVirtualMaxZ = 1000;

	mbActive = true;
	mbDrawMouse = true;
	mfMouseZ = 20;

	mpCurrentClipRegion = &mBaseClipRegion;
	mbDestroyingSet = false;
	mpGfxCurrentPointer = 0;
	mpWidgetRoot = new iWidget(eWidgetType_Root, this, mpSkin);
	mpWidgetRoot->AddCallback(eGuiMessage_OnDraw, this, kGuiCallback(DrawMouse));
}

cGuiSet::~cGuiSet()
{
	mbDestroyingSet = true;

	// delete all mlstWidgets
	delete mpWidgetRoot;

	mbDestroyingSet = false;
}

void cGuiSet::Update(float aTimeStep)
{
	// popups?
	// Popups
	//	if(mlstPopUps.empty()==false)
	//	{
	//		STLDeleteAll(mlstPopUps);
	//	}

	/////////////////////////////
	// Update widgets
	tWidgetListIt it = mlstWidgets.begin();
	for(; it != mlstWidgets.end(); ++it)
	{
		iWidget *pWidget = *it;
		pWidget->Update(aTimeStep);
	}
}

void cGuiSet::DrawAll(float afTimeStep)
{
	if(mbActive==false)
		return;

	///////////////////////////////
	//Draw all widgets
	SetCurrentClipRegion(&mBaseClipRegion);

	// TODO: Add clipRegion support???
	mpWidgetRoot->Draw(afTimeStep);//, &mBaseClipRegion);

	SetCurrentClipRegion(&mBaseClipRegion);
}

bool cGuiSet::SendMessage(eGuiMessage aMessage, cGuiMessageData &aData)
{
	switch(aMessage)
	{
		case eGuiMessage_MouseMove:			return OnMouseMove(aData);
		case eGuiMessage_MouseDown:			return OnMouseDown(aData);
		case eGuiMessage_MouseUp:			return OnMouseUp(aData);
		case eGuiMessage_MouseDoubleClick:	return OnMouseDoubleClick(aData);

		case eGuiMessage_KeyPress:			return OnKeyPress(aData);
	}

	return false;
}

////////////////////////////////////
// Rendering
void cGuiSet::Render()
{
	// TODO:
	// iLowLevelGraphics *pLowLevelGraphics = mpGraphics->GetLowLevel();
	//pLowLevelGraphics->SetDepthTestActive(false);
	//pLowLevelGraphics->SetIdentityMatrix(eMatrix_ModelView);

	//pLowLevelGraphics->SetOrthoProjection(mvVirtualSize,mfVirtualMinZ,mfVirtualMaxZ);

	///////////////////////////////
	// Render all clip regions

	RenderClipRegion();

	///////////////////////////////
	//Clear the render object set
	mBaseClipRegion.Clear();

}

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

cWidgetButton *cGuiSet::CreateWidgetButton ( const cVector2f &avLocalPos,
									const cVector2f &avSize,
									const std::string &asText,
									iWidget *apParent,
									const std::string &asName)
{
	cWidgetButton *aButton = new cWidgetButton(this, mpSkin);
	aButton->SetPosition(avLocalPos);
	aButton->SetSize(avSize);
	aButton->SetText(asText);
	aButton->SetName(asName);
	AddWidget(aButton, apParent);
	return aButton;
}

iWidget *cGuiSet::GetWidgetFromName(const std::string& asName)
{
	//TODO: use return (iWidget*)STLFindByName(mlstWidgets, asName);
	return 0;
}

void cGuiSet::DestroyWidget(iWidget *apWidget)
{
	// TODO: use
	//if(apWidget == mpFocusedWidget)
	//	mpFocusedWidget = NULL;
	//STLFindAndDelete(mlstWidgets,apWidget);
}

////////////////////////////////////
// Properties
void cGuiSet::SetActive(bool abX)
{
	if (mbActive == abX)
		return;
	mbActive = abX;
}

void cGuiSet::SetDrawMouse(bool abX)
{
	if(mbDrawMouse == abX)
		return;
	mbDrawMouse = abX;
}

void cGuiSet::SetRootWidgetClips(bool abX)
{

	//mpWidgetRoot->SetClipActive(abX);
	if(abX)
		mpWidgetRoot->SetSize(mvVirtualSize);
	else
		mpWidgetRoot->SetSize(0);
}

bool cGuiSet::GetRootWidgetClips()
{
	//return mpWidgetRoot->GetClipActive();
	return false;
}

void cGuiSet::SetVirtualSize(const cVector2f& avSize, float afMinZ, float afMaxZ)
{
	mvVirtualSize = avSize;
	mfVirtualMinZ = afMinZ;
	mfVirtualMaxZ = afMaxZ;
}

void cGuiSet::SetFocusedWidget(iWidget* apWidget)
{
	if(mpFocusedWidget)
	{
		cGuiMessageData data = cGuiMessageData(mvMousePos,0);
		mpFocusedWidget->ProcessMessage(eGuiMessage_LostFocus, data);
	}

	mpFocusedWidget = apWidget;
	if(mpFocusedWidget)
	{
		cGuiMessageData data = cGuiMessageData(mvMousePos,0);
		mpFocusedWidget->ProcessMessage(eGuiMessage_GotFocus, data);
	}
}

//void SetAttentionWidget(iWidget *apWidget);
//iWidget* GetAttentionWidget(){ return mpAttentionWidget;}

//int GetDrawPriority(){ return mlDrawPrio;}
//void SetDrawPriority(int alPrio){ mlDrawPrio = alPrio;}

//void SetCurrentPointer(cGuiGfxElement *apGfx);
//cGuiGfxElement* GetCurrentPointer(){ return mpGfxCurrentPointer;}

//void SetCullBackface(bool abX){ mbCullBackface = abX;}
//bool GetCullBackface(){ return mbCullBackface;}

bool cGuiSet::HasFocus()
{
	return mpGui->GetFocusedSet() == this;
}

void cGuiSet::SetSkin(cGuiSkin* apSkin)
{
	//if(mpSkin == apSkin) return; Remove til there is a real skin

	mpSkin = apSkin;

	if(mpSkin)
	{
		//mpGfxCurrentPointer = mpSkin->GetGfx(eGuiSkinGfx_PointerNormal);
	}
	else
	{
		mpGfxCurrentPointer = NULL;
	}
}

void cGuiSet::RenderClipRegion()
{
	/*
	iLowLevelGraphics *pLowLevelGraphics = mpGraphics->GetLowLevel();

	if(kLogRender)Log("-------------------\n");

	///////////////////////////////////////
	//See if there is anything to draw
	tGuiRenderObjectSet &setRenderObjects = m_setRenderObjects;
	if(setRenderObjects.empty())
	{
		if(kLogRender) Log("------------------------\n");
		return;
	}

	//////////////////////////////////
	// Graphics setup
	pLowLevelGraphics->SetTexture(0,NULL);

	//////////////////////////////////
	// Set up variables

	tGuiRenderObjectSetIt it = setRenderObjects.begin();

	iGuiMaterial *pLastMaterial = NULL;
	iTexture *pLastTexture = NULL;
	cGuiClipRegion *pLastClipRegion = NULL;

	cGuiGfxElement *pGfx = it->mpGfx;
	iGuiMaterial *pMaterial = it->mpCustomMaterial ? it->mpCustomMaterial : pGfx->mpMaterial;
	iTexture *pTexture = pGfx->mvTextures[0];
	cGuiClipRegion *pClipRegion = it->mpClipRegion;

	int lIdxAdd=0;

	///////////////////////////////////
	// Iterate objects
	while(it != setRenderObjects.end())
	{
		///////////////////////////////
		//Start rendering
		if(pLastMaterial != pMaterial){
			pMaterial->BeforeRender();
			if(kLogRender)Log("Material %s before\n",pMaterial->GetName().c_str());
		}

		////////////////////////////
		// SetClip area
		if(pLastClipRegion != pClipRegion)
		{
			SetClipArea(pLowLevelGraphics,pClipRegion);
		}

		pLowLevelGraphics->SetTexture(0,pTexture);
		if(kLogRender)Log("Texture %d\n",pTexture);

		//////////////////////////
		//Iterate for all with same texture and material
		do
		{
			cGuiRenderObject object = *it;
			cGuiGfxElement *pGfx = object.mpGfx;

			if(kLogRender)
			{
				if(pGfx->mvImages[0])
					Log(" gfx: %d '%s'\n",pGfx,pGfx->mvImages[0]->GetName().c_str());
				else
					Log(" gfx: %d 'null'\n");
			}

			///////////////////////////
			// Add object to batch
			for(int i=0; i<4; ++i)
			{
				cVertex &vtx = pGfx->mvVtx[i];
				cVector3f& vVtxPos = vtx.pos;
				cVector3f& vPos = object.mvPos;
				pLowLevelGraphics->AddVertexToBatch_Raw(
					cVector3f(	vVtxPos.x * object.mvSize.x + vPos.x,
					vVtxPos.y * object.mvSize.y + vPos.y,
					vPos.z),
					vtx.col * object.mColor,
					vtx.tex);
			}

			for(int i=0;i<4;i++)
				pLowLevelGraphics->AddIndexToBatch(lIdxAdd + i);

			lIdxAdd += 4;

			///////////////////////////
			//Set last texture
			pLastMaterial =  pMaterial;
			pLastTexture =   pTexture;
			pLastClipRegion = pClipRegion;

			/////////////////////////////
			//Get next object
			++it; if(it == setRenderObjects.end()) break;

			pGfx = it->mpGfx;
			pMaterial = it->mpCustomMaterial ? it->mpCustomMaterial : pGfx->mpMaterial;
			pTexture = it->mpGfx->mvTextures[0];
			pClipRegion = it->mpClipRegion;
		}
		while(	pTexture == pLastTexture &&
				pMaterial == pLastMaterial &&
				pClipRegion == pLastClipRegion);

		//////////////////////////////
		// Render batch
		pLowLevelGraphics->FlushQuadBatch(	eVtxBatchFlag_Position | eVtxBatchFlag_Texture0 |
											eVtxBatchFlag_Color0,false);
		pLowLevelGraphics->ClearBatch();
		lIdxAdd=0;

		/////////////////////////////////
		//Clip region end
		if(pLastClipRegion  != pClipRegion  || it == setRenderObjects.end())
		{
			if(pLastClipRegion->mRect.w >0)
			{
				pLowLevelGraphics->SetScissorActive(false);
				//for(int i=0; i<4; ++i) pLowLevelGraphics->SetClipPlaneActive(i, false);
			}
		}

		/////////////////////////////////
		//Material end
		if(pLastMaterial != pMaterial || it == setRenderObjects.end())
		{
			pLastMaterial->AfterRender();
			if(kLogRender)Log("Material %d '%s' after. new: %d '%s'\n",	pLastMaterial,pLastMaterial->GetName().c_str(),
																	pMaterial,pMaterial->GetName().c_str());
		}
	}

	///////////////////////////////
	//Clear render objects
	m_setRenderObjects.clear();

	if(kLogRender)Log("---------- END %d -----------\n");
	*/
}

void cGuiSet::AddWidget(iWidget *apWidget,iWidget *apParent)
{
	mlstWidgets.push_front(apWidget);

	if(apParent)
		apParent->AttachChild(apWidget);
	else
		mpWidgetRoot->AttachChild(apWidget);

	apWidget->Init();
}

bool cGuiSet::OnMouseMove(cGuiMessageData &aData)
{
	/*
	///////////////////////////
	//Set up variables
	mvMousePos = aData.mvPos;

	aData.mlVal = 0;
	if(mvMouseDown[0]) aData.mlVal |= eGuiMouseButton_Left;
	if(mvMouseDown[1]) aData.mlVal |= eGuiMouseButton_Middle;
	if(mvMouseDown[2]) aData.mlVal |= eGuiMouseButton_Right;

	///////////////////////////
	//Call widgets
	bool bRet = false;
	bool bPointerSet = false;
	tWidgetListIt it = mlstWidgets.begin();
	for(; it != mlstWidgets.end(); ++it)
	{
		iWidget *pWidget = *it;

		if(pWidget->PointIsInside(mvMousePos,false))
		{
			////////////////////////////
			//Mouse enter event
			if(pWidget->GetMouseIsOver()==false)
			{
				pWidget->SetMouseIsOver(true);
				if(pWidget->ProcessMessage(eGuiMessage_MouseEnter, aData))
				{
					bRet = true;
				}
			}

			////////////////////////////
			//Set pointer
			if(	bPointerSet == false && pWidget->GetPointerGfx())
			{
				if(	mpAttentionWidget &&
					pWidget->IsConnectedTo(mpAttentionWidget)==false)
				{
				}
				else
				{
					if(mpGfxCurrentPointer != pWidget->GetPointerGfx())
					{
						if(pWidget->IsEnabled())
						{
							SetCurrentPointer(pWidget->GetPointerGfx());
						}
					}
					bPointerSet = true;
				}
			}
		}
		else
		{
			////////////////////////////
			//Mouse leave event
			if(pWidget->GetMouseIsOver())
			{
				pWidget->SetMouseIsOver(false);
				pWidget->ProcessMessage(eGuiMessage_MouseLeave, aData);

				//In case the widget is moved under the mouse again, check:
				if(mpFocusedWidget == pWidget && pWidget->PointIsInside(mvMousePos, false))
				{
					pWidget->SetMouseIsOver(true);
					if(pWidget->ProcessMessage(eGuiMessage_MouseEnter, aData)) bRet = true;
				}
			}
		}

		////////////////////////////
		//Mouse move event
		if(pWidget->GetMouseIsOver() || mpFocusedWidget == pWidget)
		{
			if(pWidget->ProcessMessage(eGuiMessage_MouseMove, aData)) bRet = true;
		}
	}

	return bRet;
	*/
}

bool cGuiSet::OnMouseDown(cGuiMessageData &aData)
{
	///////////////////////////
	//Set up variables
	/*
	mvMouseDown[cMath::Log2ToInt(aData.mlVal)] = true;

	aData.mvPos = mvMousePos;

	iWidget *pOldFocus = mpFocusedWidget;

	///////////////////////////
	//Call widgets
	bool bRet = false;
	tWidgetListIt it = mlstWidgets.begin();
	for(; it != mlstWidgets.end(); ++it)
	{
		iWidget *pWidget = *it;

		//If these is an attention set, do send clicks to any other widgets
		if(mpAttentionWidget && pWidget->IsConnectedTo(mpAttentionWidget)==false)
		{
			continue;
		}

		if(pWidget->GetMouseIsOver())
		{
			if(mpFocusedWidget != pWidget)
			{
				if(pWidget->ProcessMessage(eGuiMessage_GotFocus, aData))
				{
					mpFocusedWidget = pWidget;
				}
			}
			else
			{
				mpFocusedWidget = pWidget;
			}

			//Log("Got focus %d\n",pWidget);

			if(pWidget->ProcessMessage(eGuiMessage_MouseDown, aData))
			{
				bRet = true;
				break;
			}
		}
	}

	//Se if anything was clicked
	if(bRet == false)
	{
		mpFocusedWidget = NULL;
	}

	//Lost focus callback
	if(mpFocusedWidget != pOldFocus)
	{
		//Log("Lost focus %d\n",pOldFocus);
		if(pOldFocus) pOldFocus->ProcessMessage(eGuiMessage_LostFocus, aData);

	}

	return bRet;
	*/
}

bool cGuiSet::OnMouseUp(cGuiMessageData &aData)
{
	///////////////////////////
	//Set up variables
	/*
	mvMouseDown[cMath::Log2ToInt(aData.mlVal)] = false;

	aData.mvPos = mvMousePos;

	///////////////////////////
	//Call widgets
	bool bRet = false;

	if(mpFocusedWidget)
	{
		bRet = mpFocusedWidget->ProcessMessage(eGuiMessage_MouseUp, aData);
	}

	if(bRet == false)
	{
		tWidgetListIt it = mlstWidgets.begin();
		for(; it != mlstWidgets.end(); ++it)
		{
			iWidget *pWidget = *it;

			//If these is an attention set, do send clicks to any other widgets
			if(mpAttentionWidget && pWidget->IsConnectedTo(mpAttentionWidget)==false)
			{
				continue;
			}

			if(pWidget != mpFocusedWidget && pWidget->GetMouseIsOver())
			{
				if(pWidget->ProcessMessage(eGuiMessage_MouseUp, aData))
				{
					bRet = true;
					break;
				}
			}
		}
	}

	return bRet;
	*/
}

bool cGuiSet::OnMouseDoubleClick(cGuiMessageData &aData)
{
	///////////////////////////
	//Set up variables
	/*
	aData.mvPos = mvMousePos;

	///////////////////////////
	//Call widgets
	bool bRet = false;
	tWidgetListIt it = mlstWidgets.begin();
	for(; it != mlstWidgets.end(); ++it)
	{
		iWidget *pWidget = *it;

		//If these is an attention set, do send clicks to any other widgets
		if(mpAttentionWidget && pWidget->IsConnectedTo(mpAttentionWidget)==false)
		{
			continue;
		}

		if(pWidget->GetMouseIsOver())
		{
			if(pWidget->ProcessMessage(eGuiMessage_MouseDoubleClick, aData))
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
	*/
}

bool cGuiSet::OnKeyPress(cGuiMessageData &aData)
{
	///////////////////////////
	//Set up variables
	/*
	aData.mvPos = mvMousePos;

	///////////////////////////
	//Call widgets
	bool bRet = false;

	if(mpFocusedWidget)
	{
		bRet = mpFocusedWidget->ProcessMessage(eGuiMessage_KeyPress, aData);
	}

	if(bRet==false)
	{
		tWidgetListIt it = mlstWidgets.begin();
		for(; it != mlstWidgets.end(); ++it)
		{
			iWidget *pWidget = *it;

			//If these is an attention set, do send clicks to any other widgets
			if(mpAttentionWidget && pWidget->IsConnectedTo(mpAttentionWidget)==false)
			{
				continue;
			}

			if(pWidget->GetMouseIsOver() && mpFocusedWidget != pWidget)
			{
				if(pWidget->ProcessMessage(eGuiMessage_KeyPress, aData))
				{
					bRet = true;
					break;
				}
			}
		}
	}

	return bRet;
	*/
}

bool cGuiSet::DrawMouse(iWidget* apWidget,cGuiMessageData& aData)
{
	/*
	if(HasFocus() && mbDrawMouse && mpGfxCurrentPointer)
	{
		DrawGfx(mpGfxCurrentPointer,cVector3f(mvMousePos.x,mvMousePos.y, mfMouseZ),
			mpGfxCurrentPointer->GetImageSize(),cColor(1,1));
	}
*/
	return true;
}
kGuiCalllbackDeclaredFuncEnd(cGuiSet,DrawMouse);
