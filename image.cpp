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
	// Открываем текстуру из файла
	SDL_Surface *temp_surface = 0;
	GLint maxTexSize;

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

	MakeTexture(temp_surface);

	SDL_FreeSurface(temp_surface);
	return 0;
}
int image::OpenFromZip(string source, GLint filter)
{
	// Открываем текстуру из zip архива с файлом
	// TODO: пока никак не работает функция - доделать

	if(!Open(source, filter))
		return 0;
	else
		return -1;
}
void image::MakeTexture(SDL_Surface *Surface, GLint filter, bool LoadPixels)
{
	// Создаём текстуру из сурфейса(бывшая часть функции Open)
	glGenTextures(1, &texture.tex);
	glBindTexture(GL_TEXTURE_2D, texture.tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLuint glFormat = GL_RGBA;
	if(!Surface->format->Amask)
	{
		glFormat = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, Surface->w, Surface->h, 0, glFormat, GL_UNSIGNED_BYTE, Surface->pixels);

	texture.pxw = Surface->w;
	texture.pxh = Surface->h;
}
void image::Draw(float x, float y)
{
	// Отрисовываем текстуру от точки (x, y)
	// 	полного размера
	// TODO: проследить за правильностью отрисовки не квадратных тектур

	Bind();

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	//Рисуем текстуру
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(0,  			0);  			//Верхний левый угол
		glTexCoord2i(0, 1); glVertex2f(0,  			texture.pxh); 	//Нижний левый угол
		glTexCoord2i(1, 1); glVertex2f(texture.pxw, texture.pxh); 	//Нижний правый угол
		glTexCoord2i(1, 0); glVertex2f(texture.pxw, 0);  			//Верхний правый угол
	glEnd();

	glLoadIdentity();


}
void image::Draw(float x, float y, float dx, float dy, float delta, int center)
{
	// Отрисовываем текстуру от точки (x, y)
	//	размером dX, dY.
	//	с возможным углом поворота delta относительно верхнего левого угла
	//	либо центра

	Bind();

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

	Bind();

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
void image::Bind()
{
	if(texture.tex)
	{
		glBindTexture(GL_TEXTURE_2D, texture.tex);
    	glLoadIdentity();
    	if(TextureManager && TextureManager->Graphics)
    	{
    		if(TextureManager->Graphics->GetCurrentTexture() != texture.tex)
    			TextureManager->Graphics->SetCurrentTexture(texture.tex);
       	}
	}
}
void image::Reload()
{
	//TODO: Косяк в функции - не удаляется старая текстура
	if(texture.fileName != "")
	{
		Open(texture.fileName);
	}
}
void image::Redraw(float x, float y, float dx, float dy, float delta, int center)
{
	// Полностью перерисовываем текстуру предварительно удалив и перегрузив в память
	// Работает с полными картинками
	Delete();
	Reload();
	if(!texture.tex)
	{
		if((dx < 0)||(dy < 0))
			Draw(x, y); //TODO: проверить
		else
			Draw(x, y, dx, dy, delta, center);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Redraw error" << endl;
#endif
	}
}
void image::Redraw(float width, float heigth, float top_x, float top_y, float top_dx, float top_dy,
				   float x, float y, float dx, float dy, float delta, int center)
{
	// Полностью перерисовываем текстуру предварительно удалив и перегрузив в память
	// Работает с куском изображения
	Delete();
	Reload();
	if(!texture.tex)
	{
		if((dx < 0)||(dy < 0))
			Draw(width, heigth, top_x, top_y, top_dx, top_dy,
					   x, y, texture.pxh, texture.pxw, delta, center); //TODO: проверить
		else
		Draw(width, heigth, top_x, top_y, top_dx, top_dy,
				   x, y, dx, dy, delta, center);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Redraw error" << endl;
#endif
	}
}
void image::Delete()
{
	//Удаляем gl текстуру изображения
	if(texture.tex)
	{
		glDeleteTextures(1, &texture.tex);
		texture.tex = 0;
	}
	if(TextureManager)
	{
		// Если менеджер задан, то анамаджим текстуру, но не удаляем - удалять будем только delete
		TextureManager->UnManageTexture(this);
		TextureManager = 0;
	}
}
void image::SetTexManager(texture_manager *TexManager)
{
	TextureManager = TexManager;
}
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
	texture.pxh = 0;
	texture.pxw = 0;
	texture.tex = 0;

	TextureManager = 0;
}
image::image(std::string file, GLint filter)
{
	texture.fileName = file;
	texture.pxh = 0;
	texture.pxw = 0;
	texture.tex = 0;

	TextureManager = 0;
	Open(file, filter);
}
image::~image()
{
	Delete();
}
texture_manager::texture_manager()
{
	Graphics = 0;
	if(!Textures.empty())
	Textures.clear();
}
texture_manager::~texture_manager()
{
	DeleteTextures();
	Textures.clear();
	Graphics = 0;
}
image *texture_manager::GetTextureInfos(GLuint texture)
{
	// Получаем информацию о текстуре из её ID
	for(unsigned int loop = 0; loop < Textures.size(); loop++)
	{
		if(Textures[loop]->texture.tex == texture)
		{
			return Textures[loop];
		}
	}
	return 0;
}
void texture_manager::ReloadTextures()
{
	for(unsigned int loop = 0; loop < Textures.size(); loop++)
	{
		Textures[loop]->Reload();
	}
}
/*
 TODO: доделать/удалить перерисовку
void texture_manager::RedrawTextures()
{
	for(int loop = 0; loop < Textures.size(); loop++)
	{
		Textures[loop]->Redraw();
	}
}
*/
void texture_manager::DeleteTextures()
{
	//Удаляем текстуры
	for(unsigned int loop = 0; loop < Textures.size(); loop++)
	{
		delete Textures[loop];
	}
}
void texture_manager::ManageTexture(image *managed_image)
{
	//Добавляем текстуру в вектор для управления
	for(unsigned int loop = 0; loop < Textures.size(); loop++)
	{
		if(Textures[loop]->texture.tex == managed_image->texture.tex)
		{
			return;
		}
	}
	if(!managed_image->TextureManager)
		managed_image->SetTexManager(this);

	Textures.push_back(managed_image);
}
void texture_manager::UnManageTexture(image *managed_image)
{
	// Удаляем текстуру из вектора управления
	// Внимание: Это только удалит текстуру из вектора управления, но не удалит саму текстуру
	// 	для этого нужно использовать delete - внутри деструктора вызовется Delete() и сделает всё необходимое
	// Лучше использовать delete вместо UnManageTexture - внутри деструктора вызовется UnManage

	int place = -1;

	// Ищем текстуру с данным ID в памяти
	for(unsigned int loop = 0; loop < Textures.size(); loop++)
	{
		if(Textures[loop]->texture.tex == managed_image->texture.tex)
		{
			place = loop;
			break;
		}
	}

	// Текстура не найдена - выходм
	if(place < 0)
		return;

	if((unsigned int)(place+1) == Textures.size())
	{
		// Текстура в самом конце - удаляем, перед этим обснулив указатель на менеджер текстур
		Textures[place]->TextureManager = 0;
		Textures.pop_back();
	}
	else
	{
		// Текстура где-то внутри вектора - удаляем, перед этим обснулив указатель на менеджер текстур
		//TODO: проверить
		//Textures[place] = Textures[ Textures.size() - 1 ];
		Textures[place]->TextureManager = 0;
		Textures.erase( Textures.begin() + place);
	}
}
