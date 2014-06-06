/*
 * Font.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef FONT_H_
#define FONT_H_

#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#include <GL/glext.h>
#endif

#include "SystemConstants.h"
#include "GraphicTypes.h"

class font;
class graphics;

typedef std::list<font *> tFontsList;
typedef std::list<font *>::iterator tFontsListIt;

typedef GLuint tTextTexture;

class cFontManager
{
	// Менеджер шрифтов - управляет памятью всех шрифтов как менеджер текстур
	// Пока не используется, всё по той же причине, что и менеджер текстур - из-за сложности передачи каждому новому шрифту указатель на менеджер
	// TODO: доделать позднее выделив в глобал или в синглтон
	friend font;

	// Храним все шрифты
	std::vector < font *> Fonts;
	graphics *Graphics;
public:
	cFontManager();
	~cFontManager();

	// Инициализация TTF
	static int FontsInit();

	// Удаляем шрифты из памяти
	void DeleteFonts();

	// Устанавливаем указатель на графику
	void SetGraphics(graphics *setGraphics);

	// Добавляем и удаляем шрифты из менеджера
	void ManageFont(font *managed_font);
	void UnManageFont(font *managed_font);
};

class cTextManager
{
	// Храним все шрифты
	tTextureMap mTextTextures;
	graphics *Graphics;

public:
	cTextManager();
	~cTextManager();

	// Удаляем шрифты из памяти
	void DeleteAll();

	// Устанавливаем указатель на графику
	void SetGraphics(graphics *setGraphics);

	// Добавляем и удаляем тексты из менеджера
	void ManageText(tTextTexture aTextTexture, std::string aMapId = "");
	void UnManageText(tTextTexture aTextTexture, std::string aMapId = "");
};
struct sFontFormatting
{
	// Формат шрифта
	SDL_Color 	textcolor; 	// Цвет текста TODO: убрать отсюда в текст
	SDL_Color 	bgcolor;	// Цвет задней текстуры
	int 		size;		// Размер шрифта
	bool		bold;		// Жирный
	bool		italic;		// Курсив
	bool		underline;	// Подчёркивание
};

class font
{
private:
	TTF_Font 		*ttf_font; 	// SDL TTF шрифт
	std::string 	fileName;	// Файл со шрифтом
	sFontFormatting	mFormat;	// Формат шрифта, содержащий множество параметров
	bool 			StaticFont; // Шрифт не меняется при изменении камеры TODO: протестировать позднее
	std::string		msCurrentText;

	cFontManager *mpFontManager;
	cTextManager *mpTextManager;

	//Создаём текстуру с текстом определённого размера(он берётся из шрифта)
	void CreateTex(GLuint *tex, int w, int h);
	void Bind(GLuint *tex);
	void Draw(float x, float y, int w, int h, GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

public:

	font(std::string file = "", int size = SYS_TEXT_SIZE);
	~font();

	// Получаем все private параметры, в том числе формата шрифта
	TTF_Font	*GetFont() {return ttf_font;}
	int 		GetSize() { return mFormat.size;}
	bool		isBold() { return mFormat.bold;}
	bool		isItalic() { return mFormat.italic;}
	bool		isUnderline() { return mFormat.underline;}
	SDL_Color	GetColor() { return mFormat.textcolor;}
	SDL_Color	GetBGColor() { return mFormat.bgcolor;}
	sFontFormatting GetFormat() { return mFormat;}
	bool GetStaticFont() { return StaticFont;}
	std::string GetCurrentText() {return msCurrentText;}

	// Открываем шрифт с заданным размером
	int Open(std::string source, int size = SYS_TEXT_SIZE);

	// Устанавливаем цвет шрифта, заднего фона, стиль и формат
	void SetColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255);
	void SetBGColor(Uint8 R, Uint8 G, Uint8 B);
	void SetStyle(bool bold, bool italic, bool underline);
	void SetFormat(bool bold, bool italic, bool underline, int size,
			Uint8 R, Uint8 G, Uint8 B, Uint8 A,
			Uint8 bgR, Uint8 bgG, Uint8 bgB);

	// Изменяем размер шрифта
	void Resize(int size);

	// Изменяем статический шрифт для движения с камерой TODO: протестировать
	void SetStatic(bool static_font);

	// Устанавливаем менеджер шрифтов для полуавтоматического управления памятью менеджером
	void SetFontManager(cFontManager *apFontManager);

	// Устанавливаем менеджер текстов для полуавтоматического управления памятью менеджером
	void SetTextManager(cTextManager *apTextManager);

	// Пишем текст прямо из класса шрифт и вносим текст в менеджер текста
	// обязательно удалить tex после каждого кадра - иначе будет засорять память
	// TODO: если ввести обязательным наличие менеджера текстур у шрифта,
	//	то можно будет попробовать сделать указатель по умолчанию на ноль
	void Write(std::string text, GLfloat x, GLfloat y, GLuint *tex,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Функция аналогичная верхней, но позволяет писать текст с выравниванием в заданном боксе
	void Write(std::string text, PE_Rect aBox, int alAlignment, GLuint *tex, GLfloat W_Shift = 0, GLfloat H_Shift = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Получаем высоту шрифта
	int GetHeigth();

	// Получаем ширину и высоту текстуры с текстом данного шрифта
	int CalcTextWidth(std::string text);
	int CalcTextHeigth(std::string text);
};

// Не используемые более классы
/*
class text
{

	GLuint 		tex;		// OpenGL текстура
	std::string textString;	// Сам текст для написания(можно менять и делать всё новые и новые записи тем же шрифтом)
	font		*textFont;	// Указатель на шрифт

	GLfloat x;	// Точка отрисовки
	GLfloat y;	//

	// Создаём текстуру
	void CreateTex();
	void Bind();

	// Отрисовка текста от точки, в определённом размере, с заданным углом поворота,
	//		относительно центра или левого верхнего угла указанным цветом
	void Draw(float x, float y, int size = -1, GLfloat Rotation = 0, int center = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f) ;

public:
	text(std::string textStrings = "", std::string fontFile = "", int fontSize = SYS_TEXT_SIZE);
	text(std::string textStrings, font	*textFont);
	~text();

	// Получаем все private настройки
	GLuint 		GetTXT() {return tex;}
	font		*GetFont() {return textFont;}
	std::string GetText() { return textString;}

	void Write(GLfloat new_x, GLfloat new_y, std::string text = "", int size = -1, GLfloat Rotation = 0, int center = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	void Write(PE_Rect aBox, int alAlignment, GLfloat W_Shift = 0, GLfloat H_Shift = 0,
			std::string text = "", int size = -1, GLfloat Rotation = 0, int center = 0,
			GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat alpha = 1.0f);

	// Устанавливаем текст для отрисовки
	void SetText(std::string newText);

	// Устанавливаем шрифт
	void SetFont(font *newFont);

	// Новые координаты
	void SetCoordinates(GLfloat new_x = 0, GLfloat new_y = 0);

	// Меняем размер текста(шрифта)
	// TODO: переделать - сделать изменение размера текста не зависимое от размера шрифта
	void ResizeText(int textSize);

	int GetTextWidth();
	int GetTextHeigth();
};
*/


#endif /* FONT_H_ */
