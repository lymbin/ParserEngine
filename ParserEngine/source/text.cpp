/*
 * text.cpp
 *
 *  Created on: 22.04.2014
 *      Author: dmitry
 */

#include "Font.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cTextManager::cTextManager()
{
	Graphics = 0;
	mTextTextures.clear();

	tTextureList aTextureList;
	aTextureList.clear();
	mTextTextures.insert(pair<std::string, tTextureList>("", aTextureList));
}

//-----------------------------------------------------------------------

cTextManager::~cTextManager()
{
	Graphics = 0;
	DeleteAll();
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

//Удаляем текст
void cTextManager::DeleteAll()
{
	tTextureMapIt MapIt = mTextTextures.begin();
	for( ; MapIt != mTextTextures.end(); ++MapIt)
	{
		tTextureListIt It = MapIt->second.begin();
		for( ; It != MapIt->second.begin(); ++It)
		{
			glDeleteTextures(1, &(*It));
		}
	}
	mTextTextures.clear();
}

//-----------------------------------------------------------------------

//Добавляем и удаляем из вектора управляющего текстом
void cTextManager::ManageText(tTextTexture aTextTexture, std::string aMapId)
{
	if(!aTextTexture)
		return; // Что-то пошло не так =(

	if(mTextTextures.empty())
	{
		// А менеджер-то пустой - заполним быстренько чем-нибудь
		tTextureList aTextureList;
		aTextureList.clear();
		mTextTextures.insert(pair<std::string, tTextureList>("", aTextureList));
	}

	tTextureMapIt MapIt = mTextTextures.find(aMapId);
	if(MapIt == mTextTextures.end())
	{
		// Не нашли ничерта!
		if(aMapId == "")
		{
			// А может в первый...
			MapIt = mTextTextures.begin();
		}
		else
			return;
	}

	tTextureListIt It = MapIt->second.begin();
	for( ; It != MapIt->second.end(); ++It)
	{
		if((*It) == aTextTexture)
		{
			return;
		}
	}

	MapIt->second.push_back(aTextTexture);

}

//-----------------------------------------------------------------------

void cTextManager::UnManageText(tTextTexture aTextTexture, std::string aMapId)
{
	// Удаляем текст из вектора управления
	// Внимание: Это только удалит текст из вектора управления, но не удалит сам текст
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage


	if(!aTextTexture || mTextTextures.empty())
		return;	// Что-то пошло не так =(

	if(aMapId == "")
	{
		// Удаляем со всех слоёв
		tTextureMapIt MapIt = mTextTextures.begin();
		for( ; MapIt != mTextTextures.end(); ++MapIt)
		{
			tTextureListIt It = MapIt->second.begin();
			for( ; It != MapIt->second.end(); ++It)
			{
				if((*It) == aTextTexture)
				{
					// Попался! Стираем его!
					MapIt->second.erase(It);
				}
			}
		}
	}
	else
	{
		tTextureMapIt MapIt = mTextTextures.find(aMapId);
		if(MapIt == mTextTextures.end())
		{
			// Не нашли эту текстуру в менеджере =(
			return;
		}
		tTextureListIt It = MapIt->second.begin();
		for( ; It != MapIt->second.begin(); ++It)
		{
			if((*It) == aTextTexture)
			{
				// Попался! Стираем его!
				MapIt->second.erase(It);
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------

void cTextManager::SetGraphics(graphics *setGraphics)
{
	Graphics = setGraphics;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// DEPRECATED METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
/*
text::text(string textStrings, font *ExistFont)
{
	tex = 0;
	textFont = ExistFont;
	textString = textStrings;
	x = y = 0;
}

//-----------------------------------------------------------------------

text::text(string textStrings, string fontFile, int fontSize)
{
	tex = 0;
	textString = textStrings;
	x = y = 0;

	if(fontFile != "")
		textFont = new font(fontFile, fontSize);
}

//-----------------------------------------------------------------------

text::~text()
{
	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
	}
	if(textFont)
	{
		delete textFont;
		textFont = 0;
	}
	textString.clear();
	x = y =0;
}

//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------

void text::SetCoordinates(GLfloat new_x, GLfloat new_y)
{
	x = new_x;
	y = new_y;
}

//-----------------------------------------------------------------------

void text::Draw(float x, float y, int size, GLfloat Rotation, int center,
			GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	//Отрисовываем текстуру от точки (x, y)
	//размером dX, dY.
	//с возможным углом поворота delta относительно верхнего левого угла
	//либо центра

	if(size>0)
	{
		// Меняем размер шрифта
		ResizeText(size);
	}

	int dx, dy;
	TTF_SizeUTF8(textFont->ttf_font, textString.c_str(), &dx, &dy);

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(x, y, 0);

	// Удаляем лишние градусы
	if(Rotation > 0)
	{
		while(Rotation >= 360)
			Rotation-=360;
	}
	else
	{
		while(Rotation <= -360)
			Rotation+=360;
	}

	if(Rotation)
	{
		// Переворачиваем текстуру
		if(center)
			glTranslatef(dx/2, dy/2, 0); 		// Смещаемся в точку (dx/2, dx/2)

		glRotatef(Rotation, 0.0f, 0.0f, 1.0f);	// Поворачиваем текстуру относительно предыдущей точки Translate на Rotate градусов по часовой стрелке

		if(center)
			glTranslatef(-dx/2, -dy/2, 0); 	// Возвращаемся в исходное состояние - в точку (x, y)

	}
	//Рисуем текстуру
	glBegin(GL_QUADS);
	 	glColor4f( red, green, blue, alpha );
		glTexCoord2i(0, 0); glVertex2f(0,  0);  //Верхний левый угол
		glTexCoord2i(0, 1); glVertex2f(0,  dy); //Нижний левый угол
		glTexCoord2i(1, 1); glVertex2f(dx, dy); //Нижний правый угол
		glTexCoord2i(1, 0); glVertex2f(dx, 0);  //Верхний правый угол
	glEnd();

	glLoadIdentity();
}

//-----------------------------------------------------------------------

// Создаём текстуру
void text::CreateTex()
{
	if(!textFont || !textFont->ttf_font)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to CreateTex text with NULL font " << endl;
#endif
		return;
	}

	if(tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
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

	TTF_SizeUTF8(textFont->ttf_font, textString.c_str(), &w, &h);
	//temp = TTF_RenderUTF8_Solid(textFont->ttf_font, textString.c_str(), textFont->format.textcolor);
	//TODO:test
	//temp = TTF_RenderUTF8_Shaded(textFont->ttf_font, textString.c_str(), textFont->format.textcolor, textFont->format.bgcolor);
	temp = TTF_RenderUTF8_Blended(textFont->ttf_font, textString.c_str(), textFont->mFormat.textcolor);

	SDL_SetAlpha(temp, 0, 0);

	tempb = SDL_CreateRGBSurface(0, w, h, SYS_TEXT_DEPTH, rmask, gmask, bmask, amask);
	if(!tempb)
	{
#ifdef DEBUG_ERRORS
		cout << "Error create surface : "<< SDL_GetError() << endl;
#endif
		SDL_FreeSurface(temp);
		SDL_FreeSurface(tempb);
		return;
	}

	SDL_Rect //src={0,0,0,0},
			dest={0,0,0,0};

	//src.x = 0;
	//src.y = 0;
	//src.w = w;
	//src.h = h;


	dest.x = 0;
	dest.y = 0;
	//dest.w = w;
	//dest.h = h;

	SDL_BlitSurface(temp, NULL, tempb, &dest);
	glGenTextures(1, &tex);
	Bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempb->w, tempb->h, 0, GL_BGRA,
				GL_UNSIGNED_BYTE, tempb->pixels);

	//gluBuild2DMipmaps(GL_TEXTURE_2D,
	//					GL_RGBA,
	//					tempb->w,
	//					tempb->h,
	//					GL_RGBA,
	//					GL_UNSIGNED_BYTE,
	//					tempb->pixels);


	SDL_FreeSurface(temp);
	SDL_FreeSurface(tempb);
}

//-----------------------------------------------------------------------

void text::Bind()
{
	if(tex)
		glBindTexture(GL_TEXTURE_2D, tex);
}

//-----------------------------------------------------------------------

void text::Write(GLfloat new_x, GLfloat new_y, std::string text, int size, GLfloat Rotation, int center,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	//TODO: добавить посимвольную отрисовку текста с определением интервала между буквами и
	//размером текста, независящим от размера шрифта

	if(text!="")
	{
		SetText(text);
	}

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

	Bind();

	Draw(x, y, size, Rotation, center, red, green, blue, alpha);
}

//-----------------------------------------------------------------------

void text::Write(PE_Rect aBox, int alAlignment, GLfloat W_Shift, GLfloat H_Shift,
		std::string text, int size, GLfloat Rotation, int center,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	//TODO: добавить посимвольную отрисовку текста с определением интервала между буквами и
	//размером текста, независящим от размера шрифта

	if(text!="")
	{
		SetText(text);
	}

	if(!textString.length())
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Write NULL text. "<< endl;
#endif
		return;
	}

	GLfloat dx, dy;

	if(!alAlignment)
	{
		alAlignment = eTextAlignment_Left | eTextAlignment_Top;
	}

	if(alAlignment & eTextAlignment_Left)
	{
		dx = aBox.X;
	}
	else if(alAlignment & eTextAlignment_Centered_H)
	{
		dx = aBox.X + (aBox.Width - GetFont()->CalcTextWidth(textString))/2.0f;
	}
	else if(alAlignment & eTextAlignment_Right)
	{
		dx = aBox.X + (aBox.Width - GetFont()->CalcTextWidth(textString));
	}

	if(alAlignment & eTextAlignment_Top)
	{
		dy = aBox.Y;
	}
	else if(alAlignment & eTextAlignment_Centered_V)
	{
		dy = aBox.Y + (aBox.Heigth - GetFont()->CalcTextHeigth(textString))/2.0f;
	}
	else if(alAlignment & eTextAlignment_Bottom)
	{
		dy = aBox.Y + (aBox.Heigth - GetFont()->CalcTextHeigth(textString));
	}

	dx+=W_Shift;
	dy+=H_Shift;

	//Забиваем новые координаты начальной точки для текста
	SetCoordinates(dx, dy);
	//Создаём текстуру с текстом определённого размера(он берётся из шрифта)
	CreateTex();

	Bind();

	Draw(x, y, size, Rotation, center, red, green, blue, alpha);
}

//-----------------------------------------------------------------------

void text::SetText(std::string newText)
{
	textString = newText;
}

//-----------------------------------------------------------------------

int text::GetTextWidth()
{
	return GetFont()->CalcTextWidth(textString);
}

//-----------------------------------------------------------------------

int text::GetTextHeigth()
{
	return GetFont()->CalcTextHeigth(textString);
}
*/
//-----------------------------------------------------------------------
