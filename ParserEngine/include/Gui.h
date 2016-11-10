/*
 * Gui.h
 *
 *  Created on: 30 сент. 2016 г.
 *      Author: dmitry
 */

#ifndef PARSERENGINE_GUI_H_
#define PARSERENGINE_GUI_H_

#include <map>

#include "GuiTypes.h"
#include "Updater.h"
#include "MathTypes.h"
#include "InputTypes.h"

	class cResources;
	class cGraphics;
	class cSound;
	class cScene;

	class cGuiSet;
	class cGuiSkin;
	class cGuiGfxElement;

	class iGuiMaterial;

typedef std::map<std::string, cGuiSet*> tGuiSetMap;
typedef tGuiSetMap::iterator tGuiSetMapIt;

#define kGuiCalllbackDeclarationEnd(FuncName) \
	static bool FuncName##_static_gui(void *apObject,iWidget* apWidget,cGuiMessageData& aData);

#define kGuiCalllbackDeclaredFuncEnd(ThisClass,FuncName) \
	bool ThisClass::FuncName##_static_gui(void *apObject,iWidget* apWidget,cGuiMessageData& aData) \
	{\
		return ((ThisClass*)apObject)->FuncName(apWidget, aData); \
	}

#define kGuiCalllbackFuncEnd(ThisClass,FuncName) \
	static bool FuncName##_static_gui(void *apObject,iWidget* apWidget,cGuiMessageData& aData) \
	{ \
		return ((ThisClass*)apObject)->FuncName(apWidget, aData); \
}

#define kGuiCallback(FuncName) &FuncName##_static_gui

struct cGuiMessageData
{
	cGuiMessageData(){}
	cGuiMessageData(const cVector2f& avPos, const cVector2f& avRel)
	{
		mvPos = avPos;
        mvRel = avRel;
	}
	cGuiMessageData(const cVector2f& avPos,const cVector2f& avRel, int alVal)
	{
		mvPos = avPos;
		mvRel = avRel;
		mlVal = alVal;
	}
	cGuiMessageData(int alVal)
	{
		mlVal = alVal;
	}
	cGuiMessageData(float afVal)
	{
		mfVal = afVal;
	}
	cGuiMessageData(const cKeyPress& aKeyPress)
	{
		mKeyPress = aKeyPress;
	}

	cVector2f	mvPos;
	cVector2f	mvRel;
	int			mlVal;
	cKeyPress	mKeyPress;
	float		mfVal;
	void*		mpData;
	eGuiMessage mMessage;
};

class cGui
{
public:
	cGui();
	~cGui();

	void Init();
	void Destroy();

	void Update(float aTimeStep);

	// Work with set
	cGuiSet *CreateSet(const std::string aName);
	cGuiSet *GetSetFromName(const std::string aName);
	void SetFocus(cGuiSet *aSet);
	void SetFocusByName(const std::string aName);
	cGuiSet *GetFocusedSet();
	void DestroySet(cGuiSet *aSet);

	// TODO: add graphics
	/*
	cGuiGfxElement* CreateGfxFilledRect(const cColor& aColor, eGuiMaterial aMaterial, bool abAddToList=true);
	cGuiGfxElement* CreateGfxImage(	const tString &asFile,eGuiMaterial aMaterial,
									const cColor& aColor=cColor(1,1), bool abAddToList=true);
	cGuiGfxElement* CreateGfxTexture(const tString &asFile,eGuiMaterial aMaterial,
									const cColor& aColor=cColor(1,1), bool abMipMaps=false,
									bool abAddToList=true);
	//Loads several images asFile+00, etc. Used for animations.
	//Must have extension!
	cGuiGfxElement* CreateGfxImageBuffer(	const tString &asFile,eGuiMaterial aMaterial,
											bool abCreateAnimation=true,
											const cColor& aColor=cColor(1,1), bool abAddToList=true);

	void DestroyGfx(cGuiGfxElement* apGfx);
	*/

	// Input sending
	bool SendMousePos(const cVector2f &avPos, const cVector2f &avRel);
	bool SendMouseClickDown(eGuiMouseButton aButton);
	bool SendMouseClickUp(eGuiMouseButton aButton);
	bool SendMouseDoubleClick(eGuiMouseButton aButton);

	bool SendKeyPress(const cKeyPress& keyPress);

	//bool SendWheel(); // TODO: add wheel support
private:
	cGuiSet *mSetInFocus;

	tGuiSetMap mMapSets;
	unsigned long mLastRenderTime;
};



#endif /* PARSERENGINE_GUI_H_ */
