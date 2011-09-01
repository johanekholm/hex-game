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
#include "HexMap.h"
#include <iostream>


ViewControllerManager* ViewControllerManager::_instance = 0;

void ViewControllerManager::destroy() {
	if (_instance != 0) {
        delete _instance->_mapView;
        delete _instance->_pushedMapView;
        
        for (std::vector<ViewController*>::iterator it = _instance->_views.begin(); it != _instance->_views.end(); ++it) {
            delete *it;
        }
        
		_instance->_views.clear();
		delete _instance;
		_instance=0;
	}
}

ViewControllerManager::ViewControllerManager() {
    _focus = 0;
    _mapView = 0;
    _pushedMapView = 0;
}

/*ViewControllerManager::~ViewControllerManager() {
	_views.clear();	
}*/


void ViewControllerManager::add(ViewController* view) {
	_views.push_back(view);
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

void ViewControllerManager::drawMap() {
    if (_mapView != 0) {
        _mapView->draw();
    }
}

ViewController* ViewControllerManager::getFocus() {
    return _focus;
}

ViewController* ViewControllerManager::getTouched(const GPoint& point) {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if ((*it)->isWithin(point)) {
			return (*it);	
		}
	}
	
	return 0;
}

void ViewControllerManager::popMapView() {
    delete _mapView;
    _mapView = _pushedMapView;
    _pushedMapView = 0;
}

void ViewControllerManager::purge() {
    for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        delete *it;
    }
    
    _views.clear();
}

void ViewControllerManager::pushMapView(HexMap* mapView) {
    if (_pushedMapView != 0) {
        delete _pushedMapView;
    }
    _pushedMapView = _mapView;
    _mapView = mapView;
}

void ViewControllerManager::remove(ViewController* view) {
    if (view == _focus) {
        _focus = 0;
    }
	
    for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            delete (*it);
            it = _views.erase(it);
            return;
        }
	}
}

void ViewControllerManager::removeSoft(ViewController* view) {
    if (view == _focus) {
        _focus = 0;
    }

	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            //std::cout << "Soft remove " << *it << std::endl;
            delete (*it);
            *it = 0;
            //std::cout << "*it= " << *it << std::endl;
            
            return;
        }
	}
}

void ViewControllerManager::setFocus(ViewController* view) {
    // notify of focus lost
    if (_focus != 0) {
        _focus->setFocus(false);
    }

    _focus = view;

    // notify of focus won
    if (view != 0) {
        view->setFocus(true);
    }
}

void ViewControllerManager::setMapView(HexMap* mapView) {
    _mapView = mapView;
}

GPoint ViewControllerManager::transformModelPositionToView(const MPoint& pos) {
	GPoint viewPos;
    GLfloat scale = 1.0f;
    
    if (_mapView != 0) {
        scale = _mapView->getScale();
    }
    
    viewPos.x = 64.0f + ((GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f) * scale;
	viewPos.y = 64.0f + ((GLfloat)pos.y * 50.0f) * scale;
    return viewPos;
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

