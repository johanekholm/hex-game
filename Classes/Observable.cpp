/*
 *  Observable.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Observable.h"
#include "IObserver.h"

/*Observable::~Observable() {
	
}*/

void Observable::addObserver(IObserver* observer) {
	_observers.push_back(observer);
    observer->updateState();
}

void Observable::removeObserver(IObserver* observer) {
	for (std::vector<IObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		if ((*it) == observer) {
            _observers.erase(it);
            return;
        }
	}    
}

void Observable::updateObservers() {
	for (std::vector<IObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->updateState();
	}
}

void Observable::updateObserversDestroyed() {
	for (std::vector<IObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->destroyed();
	}
}
