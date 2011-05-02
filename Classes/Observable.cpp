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
}

void Observable::updateObservers() {
	for (std::vector<IObserver*>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->update();
	}
}
