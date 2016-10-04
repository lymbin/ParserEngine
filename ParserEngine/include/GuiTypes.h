/*
 * GuiTypes.h
 *
 *  Created on: 30 сент. 2016 г.
 *      Author: dmitry
 */

#ifndef PARSERENGINE_INCLUDE_GUITYPES_H_
#define PARSERENGINE_INCLUDE_GUITYPES_H_

enum eGuiMouseButton
{
	eGuiMouseButton_Left 		=	0x00000001,
	eGuiMouseButton_Middle 		=	0x00000002,
	eGuiMouseButton_Right		=	0x00000004,
	eGuiMouseButton_LastEnum 	=	4
};

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

//--------------------------------



#endif /* PARSERENGINE_INCLUDE_GUITYPES_H_ */
