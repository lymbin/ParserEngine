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

	// Загружаем текстуру во временную SDL поверхность
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

	// Создаём GL текстуру из SDL поверхности
	MakeTexture(temp_surface);

	// Освобождаем SDL поверхность
	SDL_FreeSurface(temp_surface);

	if(TextureManager)
	{
		// Если задан менеджер текстур, то добавляем эту текстуру в менеджер
		TextureManager->ManageTexture(this);
	}
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
void image::DrawTransform(float x, float y, PE_Rect *Box, GLfloat Scale, GLfloat Rotation)
{
	// Трансформируем расположение текстуры на экране
	// Доступно: 	изменение позиции на экране
	//				изменение размера текстуры
	//				поворот текстуры

	float dx, dy;

	if(Box->Width >= texture.pxw)
		dx = texture.pxw;
	else
		dx = Box->Width;

	if(Box->Heigth >= texture.pxh)
		dy = texture.pxh;
	else
		dy = Box->Heigth;

	glLoadIdentity();
	glTranslatef(x,y,0); // Смещаемся в точку (x, y)

	if(Rotation)
	{
		// Поворот
		GLfloat rotate_x, rotate_y;

		// Начальные значения координат поворота
		rotate_x = dx/2;
		rotate_y = dy/2;

		// Магические формулы смещения координат поворота в случае не квадратных текстур(кусков текстур)
		// TODO: немного косячные формулы - лучше перепроверить для разных случаев
		if(Rotation > 0)
			while(Rotation >= 360)
				Rotation-=360;
		else
			while(Rotation <= -360)
				Rotation+=360;

		if(dx > dy)
			rotate_x -= ((dx - dy)/2);
		else if(dx < dy)
			rotate_y += ((dy - dx)/2);

		glTranslatef(rotate_x,rotate_y,0); 		// Смещаемся в точку (rotate_x, rotate_y)
		glRotatef(Rotation, 0.0f, 0.0f, 1.0f);	// Поворачиваем текстуру относительно предыдущей точки Translate на Rotate градусов по часовой стрелке
		glTranslatef(-rotate_x,-rotate_y,0); 	// Возвращаемся в исходное состояние - в точку (x, y)
	}
	// Изменение размера текстуры
	glScaled(Scale, Scale, 0);
}
void image::Draw(float x, float y, GLfloat Scale, GLfloat Rotation,
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	// Отрисовываем текстуру от точки (x, y)
	// 	полного размера
	// TODO: проследить за правильностью отрисовки не квадратных тектур
	// TODO: пока никак не используем размер и поворот - доделать это
	Bind();

	glEnable(GL_TEXTURE_2D);

	PE_Rect Box;
	Box.X = 0;
	Box.Y = 0;
	Box.Width = 1;
	Box.Heigth = 1;

	DrawTransform(x, y, &Box, Scale, Rotation);


	//Рисуем текстуру
	glBegin(GL_QUADS);
		glColor4f(red, green, blue, alpha); // Устанавливаем цвет

		glTexCoord2i(0, 0); glVertex2f(0,  			0);  			//Верхний левый угол
		glTexCoord2i(0, 1); glVertex2f(0,  			texture.pxh); 	//Нижний левый угол
		glTexCoord2i(1, 1); glVertex2f(texture.pxw, texture.pxh); 	//Нижний правый угол
		glTexCoord2i(1, 0); glVertex2f(texture.pxw, 0);  			//Верхний правый угол
	glEnd();

	glLoadIdentity();

}
void image::Draw(float x, float y, PE_Rect *Box,
			GLfloat Scale, GLfloat Rotation,
			GLfloat red, GLfloat green , GLfloat blue, GLfloat alpha)
{
	/*Отрисовка куска текстуры в точке (x, y)
	 *  с параметрами куска - (top_x, top_y) (dx, dy)
	 *  с заданным увеличением и поворотом текстуры(по умолчанию отсутствует)
	 *  а также цветом текстуры(по умолчанию белый)
	 *  TODO: пока никак не используем размер и поворот - доделать это
	 */
	Bind(); // Биндим текстуру

	float top_x = Box->X;
	float top_y = Box->Y;

	float dx, dy;

	dx = Box->Width;
	dy = Box->Heigth;

	glEnable(GL_TEXTURE_2D);

	DrawTransform(x, y, Box, Scale, Rotation);



	// Отрисовываем текстуру
	glBegin(GL_QUADS);
		glColor4f(red, green, blue, alpha); // Устанавливаем цвет
		glTexCoord2f((top_x/texture.pxw),		(top_y/texture.pxh));		glVertex2f(0, 0); //Верхний левый угол
		glTexCoord2f((top_x/texture.pxw),		((top_y+dy)/texture.pxh));	glVertex2f(0, dy);//Нижний левый угол
		glTexCoord2f(((top_x+dx)/texture.pxw),	((top_y+dy)/texture.pxh)); 	glVertex2f(dx,dy);//Нижний правый угол
		glTexCoord2f(((top_x+dx)/texture.pxw),	(top_y/texture.pxh));		glVertex2f(dx, 0);//Верхний правый угол
	glEnd();
	glLoadIdentity();

}



void image::Bind()
{
	// Биндим текстуру
	if(texture.tex)
	{
		glBindTexture(GL_TEXTURE_2D, texture.tex);
    	glLoadIdentity();
    	if(TextureManager && TextureManager->Graphics)
    	{
    		// Если менеджер текстур и графика в нём определены, то меняем текущуюю текстуру
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
void image::Redraw(float x, float y, GLfloat Scale, GLfloat Rotatation,
			GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	// Полностью перерисовываем текстуру предварительно удалив и перегрузив в память
	// Работает с полными картинками
	Delete();
	Reload();
	if(!texture.tex)
	{
		Draw(x, y, Scale, Rotatation, red, green, blue, alpha);
	}
	else
	{
#ifdef DEBUG_ERRORS
		cout << "Redraw error" << endl;
#endif
	}
}
void image::Redraw(float x, float y, PE_Rect *Box,
		GLfloat Scale, GLfloat Rotatation ,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	// Полностью перерисовываем текстуру предварительно удалив и перегрузив в память
	// Работает с куском изображения
	Delete();
	Reload();
	if(!texture.tex)
	{
		Draw(x, y, Box, Scale, Rotatation, red, green , blue, alpha);
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
	texture.pxh = 0;
	texture.pxw = 0;
}

// Устанавливаем менеджер текстур для полуавтоматического управления памятью менеджером
void image::SetTexManager(texture_manager *TexManager)
{
	// Если менеджер уже задан - выходим, т.к. менеджер может быть всего один на всю программу
	if(TextureManager)
		return;

	TextureManager = TexManager;

	// Добавляем в менеджер эту текстуру(даже если она там есть)
	TextureManager->ManageTexture(this);
}
float image::Width()
{
	return texture.pxw;
}
float image::Heigth()
{
	return texture.pxh;
}
iTexture image::GetTXT()
{
	return texture;
}
image::image(std::string file,  GLint filter)
{
	texture.fileName = file;
	texture.pxh = 0;
	texture.pxw = 0;
	texture.tex = 0;

	TextureManager = 0;

	if(file != "")
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
void texture_manager::SetGraphics(graphics *setGraphics)
{
	Graphics = setGraphics;
}
