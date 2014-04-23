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
	//Инициализация TTF
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
	//Устанавливаем цвет текста
	format.textcolor.r = R;
	format.textcolor.g = G;
	format.textcolor.b = B;
	format.textcolor.unused = A;
}
void font::SetBGColor(Uint8 R, Uint8 G, Uint8 B)
{
	//Устанавливаем цвет заднего фона для текста
	format.bgcolor.r = R;
	format.bgcolor.g = G;
	format.bgcolor.b = B;
}
void font::SetStyle(bool bold, bool italic, bool underline)
{
	//Устанавливаем стиль для текста
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
	//Изменяем размер шрифта
	if(fileName.length())
	{
		format.size = size;
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
		TTF_SizeUTF8(ttf_font, text.c_str(), &w, &h);
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
		TTF_SizeUTF8(ttf_font, text.c_str(), &w, &h);
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
int font::Open(string source, int fontSize)
{
	//Открываем фон из источника
	ttf_font = TTF_OpenFont(source.c_str(), fontSize);
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
	format.bold = format.italic = format.underline = false;
	format.textcolor.r = format.textcolor.g = format.textcolor.b = format.textcolor.unused =255;
	format.bgcolor.r = format.bgcolor.g = format.bgcolor.b = format.bgcolor.unused = 0;
	format.size = SYS_TEXT_SIZE;
}
font::font(string file, int fontSize)
{
	ttf_font = 0;
	fileName = file;
	format.bold = format.italic = format.underline = false;
	format.textcolor.r = format.textcolor.g = format.textcolor.b = format.textcolor.unused =255;
	format.bgcolor.r = format.bgcolor.g = format.bgcolor.b = format.bgcolor.unused = 0;
	format.size = fontSize;

	Open(file, format.size);
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
	x = y = 0;
}
text::text(string textStrings)
{
	tex = 0;
	textFont = 0;
	textString = textStrings;
	x = y = 0;
}
text::text(string textStrings, font *textFonts)
{
	tex = 0;
	textFont = textFonts;
	textString = textStrings;
	x = y = 0;
}
text::text(string textStrings, string fontFile, int fontSize)
{
	tex = 0;
	textFont = new font(fontFile, fontSize);
	textString = textStrings;
	x = y = 0;
}
text::~text()
{
	if(tex)
		glDeleteTextures(1, &tex);

}
void text::ResizeText(int textSize)
{
	if(textFont)
	{
		textFont->Resize(textSize);
	}
	if(tex)
	{
		//TODO:Удалять старый текст(новый скорее всего не удаляет старый)
		glDeleteTextures(1, &tex);
		Write(x, y);
	}
}
void text::SetCoordinates(GLfloat new_x, GLfloat new_y)
{
	x = new_x;
	y = new_y;
}
void text::Draw(float x, float y, float dx, float dy, float delta, int center)
{
	//Отрисовываем текстуру от точки (x, y)
	//размером dX, dY.
	//с возможным углом поворота delta относительно верхнего левого угла
	//либо центра

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(x, y, 0);

	if(delta)
		glRotatef(delta, 0, 0, -1);
	if(center)
		glTranslatef(-dx/2, -dy/2, 0);

	//Рисуем текстуру
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(0,  0);  //Верхний левый угол
		glTexCoord2i(0, 1); glVertex2f(0,  dy); //Нижний левый угол
		glTexCoord2i(1, 1); glVertex2f(dx, dy); //Нижний правый угол
		glTexCoord2i(1, 0); glVertex2f(dx, 0);  //Верхний правый угол
	glEnd();

	glLoadIdentity();
}
void text::CreateTex()
{
	if(!textFont)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to CreateTex text with NULL font " << endl;
#endif
		return;
	}

	SDL_Surface *temp = 0, *tempb = 0;
	int w, h;
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if(!textFont->ttf_font)
	{
	#ifdef DEBUG_ERRORS
			cout << "Unable to CreateTex text with NULL font : "<< SDL_GetError() << endl;
	#endif
			SDL_FreeSurface(temp);
			SDL_FreeSurface(tempb);
			return;
	}
	temp = TTF_RenderText_Solid(textFont->ttf_font, textString.c_str(), textFont->format.textcolor);
	SDL_SetAlpha(temp, 0, 0);

	tempb = SDL_CreateRGBSurface(0, x, y, SYS_TEXT_DEPTH, rmask, gmask, bmask, amask);
	if(!tempb)
	{
#ifdef DEBUG_ERRORS
		cout << "Error create surface : "<< SDL_GetError() << endl;
#endif
		SDL_FreeSurface(temp);
		SDL_FreeSurface(tempb);
		return;
	}

	SDL_Rect src={0,0,0,0}, dest={0,0,0,0};
	TTF_SizeUTF8(textFont->ttf_font, textString.c_str(), &w, &h);

	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;

	dest.x = 0;
	dest.y = 0;
	dest.w = w;
	dest.h = h;

	SDL_BlitSurface(temp, &src, tempb, &dest);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
						GL_RGBA,
						tempb->w,
						tempb->h,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						tempb->pixels);

	SDL_FreeSurface(temp);
	SDL_FreeSurface(tempb);
}
void text::Write(GLfloat new_x, GLfloat new_y, int center)
{
	//TODO: добавить посимвольную отрисовку текста с определением интервала между буквами и
	//размером текста, независящим от размера шрифта

	if(!textString.length())
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Write NULL text. "<< endl;
#endif
		return;
	}

	//Забиваем новые координаты начальной точки для текста
	SetCoordinates(new_x, new_y);
	//Создаём текстуру с текстом определённого размера(он берётся из шрифта)
	CreateTex();

	int w, h;
	TTF_SizeUTF8(textFont->ttf_font, textString.c_str(), &w, &h);

	glBindTexture(GL_TEXTURE_2D, tex);
	Draw(x, y, w, h, 0, center);

}
void text::Write(GLfloat new_x, GLfloat new_y, std::string newText, int center)
{
	textString = newText;
	Write(new_x, new_y, center);
}
