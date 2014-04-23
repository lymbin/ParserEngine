/*
 * image.cpp
 *
 *  Created on: 21.04.2014
 *      Author: dmitry
 */

#include "engine.h"

using namespace std;


int image::Open(std::string source, GLint filter)
{
	SDL_Surface *temp_surface = 0;
	GLint maxTexSize;
	GLuint glFormat = GL_RGBA;

	if(!source.substr(source.length()-3, 3).compare("jpg"))
	{
		glFormat = GL_RGB;
	}

	temp_surface = IMG_Load(source.data());

	if(!temp_surface)
	{
#ifdef DEBUG_ERRORS
		cout << "Image manager error: " << texture.fileName << " : " << SDL_GetError() << endl;
#endif
		return -1;
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

	//TODO: рассмотреть внимательней следующий if
	if(temp_surface->w > maxTexSize)
	{
#ifdef DEBUG_ERRORS
		cout << "Image manager error: \"" << source << "\" texturesize too large." << endl;
#endif
		SDL_FreeSurface(temp_surface);
		return -1;
	}

	glGenTextures(1, &texture.tex);
	glBindTexture(GL_TEXTURE_2D, texture.tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, temp_surface->w, temp_surface->h, 0, glFormat, GL_UNSIGNED_BYTE, temp_surface->pixels);

	texture.pxw = temp_surface->w;
	texture.pxh = temp_surface->h;

	SDL_FreeSurface(temp_surface);
	return 0;
}
void image::Draw(float x, float y)
{

}
void image::Draw(float x, float y, float dx, float dy, float delta, int center)
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
void image::Draw(float width, float heigth,
						float top_x, float top_y, float top_dx, float top_dy,
						float x, float y, float dx, float dy,
						float delta, int center)
{
	//Вырезаем кусочек текстуры от большой размером width*heigth
	//от верхней левой точки (top_x, top_y) размером top_dx, top_dy
	//и отрисовываем изображение от точки (x, y)
	//с размером dx, dy
	//и возможным углом поворота delta относительно верхнего левого угла
	//либо центра
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(x,y,0);

	if(delta)
		glRotatef(delta, 0, 0, -1);

	if (center)
		glTranslatef(-dx/2,-dy/2,0);//смещаем по центре

	glBegin(GL_QUADS);
		glTexCoord2f((top_x/width),			(top_y/heigth));			glVertex2f(0, 0); //Верхний левый угол
		glTexCoord2f((top_x/width),			((top_y+top_dy)/heigth));	glVertex2f(0, dy);//Нижний левый угол
		glTexCoord2f(((top_x+top_dx)/width),((top_y+top_dy)/heigth)); 	glVertex2f(dx,dy);//Нижний правый угол
		glTexCoord2f(((top_x+top_dx)/width),(top_y/heigth));			glVertex2f(dx, 0);//Верхний правый угол
	glEnd();

	glLoadIdentity();
}
/*
void texture_manager::Resize(float width, float heigth)
{
	//Изменяем размер изображения

}*/
/*
void image::render()
{
	//Функция отрисовывает изображение
	//TODO: доделать
	Draw(0.0, 0.0, 300, 200);
}
*/
float image::Width()
{
	return texture.pxw;
}
float image::Heigth()
{
	return texture.pxh;
}
image::image()
{
	texture.fileName = "";
}
image::image(std::string file, GLint filter)
{
	texture.fileName = file;
	Open(file, filter);
}
image::~image()
{
	if(texture.tex)
		glDeleteTextures(1, &texture.tex);
}

