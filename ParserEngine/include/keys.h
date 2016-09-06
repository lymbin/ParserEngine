/*
 * keys.h
 *
 *  Created on: 07.05.2014
 *      Author: dmitry
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "SDL2/SDL.h"

// TODO: add international keys if you want.
	///maps all keys to sdl keys

	typedef enum {
		KEY_UNKNOWN		= SDL_SCANCODE_UNKNOWN,
		//KEY_FIRST		= SDLK_FIRST,
		KEY_BACKSPACE	= SDL_SCANCODE_BACKSPACE,
		KEY_TAB			= SDL_SCANCODE_TAB,
		KEY_CLEAR		= SDL_SCANCODE_CLEAR,
		KEY_RETURN		= SDL_SCANCODE_RETURN,
		KEY_PAUSE		= SDL_SCANCODE_PAUSE,
		KEY_ESCAPE		= SDL_SCANCODE_ESCAPE,
		KEY_SPACE		= SDL_SCANCODE_SPACE,
		KEY_EXCLAIM		= SDLK_EXCLAIM,
		KEY_QUOTEDBL	= SDLK_QUOTEDBL,
		KEY_HASH		= SDLK_HASH,
		KEY_DOLLAR		= SDLK_DOLLAR,
		KEY_AMPERSAND	= SDLK_AMPERSAND,
		KEY_QUOTE		= SDLK_QUOTE,
		KEY_LEFTPAREN	= SDLK_LEFTPAREN,
		KEY_RIGHTPAREN	= SDLK_RIGHTPAREN,
		KEY_ASTERISK	= SDLK_ASTERISK,
		KEY_PLUS		= SDLK_PLUS,
		KEY_COMMA		= SDL_SCANCODE_COMMA,
		KEY_MINUS		= SDL_SCANCODE_MINUS,
		KEY_PERIOD		= SDL_SCANCODE_PERIOD,
		KEY_SLASH		= SDL_SCANCODE_SLASH,
		KEY_0			= SDL_SCANCODE_0,
		KEY_1			= SDL_SCANCODE_1,
		KEY_2			= SDL_SCANCODE_2,
		KEY_3			= SDL_SCANCODE_3,
		KEY_4			= SDL_SCANCODE_4,
		KEY_5			= SDL_SCANCODE_5,
		KEY_6			= SDL_SCANCODE_6,
		KEY_7			= SDL_SCANCODE_7,
		KEY_8			= SDL_SCANCODE_8,
		KEY_9			= SDL_SCANCODE_9,
		KEY_COLON		= SDLK_COLON,
		KEY_SEMICOLON	= SDL_SCANCODE_SEMICOLON,
		KEY_LESS		= SDLK_LESS,
		KEY_EQUALS		= SDL_SCANCODE_EQUALS,
		KEY_GREATER		= SDLK_GREATER,
		KEY_QUESTION	= SDLK_QUESTION,
		KEY_AT			= SDLK_AT,


		/*
		Skip uppercase letters
		*/

		KEY_LEFTBRACKET	= SDL_SCANCODE_LEFTBRACKET,
		KEY_BACKSLASH	= SDL_SCANCODE_BACKSLASH,
		KEY_RIGHTBRACKET= SDL_SCANCODE_RIGHTBRACKET,
		KEY_CARET		= SDLK_CARET,
		KEY_UNDERSCORE	= SDLK_UNDERSCORE,
		KEY_BACKQUOTE	= SDLK_BACKQUOTE,
		KEY_a			= SDL_SCANCODE_A,
		KEY_b			= SDL_SCANCODE_B,
		KEY_c			= SDL_SCANCODE_C,
		KEY_d			= SDL_SCANCODE_D,
		KEY_e			= SDL_SCANCODE_E,
		KEY_f			= SDL_SCANCODE_F,
		KEY_g			= SDL_SCANCODE_G,
		KEY_h			= SDL_SCANCODE_H,
		KEY_i			= SDL_SCANCODE_I,
		KEY_j			= SDL_SCANCODE_J,
		KEY_k			= SDL_SCANCODE_K,
		KEY_l			= SDL_SCANCODE_L,
		KEY_m			= SDL_SCANCODE_M,
		KEY_n			= SDL_SCANCODE_N,
		KEY_o			= SDL_SCANCODE_O,
		KEY_p			= SDL_SCANCODE_P,
		KEY_q			= SDL_SCANCODE_Q,
		KEY_r			= SDL_SCANCODE_R,
		KEY_s			= SDL_SCANCODE_S,
		KEY_t			= SDL_SCANCODE_T,
		KEY_u			= SDL_SCANCODE_U,
		KEY_v			= SDL_SCANCODE_V,
		KEY_w			= SDL_SCANCODE_W,
		KEY_x			= SDL_SCANCODE_X,
		KEY_y			= SDL_SCANCODE_Y,
		KEY_z			= SDL_SCANCODE_Z,
		KEY_DELETE		= SDL_SCANCODE_DELETE,

		/* End of ASCII mapped keysyms */

		/* Numeric keypad */

		KEY_KP0			= SDL_SCANCODE_KP_0,
		KEY_KP1			= SDL_SCANCODE_KP_1,
		KEY_KP2			= SDL_SCANCODE_KP_2,
		KEY_KP3			= SDL_SCANCODE_KP_3,
		KEY_KP4			= SDL_SCANCODE_KP_4,
		KEY_KP5			= SDL_SCANCODE_KP_5,
		KEY_KP6			= SDL_SCANCODE_KP_6,
		KEY_KP7			= SDL_SCANCODE_KP_7,
		KEY_KP8			= SDL_SCANCODE_KP_8,
		KEY_KP9			= SDL_SCANCODE_KP_9,
		KEY_KP_PEROID	= SDL_SCANCODE_KP_PERIOD,
		KEY_KP_DIVIDE	= SDL_SCANCODE_KP_DIVIDE,
		KEY_KP_MULTIPLY	= SDL_SCANCODE_KP_MULTIPLY,
		KEY_KP_MINUS	= SDL_SCANCODE_KP_MINUS,
		KEY_KP_PLUS		= SDL_SCANCODE_KP_PLUS,
		KEY_KP_ENTER	= SDL_SCANCODE_KP_ENTER,
		KEY_KP_EQUALS	= SDL_SCANCODE_KP_EQUALS,

		/* Arrows + Home/End pad */

		KEY_UP			= SDL_SCANCODE_UP,
		KEY_DOWN		= SDL_SCANCODE_DOWN,
		KEY_RIGHT		= SDL_SCANCODE_RIGHT,
		KEY_LEFT		= SDL_SCANCODE_LEFT,
		KEY_INSERT		= SDL_SCANCODE_INSERT,
		KEY_HOME		= SDL_SCANCODE_HOME,
		KEY_END			= SDL_SCANCODE_END,
		KEY_PAGEUP		= SDL_SCANCODE_PAGEUP,
		KEY_PAGEDOWN	= SDL_SCANCODE_PAGEDOWN,

		/* Function keys */

		KEY_F1			= SDL_SCANCODE_F1,
		KEY_F2			= SDL_SCANCODE_F2,
		KEY_F3			= SDL_SCANCODE_F3,
		KEY_F4			= SDL_SCANCODE_F4,
		KEY_F5			= SDL_SCANCODE_F5,
		KEY_F6			= SDL_SCANCODE_F6,
		KEY_F7			= SDL_SCANCODE_F7,
		KEY_F8			= SDL_SCANCODE_F8,
		KEY_F9			= SDL_SCANCODE_F9,
		KEY_F10			= SDL_SCANCODE_F10,
		KEY_F11			= SDL_SCANCODE_F11,
		KEY_F12			= SDL_SCANCODE_F12,
		KEY_F13			= SDL_SCANCODE_F13,
		KEY_F14			= SDL_SCANCODE_F14,
		KEY_F15			= SDL_SCANCODE_F15,

		/* Key state modifier keys */

		KEY_NUMLOCK		= SDL_SCANCODE_NUMLOCKCLEAR,
		KEY_CAPSLOCK	= SDL_SCANCODE_CAPSLOCK,
		KEY_SCROLLOCK	= SDL_SCANCODE_SCROLLLOCK,
		KEY_RSHIFT		= SDL_SCANCODE_RSHIFT,
		KEY_LSHFIT		= SDL_SCANCODE_LSHIFT,
		KEY_RCTRL		= SDL_SCANCODE_RCTRL,
		KEY_LCTRL		= SDL_SCANCODE_LCTRL,
		KEY_RALT		= SDL_SCANCODE_RALT,
		KEY_LALT		= SDL_SCANCODE_LALT,
		//KEY_RMETA		= SDLK_RMETA,
		//KEY_LMETA		= SDLK_LMETA,
		KEY_LSUPER		= SDL_SCANCODE_LGUI,		/* Left "Windows" key */
		KEY_RSUPER		= SDL_SCANCODE_RGUI,		/* Right "Windows" key */

		/* Miscellaneous function keys */

		KEY_HELP		= SDL_SCANCODE_HELP,
		KEY_PRINT		= SDL_SCANCODE_PRINTSCREEN,
		KEY_SYSREQ		= SDL_SCANCODE_SYSREQ,
		//KEY_BREAK		= SDL_SCANCODE_BREAK,
		KEY_MENU		= SDL_SCANCODE_MENU,

		/* Add any other keys here */
	} eKey;

	typedef enum{
		BUTTON_UNKNOWN = 0,
		BUTTON_LEFT = SDL_BUTTON_LEFT,
		BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
		BUTTON_RIGHT = SDL_BUTTON_RIGHT,
		//BUTTON_WHEELUP = SDL_BUTTON_WHEELUP,
		//BUTTON_WHEELDOWN = SDL_BUTTON_WHEELDOWN,
		BUTTON_X1 = SDL_BUTTON_X1,
		BUTTON_X2 = SDL_BUTTON_X2,
		BUTTON_LASTENUM,
	}eButton;


#endif /* KEYS_H_ */
