/*
 * SystemConstants.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef SYSTEMCONSTANTS_H_
#define SYSTEMCONSTANTS_H_

#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>

#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef WIN32
#include <GL/glext.h>
#endif /* WIN32 */

///////////////// MAIN SYSTEM CONSTANTS /////////////////

const int 			SYS_AUDIO = 0; 					//аудио(0 - отключено, !0 - включено)
const int			SYS_FULLSCREEN = 0;				//оконный режим(0 - оконный режим, !0 - полноэкранны режим)
const int			SYS_REGULATING_FRAME_RATE = 0;	//регулируем ли FPS(0 - нет, !0 - да)

const int			SYS_WIDTH = 1024;				//ширина
const int			SYS_HEIGTH = 768;				//высота
const int			SYS_BPP = 32;					//палитра
const int			SYS_FPS = 60;					//FPS

const int			SYS_FRAME_PIXELS = 10;


///////////////// GRAPHIC SYSTEM CONSTANTS /////////////////

const GLint			SYS_GL_IMG_FILTER = GL_NEAREST; //Стандартный фильтр для изображений
const int			SYS_TEXT_SIZE = 16;				//Размер текста по умолчанию
const int			SYS_TEXT_DEPTH = 32;			//Глубина прорисовки текста
const std::string	SYS_GL_IMG_ZIP_MODE = "rb";

///////////////// AUDIO SYSTEM CONSTANTS /////////////////

const int			SYS_AUDIO_VOLUME = 64;
const int			SYS_AUDIO_RATE = 44100;
const Uint16 		SYS_AUDIO_FORMAT = AUDIO_S16; // 16-bit stereo
const int			SYS_AUDIO_CHANNELS = 2;
const int 			SYS_AUDIO_BUFFERS = 1024;

///////////////// VERSIONS CONSTANTS /////////////////

const std::string 	SYS_VERSION = "0.0.0.0.43";
const std::string 	SYS_BUILD = "000043";
const std::string	SYS_TEST_VERSION = "0.1.8.43";


#endif /* SYSTEMCONSTANTS_H_ */
