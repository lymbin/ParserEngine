/*
 * text.cpp
 *
 *  Created on: 22.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;

int font::FontInit()
{
	if(TTF_Init() < 0)
	{
#ifdef DEBUG_ERRORS
		cout << "Error initializing TTF : " << SDL_GetError() << endl;
#endif
		return -1;
	}
#ifdef DEBUG_SYS
	cout << "TTF initialization - success" << endl;
#endif
	return 0;
}

void font::SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A )
{
	textcolor.r = R;
	textcolor.g = G;
	textcolor.b = B;
	textcolor.unused = A;
}
void font::SetBGColor(Uint8 R, Uint8 G, Uint8 B)
{
	bgcolor.r = R;
	bgcolor.g = G;
	bgcolor.b = B;
}
void font::SetStyle(bool bold, bool italic, bool underline)
{
	int flags = 0;

	if(bold)
		flags |= TTF_STYLE_BOLD;
	if(italic)
		flags |= TTF_STYLE_ITALIC;
	if(underline)
		flags |= TTF_STYLE_UNDERLINE;
	if(!flags)
		flags = TTF_STYLE_NORMAL;

	if(ttf_font)
		TTF_SetFontStyle(ttf_font, flags);
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to set new font style with NULL font" << endl;
#endif
	}
}
void font::Resize(int size)
{
	if(fileName.length())
	{
		Open(fileName, size);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to resize font." << endl;
#endif
	}
}
int font::GetHeigth()
{
	if(ttf_font)
	{
		return TTF_FontHeight(ttf_font);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to GetHeigth NULL font. "<< endl;
#endif
		return 0;
	}
}
int font::CalcTextWidth(string text)
{
	if(ttf_font)
	{
		int w, h;
		TTF_SizeText(ttf_font, text.c_str(), &w, &h);
		return w;
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to CalcTextWidth NULL font. "<< endl;
#endif
		return 0;
	}
}
int font::CalcTextHeigth(string text)
{
	if(ttf_font)
	{
		int w, h;
		TTF_SizeText(ttf_font, text.c_str(), &w, &h);
		return h;
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to CalcTextHeigth NULL font. "<< endl;
#endif
		return 0;
	}
}
int font::Open(string source, int size)
{
	//Открываем фон из источника
	ttf_font = TTF_OpenFont(source.c_str(), size);
	if(!ttf_font)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to open font : " << source << SDL_GetError() << endl;
#endif
		return -1;
	}
	return 0;
}
void font::Write(std::string text, GLuint tex, GLfloat x, GLfloat y)
{

}
font::font()
{
	ttf_font = 0;
	fileName = "";
	textcolor.r = textcolor.g = textcolor.b = textcolor.unused =255;
	bgcolor.r = bgcolor.g = bgcolor.b = bgcolor.unused = 0;
}
font::font(string file, int size)
{
	ttf_font = 0;
	fileName = file;
	textcolor.r = textcolor.g = textcolor.b = textcolor.unused =255;
	bgcolor.r = bgcolor.g = bgcolor.b = bgcolor.unused = 0;

	Open(file, size);
}
font::~font()
{
	if(ttf_font)
		TTF_CloseFont(ttf_font);
}

text::text()
{
	tex = 0;
	textFont = 0;
	textString = "";
	size = SYS_TEXT_SIZE;
	x = y = 0;
}
text::text(string textStrings)
{
	tex = 0;
	textFont = 0;
	textString = textStrings;
	size = SYS_TEXT_SIZE;
	x = y = 0;
}
text::text(string textStrings, int textSize)
{
	tex = 0;
	textFont = 0;
	textString = textStrings;
	size = textSize;
	x = y = 0;
}
text::text(string textStrings, font *textFonts)
{
	tex = 0;
	textFont = textFonts;
	textString = textStrings;
	size = SYS_TEXT_SIZE;
	x = y = 0;
}
text::text(string textStrings, string fontFile, int fontSize)
{
	tex = 0;
	textFont = new font(fontFile, fontSize);
	textString = textStrings;
	size = fontSize;
	x = y = 0;
}

void text::ResizeText(int textSize)
{
	size = textSize;
	if(textFont)
	{
		textFont->Resize(textSize);
	}
	if(tex)
	{
		Write(x, y);
	}
}
void text::SetCoordinates(GLfloat new_x, GLfloat new_y)
{
	x = new_x;
	y = new_y;
}
void text::Write(GLfloat new_x, GLfloat new_y, bool center)
{
	/*
	if(!textString.length())
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Write NULL text. "<< endl;
#endif
		return;
	}

	int c;
	GLfloat sX, sY, posX = 0;

	if(center)
	{
		for(unsigned int loop=0; loop < textString.length(); loop++)
		{
			c = (unsigned int)textString[loop];
			sX = textFont->
		}
	}
	*/
}
void text::Write(GLfloat new_x, GLfloat new_y, std::string newText, bool center)
{

}
