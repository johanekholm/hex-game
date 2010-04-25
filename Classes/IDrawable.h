/*
 *  IDrawable.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef IDRAWABLE_H
#define IDRAWABLE_H

class IDrawable {
public:
	virtual ~IDrawable() {}
	virtual GPoint getDrawPosition() = 0;
};

#endif