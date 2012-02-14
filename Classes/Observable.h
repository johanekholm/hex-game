/*
 *  Observable.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>

class IObserver;

class Observable {
	std::vector<IObserver*> _observers;
public:
	//~Observable();
	void addObserver(IObserver* observer);
    void removeObserver(IObserver* observer);
	void updateObservers();
    void updateObserversDestroyed();

};

#endif
