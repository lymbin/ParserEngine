/*
 * Texture.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>

#include "Graphics.h"
#include "Color.h"
#include "GraphicTypes.h"

class cTexture;
class graphics;
class cColor;

typedef std::vector <cTexture *> tTextureVector;

class texture_manager
{
	// Менеджер текстур - управляет памятью.
	// Пока не используется для изображений из-за сложности передачи каждому новому изображению указатель на менеджер
	// TODO: доделать позднее выделив в глобал или в синглтон
	friend cTexture;
	friend graphics;
	graphics *Graphics;
protected:

	// Вектор хранящий все текстуры, которыми управляем
	tTextureVector Textures;
public:
	texture_manager();
	~texture_manager();

	// Получаем информацию по текстуре
	cTexture *GetTextureInfos(GLuint texture);

	//Перезагружаем текстуры
	void ReloadTextures();

	//
	//void RedrawTextures();

	//Удаляем текстуры
	void DeleteTextures();

	//Добавляем и удаляем из вектора управляющего текстурами
	void ManageTexture(cTexture *managed_image);
	void UnManageTexture(cTexture *managed_image);

	void SetGraphics(graphics *setGraphics);

};

class iLowLevelTexture
{
	//Содержит саму OpenGL текстуру изображения и всевозможные данные о ней

	std::string msPath; //Путь до файла

protected:
	float mfPxHeight; //ширина в пикселах
	float mfPxWidth; //высота в пикселах

public:
	iLowLevelTexture();
	~iLowLevelTexture();

	// Вывод реальных размеров изображения
	float GetWidth();
	float GetHeigth();

	std::string GetPath();

	void SetPath(std::string asPath);

};
class cTexture : public iLowLevelTexture
{
	friend texture_manager;
	texture_manager *TextureManager; // TODO:в глобал или в синглтон

	//TODO: протестировать
	//std::vector< std::vector< bool > > m_PixelOn; // Храним пиксели текстуры для модуля столкновений(коллизии)

	GLuint mTexture;

public:
	//cTexture(std::string file = "", GLint filter = SYS_GL_IMG_FILTER);
	cTexture(std::string file = "", GLint filter = GL_NEAREST);
	~cTexture();

	void SetTexManager(texture_manager *TextureManager);

	// Различные функции отрисовки
	void DrawTransform(float x, float y, PE_Rect *Box,
					GLfloat Scale = 1, GLfloat Rotation = 0);

	/*
	 *  Простая отрисовка полной текстуры в точке (x,y) и размерами исходной текстуры
	 *  с заданным увеличением и поворотом текстуры(по умолчанию отсутствует)
	 *  а также цветом текстуры(по умолчанию белый)
	 */
	void Draw(float x, float y,
			GLfloat Scale = 1, GLfloat Rotation = 0, cColor aCol = cColor(1.0f, 1.0f));

	/*
	 *  Отрисовка куска текстуры в точке (x, y)
	 *  с параметрами куска - (top_x, top_y) (dx, dy)
	 *  с заданным увеличением и поворотом текстуры(по умолчанию отсутствует)
	 *  а также цветом текстуры(по умолчанию белый)
	 */
	void Draw(float x, float y, PE_Rect *Box,
			GLfloat Scale = 1, GLfloat Rotation = 0, cColor aCol = cColor(1.0f, 1.0f));

	/*

	// Открываем изображение из файла
	int Open(std::string source, GLint filter = SYS_GL_IMG_FILTER);
	int OpenFromZip(std::string source, GLint filter = SYS_GL_IMG_FILTER);

	// Создаём текстуру из сурфейса(бывшая часть функции Open)
	void MakeTexture(SDL_Surface *Surface, GLint filter = SYS_GL_IMG_FILTER, bool LoadPixels = false);

	*/

	// Открываем изображение из файла
	int Open(std::string source, GLint filter = GL_NEAREST);
	int OpenFromZip(std::string source, GLint filter = GL_NEAREST);

	// Создаём текстуру из сурфейса(бывшая часть функции Open)
	void MakeTexture(SDL_Surface *Surface, GLint filter = GL_NEAREST, bool LoadPixels = false);

	// Полностью перерисовываем изображения с различными функциями отрисовки
	void Redraw(float x, float y,
			GLfloat Scale = 1, GLfloat Rotatation = 0, cColor aCol = cColor(1.0f, 1.0f));

	void Redraw(float x, float y, PE_Rect *Box,
			GLfloat Scale = 1, GLfloat Rotatation = 0, cColor aCol = cColor(1.0f, 1.0f));

	// Биндим текстуру для работы с OpenGL
	void Bind();

	// Перезагружаем текстуру в памяти из файла
	void Reload();

	// Удаляем текстуру
	void Delete();

};


#endif /* TEXTURE_H_ */
