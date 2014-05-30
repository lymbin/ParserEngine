/*
 * Color.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

class cColor
{
public:
	float r, g, b, a;

	cColor(float afR, float afG, float afB, float afA);
	cColor(float afR, float afG, float afB);
	cColor();
	cColor(float afColor);
	cColor(float afColor, float afA);

	cColor operator*(float afColor) const;
	cColor operator/(float afColor) const;

	cColor operator+(const cColor &aCol) const;
	cColor operator-(const cColor &aCol) const;
	cColor operator*(const cColor &aCol) const;
	cColor operator/(const cColor &aCol) const;

	bool operator==(cColor aCol) const;

	std::string ToString() const;
};


#endif /* COLOR_H_ */
