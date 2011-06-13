/*
 *  IObserver.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef IOBSERVER_H
#define IOBSERVER_H

//#include <vector>

class IObserver {
public:
	virtual ~IObserver() {}
	virtual void update() = 0;
    virtual void destroyed() = 0;
};

#endif
