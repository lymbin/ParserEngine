/*
 * Font.cpp
 *
 *  Created on: 06.06.2014
 *      Author: dmitry
 */

#include "Font.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cFontManager::cFontManager()
{
	Graphics = 0;
	if(!Fonts.empty())
		Fonts.clear();
#ifdef DEBUG_SYS
	cout << "Font manager initialization - success" << endl;
#endif
}

//-----------------------------------------------------------------------

cFontManager::~cFontManager()
{
	DeleteFonts();
	Fonts.clear();

	if(TTF_WasInit())
		TTF_Quit();

#ifdef DEBUG_SYS
	cout << "Font manager clean up - success" << endl;
#endif
}

//-----------------------------------------------------------------------

font::font(string file, int fontSize)
{
	ttf_font = 0;
	fileName = file;
	StaticFont = true;
	mFormat.bold = mFormat.italic = mFormat.underline = false;
	mFormat.textcolor.r = mFormat.textcolor.g = mFormat.textcolor.b = mFormat.textcolor.a =255;
	mFormat.bgcolor.r = mFormat.bgcolor.g = mFormat.bgcolor.b = mFormat.bgcolor.a = 0;
	mFormat.size = fontSize;

	mpFontManager = 0;
	mpTextManager = 0;

	if(fileName != "")
		Open(file, mFormat.size);

	msCurrentText = "";
}

//-----------------------------------------------------------------------

font::~font()
{
	if(mpFontManager)
	{
		mpFontManager->UnManageFont(this);
		mpFontManager = 0;
	}
	if(mpTextManager)
	{
		mpTextManager = 0;
	}
	if(ttf_font && TTF_WasInit())
		TTF_CloseFont(ttf_font);
	ttf_font = 0;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cFontManager::ManageFont(font *managed_font)
{
	if(!managed_font)
		return;

	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		if(managed_font->GetFont() == Fonts[loop]->GetFont())
			return;
	}

	Fonts.push_back(managed_font);
	managed_font->SetFontManager(this);
}

//-----------------------------------------------------------------------

void cFontManager::UnManageFont(font *managed_font)
{
	if(!managed_font)
		return;

	int place = -1;

	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		if(managed_font->GetFont() == Fonts[loop]->GetFont())
		{
			place = (int)loop;
			break;
		}
	}

	if(place < 0)
		return;

	if((unsigned int)(place+1) == Fonts.size())
	{
		// Шрифт в самом конце - удаляем, перед этим обснулив указатель на менеджер шрифтов
		//Fonts[place]-> = 0;
		Fonts.pop_back();
	}
	else
	{
		// Шрифт где-то внутри вектора - удаляем, перед этим обснулив указатель на менеджер шрифтов
		//TODO: проверить
		//Textures[place] = Textures[ Textures.size() - 1 ];
		//Fonts[place]->TextureManager = 0;
		Fonts.erase( Fonts.begin() + place);
	}
}

//-----------------------------------------------------------------------

// Инициализация TTF
int cFontManager::FontsInit()
{
	if(!TTF_WasInit())
	{
		if(TTF_Init() < 0)
		{
#ifdef DEBUG_ERRORS
			cout << "Error initializing TTF : " << SDL_GetError() << endl;
#endif
			return -1;
		}
	}
	else
	{
#ifdef DEBUG_SYS
	cout << "TTF already initialized! " << endl;
#endif
		return 0;
	}

#ifdef DEBUG_SYS
	cout << "TTF initialization - success" << endl;
#endif
	return 0;
}

//-----------------------------------------------------------------------

// Удаляем шрифты из памяти
void cFontManager::DeleteFonts()
{
	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		delete Fonts[loop];
	}
	Fonts.clear();
}

//-----------------------------------------------------------------------

// Устанавливаем указатель на графику
void cFontManager::SetGraphics(graphics *setGraphics)
{
	Graphics = setGraphics;
}

//-----------------------------------------------------------------------

void font::SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A )
{
	// Устанавливаем цвет текста
	mFormat.textcolor.r = R;
	mFormat.textcolor.g = G;
	mFormat.textcolor.b = B;
	mFormat.textcolor.a = A;
}

//-----------------------------------------------------------------------

void font::SetBGColor(Uint8 R, Uint8 G, Uint8 B)
{
	// Устанавливаем цвет заднего фона для текста
	mFormat.bgcolor.r = R;
	mFormat.bgcolor.g = G;
	mFormat.bgcolor.b = B;
}

//-----------------------------------------------------------------------

void font::SetStyle(bool bold, bool italic, bool underline)
{
	// Устанавливаем стиль для текста
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

//-----------------------------------------------------------------------

// Изменяем размер шрифта
void font::Resize(int size)
{
	if(fileName.length())
	{
		// заново пересоздаём шрифт из источника с заданным размером
		mFormat.size = size;
		Open(fileName, size);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to resize font." << endl;
#endif
	}
}

//-----------------------------------------------------------------------

void font::SetStatic(bool static_font)
{
	StaticFont = static_font;
}

//-----------------------------------------------------------------------

// Устанавливаем менеджер шрифтов для полуавтоматического управления памятью менеджером
void font::SetFontManager(cFontManager *apFontManager)
{
	mpFontManager = apFontManager;
	if(mpFontManager)
		mpFontManager->ManageFont(this);
}

//-----------------------------------------------------------------------

// Устанавливаем менеджер текстов для полуавтоматического управления памятью менеджером
void font::SetTextManager(cTextManager *apTextManager)
{
	mpTextManager = apTextManager;
}

//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------

// Открываем шрифт из источника
int font::Open(string source, int fontSize)
{
	if(ttf_font)
		TTF_CloseFont(ttf_font);

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

//-----------------------------------------------------------------------

// Пишем текст без использования text класса
void font::Write(std::string text, GLfloat x, GLfloat y, GLuint *tex, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if(!ttf_font)
	{
#ifdef DEBUG_ERRORS
		cout << "Font can not be founded! " << endl;
#endif
		return;
	}
	if(text == "")
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to Write NULL text. "<< endl;
#endif
		return;
	}

	msCurrentText = text;
	// Получаем размеры текста данного шрифта
	int w, h;
	TTF_SizeUTF8(ttf_font, msCurrentText.c_str(), &w, &h);

	CreateTex(tex, w, h);
	Bind(tex);
	Draw(x, y, w, h, red, green, blue, alpha);
}

//-----------------------------------------------------------------------

// Функция аналогичная верхней, но позволяет писать текст с выравниванием в заданном боксе
void font::Write(std::string text, PE_Rect aBox, int alAlignment, GLuint *tex, GLfloat W_Shift, GLfloat H_Shift,
				GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
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
		dx = aBox.X + (aBox.Width - CalcTextWidth(text))/2.0f;
	}
	else if(alAlignment & eTextAlignment_Right)
	{
		dx = aBox.X + (aBox.Width - CalcTextWidth(text));
	}

	if(alAlignment & eTextAlignment_Top)
	{
		dy = aBox.Y;
	}
	else if(alAlignment & eTextAlignment_Centered_V)
	{
		dy = aBox.Y + (aBox.Heigth - CalcTextHeigth(text))/2.0f;
	}
	else if(alAlignment & eTextAlignment_Bottom)
	{
		dy = aBox.Y + (aBox.Heigth - CalcTextHeigth(text));
	}
	dx+=W_Shift;
	dy+=H_Shift;

	Write(text, dx, dy, tex, red, green, blue, alpha);
}
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void font::CreateTex(GLuint *tex, int w, int h)
{
	if(*tex)
	{
		// Если текстура не пуста удаляем нафиг всё из неё
		if(mpTextManager)
		{
			mpTextManager->UnManageText(*tex);
		}
		glDeleteTextures(1, tex);
		*tex = 0;
	}

	// Создаём текстуру для tex

	SDL_Surface *temp = 0, *tempb = 0;

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

	temp = TTF_RenderUTF8_Blended(ttf_font, msCurrentText.c_str(), mFormat.textcolor);

	SDL_SetSurfaceBlendMode(temp, SDL_BLENDMODE_NONE);

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

	/*src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;
	 */

	dest.x = 0;
	dest.y = 0;
	//dest.w = w;
	//dest.h = h;

	SDL_BlitSurface(temp, NULL, tempb, &dest);
	glGenTextures(1, tex);

	glBindTexture(GL_TEXTURE_2D, *tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempb->w, tempb->h, 0, GL_BGRA,
				GL_UNSIGNED_BYTE, tempb->pixels);

	SDL_FreeSurface(temp);
	SDL_FreeSurface(tempb);
	if(mpTextManager)
	{
		mpTextManager->ManageText(*tex);
	}
}

//-----------------------------------------------------------------------

void font::Bind(GLuint *tex)
{
	if(*tex)
		glBindTexture(GL_TEXTURE_2D, *tex);
}

//-----------------------------------------------------------------------

void font::Draw(float x, float y, int w, int h, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(x, y, 0);

	//Рисуем текстуру
	glBegin(GL_QUADS);
		glColor4f( red, green, blue, alpha );
		glTexCoord2i(0, 0); glVertex2f(0,  0);  //Верхний левый угол
		glTexCoord2i(0, 1); glVertex2f(0,  h); //Нижний левый угол
		glTexCoord2i(1, 1); glVertex2f(w, h); //Нижний правый угол
		glTexCoord2i(1, 0); glVertex2f(w, 0);  //Верхний правый угол
	glEnd();

	glLoadIdentity();
}

//-----------------------------------------------------------------------
