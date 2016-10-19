/*
 * Manager.h - virtual base class for all managers
 *
 *  Created on: 	19.10.2016
 *
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef PARSERENGINE_INCLUDE_MANAGER_H_
#define PARSERENGINE_INCLUDE_MANAGER_H_

class iManager {
public:
	iManager() {}
	virtual ~iManager() {}

	virtual int 	Init() = 0;
	virtual int 	Destroy() = 0;

	virtual void 	Update() { };
};



#endif /* PARSERENGINE_INCLUDE_MANAGER_H_ */
