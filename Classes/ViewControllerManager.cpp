/*
 *  ViewControllerManager.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ViewControllerManager.h"
#include "ViewController.h"

ViewControllerManager* ViewControllerManager::_instance = 0;

void ViewControllerManager::destroy() {
	if (_instance != 0) {
		_instance->_views.clear();
		delete _instance;
		_instance=0;
	}
}

ViewControllerManager::ViewControllerManager() {

}

/*ViewControllerManager::~ViewControllerManager() {
	_views.clear();	
}*/


void ViewControllerManager::add(ViewController* view) {
	_views.push_back(view);
}

void ViewControllerManager::remove(ViewController* view) {
    // to-do: remove from vector and delete
}

void ViewControllerManager::draw() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		(*it)->draw();
	}
}

void ViewControllerManager::drawGUI() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		(*it)->drawGUI();
	}
}

ViewController* ViewControllerManager::getTouched(const GPoint& point) {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if ((*it)->isWithin(point)) {
			return (*it);	
		}
	}
	
	return 0;
}
