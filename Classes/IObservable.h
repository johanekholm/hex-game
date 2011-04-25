/*
 *  IObservable.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <vector>

class IObservable {
public:
	virtual ~IObservable() {}
	virtual void update() = 0;
};

#endif
