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
#include <iostream>


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
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            delete (*it);
            it = _views.erase(it);
            return;
        }
	}
}

void ViewControllerManager::removeSoft(ViewController* view) {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            std::cout << "Soft remove " << *it << std::endl;
            delete (*it);
            *it = 0;
            std::cout << "*it= " << *it << std::endl;

            return;
        }
	}
}


void ViewControllerManager::draw() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if (*it != 0) {
            (*it)->draw();            
        }
	}
}

void ViewControllerManager::drawGUI() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if (*it != 0) {
            (*it)->drawGUI();
        }
	}
}

void ViewControllerManager::update() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end();) {
		if ((*it) != 0) {
            (*it)->update();
            ++it;
        } else {
            //std::cout << "Erasing it " << *it << " size is " << _views.size() << std::endl;

            it = _views.erase(it);
            //std::cout << "Size after is " << _views.size() << std::endl;
        }
        
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
