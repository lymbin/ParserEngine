/*
 * Color.cpp
 *
 *  Created on: 29.05.2014
 *      Author: dmitry
 */

#include "Color.h"

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cColor::cColor(float afR, float afG, float afB, float afA)
{
	r = afR;
	g =	afG;
	b = afB;
	a = afA;
}

//-----------------------------------------------------------------------

cColor::cColor(float afR, float afG, float afB)
{
	r = afR;
	g =	afG;
	b = afB;
	a = 1.0;
}

//-----------------------------------------------------------------------

cColor::cColor()
{
	r = g = b = 0.0;
	a = 1.0;
}

//-----------------------------------------------------------------------

cColor::cColor(float afColor)
{
	r = g = b = afColor;
	a = 1.0;
}
cColor::cColor(float afColor, float afA)
{
	r = g = b = afColor;
	a = afA;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cColor cColor::operator*(float afColor) const
{
	cColor col;
	col.r = r*afColor;
	col.g = g*afColor;
	col.b = b*afColor;
	col.a = a*afColor;
	return col;
}

//-----------------------------------------------------------------------

cColor cColor::operator/(float afColor) const
{
	cColor col;
	col.r = r/afColor;
	col.g = g/afColor;
	col.b = b/afColor;
	col.a = a/afColor;
	return col;
}

//-----------------------------------------------------------------------

cColor cColor::operator+(const cColor &aCol) const
{
	return cColor(
			r + aCol.r,
			g + aCol.g,
			b + aCol.b,
			a + aCol.a);
}

//-----------------------------------------------------------------------

cColor cColor::operator-(const cColor &aCol) const
{
	return cColor(
			r - aCol.r,
			g - aCol.g,
			b - aCol.b,
			a - aCol.a);
}

//-----------------------------------------------------------------------

cColor cColor::operator*(const cColor &aCol) const
{
	return cColor(
			r * aCol.r,
			g * aCol.g,
			b * aCol.b,
			a * aCol.a);
}

//-----------------------------------------------------------------------

cColor cColor::operator/(const cColor &aCol) const
{
	return cColor(
			r / aCol.r,
			g / aCol.g,
			b / aCol.b,
			a / aCol.a);
}

//-----------------------------------------------------------------------

bool cColor::operator==(cColor aCol) const
{
	if(r == aCol.r && g == aCol.g && b == aCol.b && a == aCol.a)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

std::string cColor::ToString() const
{
	std::stringstream sstream;
	sstream << r << " : " << g << " : " << b << " : " << a;
	return sstream.str();
}

//-----------------------------------------------------------------------


