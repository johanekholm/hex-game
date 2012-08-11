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
#include "ShapeImage.h"
#include <iostream>


ViewControllerManager* ViewControllerManager::_instance = 0;

ViewControllerManager* ViewControllerManager::instance() {
    if (_instance == 0) {
        _instance = new ViewControllerManager();
    }		
    return _instance;
}

void ViewControllerManager::destroy() {
	if (_instance != 0) {
        delete _instance->_mapView;
        
        for (std::vector<ViewController*>::iterator it = _instance->_views.begin(); it != _instance->_views.end(); ++it) {
            delete *it;
        }
        
		_instance->_views.clear();
        _instance->_stagedViews.clear();
        delete _instance->_hudBackground;
		delete _instance;
		_instance=0;
	}
}

ViewControllerManager::ViewControllerManager() {
    _focus = 0;
    _mapView = 0;
    _cameraPos = 0;
	_cameraTargetPos = 0;
    _hudBackground = new RectangleImage(RGBAMake(0.1f, 0.1f, 0.1f, 1.0f), 320.0f, 80.0f, true);
}

/*ViewControllerManager::~ViewControllerManager() {
	_views.clear();	
}*/


void ViewControllerManager::add(ViewController* view) {
    _stagedViews.push_back(view);
}

GPoint ViewControllerManager::adjustForCamera(const GPoint& pos) {
    return pos + _cameraPos;
}

void ViewControllerManager::applyCameraBoundaries(GPoint& pos) {
	GPoint nwBoundary = _mapView->getNWBoundary(); //GPointMake(64.0f, 40.0f);
	GPoint seBoundary = _mapView->getSEBoundary() - GPointMake(320.0f, 400.0f);
	
	if (seBoundary.x < 0.0f) {
		seBoundary.x = 0.0f;
	}
	
	if (seBoundary.y < 0.0f) {
		seBoundary.y = 0.0f;
	}

	if (pos.x < nwBoundary.x) {
        pos.x = nwBoundary.x;
    }
    
    if (pos.y < nwBoundary.y) {
        pos.y = nwBoundary.y;
    }    

	if (pos.x > seBoundary.x) {
        pos.x = seBoundary.x;
    }
    
    if (pos.y > seBoundary.y) {
        pos.y = seBoundary.y;
    }    
}

void ViewControllerManager::centerCamera(const GPoint& pos, bool sweep) {
    this->setCameraTargetPosition(pos - GPointMake(160.0f, 240.0f));
	
	if (!sweep) {
		this->setCameraPosition(pos - GPointMake(160.0f, 240.0f));		
	}
}

void ViewControllerManager::centerCamera(const MPoint& pos, bool sweep) {
    this->setCameraTargetPosition(this->transformModelPositionToView(pos) - GPointMake(160.0f, 240.0f));

	if (!sweep) {
		this->setCameraPosition(this->transformModelPositionToView(pos) - GPointMake(160.0f, 240.0f));
	}
}

void ViewControllerManager::draw() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if (*it != 0) {
            (*it)->draw(_cameraPos);            
        }
	}
}

void ViewControllerManager::drawGUI() {
    _hudBackground->drawAt(GPointMake(0.0f, 400.0f));
    
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if (*it != 0) {
            (*it)->drawGUI(_cameraPos);
        }
	}
}

void ViewControllerManager::drawMap() {
    if (_mapView != 0) {
        _mapView->draw(_cameraPos);
    }
}

ViewController* ViewControllerManager::getFocus() {
    return _focus;
}

ViewController* ViewControllerManager::getTouched(const GPoint& point) {    
	for (std::vector<ViewController*>::reverse_iterator it = _views.rbegin(); it != _views.rend(); ++it) {
		if (*it != 0) {
            if ((*it)->isWithin(point)) {
                return (*it);	
            }
        }
	}
	
	return 0;
}

void ViewControllerManager::insert(ViewController* view) {
    std::vector<ViewController*>::iterator it = _views.begin();
	while (true) {
		if (it == _views.end() || (*it != 0 && (*it)->getLayer() >= view->getLayer())) {
            _views.insert(it, view);
            break;
        }
        ++it;
	}
}

void ViewControllerManager::moveCamera(const GPoint& pos) {
    this->setCameraPosition(_cameraPos + pos);
    this->setCameraTargetPosition(_cameraPos + pos);
}

void ViewControllerManager::passFocus() {
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
		if (*it != 0 && *it != _focus && (*it)->catchFocus()) {
            this->setFocus(*it);
        }
	}
}

void ViewControllerManager::purge() {
    for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        delete *it;
    }
    
    _views.clear();
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

void ViewControllerManager::removeAllSoftly() {
    _focus = 0;
	
    for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        delete (*it);
        *it = 0;
	}    
}

void ViewControllerManager::removeSoft(ViewController* view) {
    if (view == _focus) {
        _focus = 0;
    }

	for (std::vector<ViewController*>::iterator it = _stagedViews.begin(); it != _stagedViews.end(); ++it) {
        if (*it == view) {
            delete (*it);
            it = _stagedViews.erase(it);
            return;
        }
	}

	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end(); ++it) {
        if (*it == view) {
            delete (*it);
            *it = 0;            
            return;
        }
	}
}

void ViewControllerManager::setCameraPosition(const GPoint& pos) {
    _cameraPos = pos;
    
	this->applyCameraBoundaries(_cameraPos);
}

void ViewControllerManager::setCameraTargetPosition(const GPoint& pos) {
    _cameraTargetPos = pos;
	
	this->applyCameraBoundaries(_cameraTargetPos);
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
    delete _mapView;    
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

void ViewControllerManager::translateToCamera() {
    glLoadIdentity();
	glTranslatef(-_cameraPos.x, -_cameraPos.y, 0.0f);
}

void ViewControllerManager::translateToCameraAndPosition(const GPoint& pos) {
    glLoadIdentity();
	glTranslatef(-_cameraPos.x + pos.x, -_cameraPos.y + pos.y, 0.0f);
}

void ViewControllerManager::update() {
	this->updateCamera();
	
    // update viewcontrollers, erase those that have been soft-removed in a controlled fashion
	for (std::vector<ViewController*>::iterator it = _views.begin(); it != _views.end();) {
		if ((*it) != 0) {
            (*it)->update();
            ++it;
        } else {
            it = _views.erase(it);
        }
	}
	
	// insert newly added views before starting loop 
    if (_stagedViews.size() > 0) {
        for (std::vector<ViewController*>::iterator it = _stagedViews.begin(); it != _stagedViews.end(); ++it) {
            this->insert(*it);
        }
        _stagedViews.clear();        
    }
}

void ViewControllerManager::updateCamera() {
	GPoint delta;
	GPoint toTarget;
	GLfloat length, speed;
	
	toTarget = _cameraTargetPos - _cameraPos;
	
	if (toTarget != 0) {
		length = toTarget.length();

		if (length < 0.1f) {
			_cameraPos = _cameraTargetPos;
		} else {
			speed = 0.001f * length;
			
			if (speed < 0.05f) {
				speed = 0.05f;
			}
			delta = (_cameraTargetPos - _cameraPos) * speed;
			_cameraPos += delta;			
		}
	}
}

