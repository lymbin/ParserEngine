/*
 * text.cpp
 *
 *  Created on: 22.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;


//-----------------------------------------------------------------------

void font_manager::ManageFont(font *managed_font)
{
	if(!managed_font)
		return;

	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		if(managed_font->ttf_font == Fonts[loop]->ttf_font)
			return;
	}

	Fonts.push_back(managed_font);
}

//-----------------------------------------------------------------------

void font_manager::UnManageFont(font *managed_font)
{
	if(!managed_font)
		return;

	int place = -1;

	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		if(managed_font->ttf_font == Fonts[loop]->ttf_font)
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

font_manager::font_manager()
{
	Graphics = 0;
	if(!Fonts.empty())
		Fonts.clear();
}

//-----------------------------------------------------------------------

font_manager::~font_manager()
{
	DeleteFonts();
	Fonts.clear();
}

//-----------------------------------------------------------------------

// Инициализация TTF
int font_manager::FontsInit()
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
void font_manager::DeleteFonts()
{
	for(unsigned int loop = 0; loop < Fonts.size(); loop++)
	{
		delete Fonts[loop];
	}
}

//-----------------------------------------------------------------------

// Устанавливаем указатель на графику
void font_manager::SetGraphics(graphics *setGraphics)
{
	Graphics = setGraphics;
}

//-----------------------------------------------------------------------

void font::SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A )
{
	// Устанавливаем цвет текста
	format.textcolor.r = R;
	format.textcolor.g = G;
	format.textcolor.b = B;
	format.textcolor.unused = A;
}

//-----------------------------------------------------------------------

void font::SetBGColor(Uint8 R, Uint8 G, Uint8 B)
{
	// Устанавливаем цвет заднего фона для текста
	format.bgcolor.r = R;
	format.bgcolor.g = G;
	format.bgcolor.b = B;
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

//-----------------------------------------------------------------------

void font::SetStatic(bool static_font)
{
	StaticFont = static_font;
}

//-----------------------------------------------------------------------

// Устанавливаем менеджер шрифтов для полуавтоматического управления памятью менеджером
void font::SetTexManager(font_manager *FonManager)
{
	// Если менеджер уже задан - выходим, т.к. менеджер может быть всего один на всю программу
	if(FontManager)
		return;

	FontManager = FonManager;

	// Добавляем в менеджер этот шрифт(даже если он там есть)
	FontManager->ManageFont(this);
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
	ttf_font = TTF_OpenFont(source.c_str(), fontSize);
	if(!ttf_font)
	{
#ifdef DEBUG_ERRORS
		cout << "Unable to open font : " << source << SDL_GetError() << endl;
#endif
		return -1;
	}
	if(FontManager)
		FontManager->ManageFont(this);

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

	// Получаем размеры текста данного шрифта
	int w, h;
	TTF_SizeUTF8(ttf_font, text.c_str(), &w, &h);

	if(!*tex)
	{
		//TODO: Эта часть скопирована из Write класса text - подумать над изменением???
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

		temp = TTF_RenderUTF8_Blended(ttf_font, text.c_str(), format.textcolor);

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
	}

	// Эта часть скопирована из класса text
	glBindTexture(GL_TEXTURE_2D, *tex);

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

font::font(string file, int fontSize)
{
	ttf_font = 0;
	fileName = file;
	StaticFont = true;
	format.bold = format.italic = format.underline = false;
	format.textcolor.r = format.textcolor.g = format.textcolor.b = format.textcolor.unused =255;
	format.bgcolor.r = format.bgcolor.g = format.bgcolor.b = format.bgcolor.unused = 0;
	format.size = fontSize;

	FontManager = 0;

	if(fileName != "")
		Open(file, format.size);
}

//-----------------------------------------------------------------------

font::~font()
{
	if(FontManager)
	{
		FontManager->UnManageFont(this);
		FontManager = 0;
	}
	if(ttf_font)
		TTF_CloseFont(ttf_font);
	ttf_font = 0;
}

//-----------------------------------------------------------------------

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
	temp = TTF_RenderUTF8_Blended(textFont->ttf_font, textString.c_str(), textFont->format.textcolor);

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
	glGenTextures(1, &tex);
	Bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempb->w, tempb->h, 0, GL_BGRA,
				GL_UNSIGNED_BYTE, tempb->pixels);

	/*gluBuild2DMipmaps(GL_TEXTURE_2D,
						GL_RGBA,
						tempb->w,
						tempb->h,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						tempb->pixels);
						*/

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

void text::Write(GLfloat new_x, GLfloat new_y, int size, GLfloat Rotation, int center,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
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

	Bind();

	Draw(x, y, size, Rotation, center, red, green, blue, alpha);
}

//-----------------------------------------------------------------------

void text::SetText(std::string newText)
{
	textString = newText;
}

//-----------------------------------------------------------------------

text_manager::text_manager()
{
	Graphics = 0;
	if(!Texts.empty())
		Texts.clear();
}

//-----------------------------------------------------------------------

text_manager::~text_manager()
{
	DeleteText();
	if(!Texts.empty())
		Texts.clear();
	Graphics = 0;
}

//-----------------------------------------------------------------------

// Получаем информацию по тексту
text *text_manager::GetTextInfos(GLuint texture)
{
	// Получаем информацию о текстуре из её ID
	for(unsigned int loop = 0; loop < Texts.size(); loop++)
	{
		if(Texts[loop]->tex == texture)
		{
			return Texts[loop];
		}
	}
	return 0;
}

//-----------------------------------------------------------------------

//Удаляем текст
void text_manager::DeleteText()
{
	for(unsigned int loop = 0; loop < Texts.size(); loop++)
	{
		delete Texts[loop];
	}
}

//-----------------------------------------------------------------------

//Добавляем и удаляем из вектора управляющего текстом
void text_manager::ManageText(text *managed_text)
{
	for(unsigned int loop = 0; loop < Texts.size(); loop++)
	{
		if(Texts[loop]->tex == managed_text->tex)
		{
			return;
		}
	}
	/*if(!managed_image->TextureManager)
		managed_image->SetTexManager(this);
*/
	Texts.push_back(managed_text);
}

//-----------------------------------------------------------------------

void text_manager::UnManageText(text *managed_text)
{
	// Удаляем текст из вектора управления
	// Внимание: Это только удалит текст из вектора управления, но не удалит сам текст
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage

	int place = -1;

	// Ищем текст с данным ID в памяти
	for(unsigned int loop = 0; loop < Texts.size(); loop++)
	{
		if(Texts[loop]->tex == managed_text->tex)
		{
			place = loop;
			break;
		}
	}

	// Текстура не найдена - выходм
	if(place < 0)
		return;

	if((unsigned int)(place+1) == Texts.size())
	{
		// Текст в самом конце - удаляем, перед этим обснулив указатель на менеджер Texts
		//Texts[place]->TextureManager = 0;
		Texts.pop_back();
	}
	else
	{
		// Текст где-то внутри вектора - удаляем, перед этим обснулив указатель на менеджер Texts
		//TODO: проверить
		//Textures[place] = Textures[ Textures.size() - 1 ];
		//Texts[place]->TextureManager = 0;
		Texts.erase( Texts.begin() + place);
	}
}

//-----------------------------------------------------------------------

void text_manager::SetGraphics(graphics *setGraphics)
{
	Graphics = setGraphics;
}

//-----------------------------------------------------------------------
