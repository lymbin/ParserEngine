/*
 * MathTypes.h - base math types
 * Taken from HPL Engine
 *
 *  Created on:	14.09.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef PARSERENGINE_INCLUDE_MATHTYPES_H_
#define PARSERENGINE_INCLUDE_MATHTYPES_H_

#include <list>
#include <vector>
#include <cmath>
#include <cstdlib>

#include "Vector2.h"

template <class T> class cRect2 {
public:
	T x,y,w,h;
	cRect2(T aX, T aY, T aW, T aH){
		x = aX; y = aY; w = aW; h = aH;
	}
	cRect2(T aX, T aY){
		x = aX; y = aY; w = 0; h = 0;
	}
	cRect2(){
		x = 0; y = 0; w = 0; h = 0;
	}
	cRect2(cVector2<T> aPos, cVector2<T> aSize){
		x = aPos.x; y = aPos.y; w = aSize.x; h = aSize.y;
	}

	void FromVec(T *apV)
	{
		x = apV[0];
		y = apV[1];
		w = apV[2];
		h = apV[3];
	}
};

typedef cRect2<float> cRect2f;
typedef cRect2<int> cRect2l;
typedef std::list<cRect2f> tRect2lList;
typedef tRect2lList tRect2lListIt;

typedef cVector2<float> cVector2f;
typedef cVector2<int> cVector2l;

typedef std::list<cVector2f> tVector2fList;
typedef tVector2fList::iterator tVector2fListIt;

#endif /* PARSERENGINE_INCLUDE_MATHTYPES_H_ */
