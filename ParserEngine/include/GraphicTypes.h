/*
 * GraphicTypes.h
 *
 *  Created on: 06.05.2014
 *      Author: dmitry
 */

#ifndef GRAPHICTYPES_H_
#define GRAPHICTYPES_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <math.h>

#include <vector>
#include <list>
#include <map>
#include <stack>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

// Графические типы данных //

// Точка
struct PE_Point
{
	GLfloat x;	/**< Координата X */
	GLfloat y;	/**< Координата Y */
};

// Прямоугольник
struct PE_Rect
{
	GLfloat X; 		/**< Координата X верхней левой точки  */
	GLfloat Y;		/**< Координата Y верхней левой точки  */
	GLfloat Width; 	/**< Ширина прямоугольника 	*/
	GLfloat Heigth; /**< Высота прямоугольника 	*/
};

// Четырёхугольник
struct PE_Tetra
{
	GLfloat Left; 		/**< Координата крайней левой точки  */
	GLfloat Right;		/**< Координата крайней правой точки  */
	GLfloat Top; 		/**< Координата крайней верхней точки  */
	GLfloat Bottom;		/**< Координата крайней нижней точки  */
};

typedef std::vector<PE_Rect> tRectVector;


enum eTextAlignment{
	eTextAlignment_Left = 1,
	eTextAlignment_Centered_H = 2,
	eTextAlignment_Right = 4,
	eTextAlignment_Top = 8,
	eTextAlignment_Centered_V = 16,
	eTextAlignment_Bottom = 32
};

#endif /* GRAPHICTYPES_H_ */
