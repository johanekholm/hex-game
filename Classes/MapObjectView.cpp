/*
 *  UnitView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MapObjectView.h"
#include "UnitView.h"
#include "geometry.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "ViewControllerManager.h"
#include "toolkit.h"
#include "Action.h"
#include "ShapeImage.h"

#include <math.h>
#include <iostream>


MapObjectView::~MapObjectView() {
	delete _objectImage;
	delete _actionImage;
    delete _actionMarker;
    _actionPoints.clear();
	_objectModel = 0;
}

MapObjectView::MapObjectView(MapObject* model, GLfloat width, GLfloat height, int index) {
	_objectModel = model;
    _width = width;
    _height = height;
	//_objectImage = new GameImage(width, height, TextureCatalog::instance()->get("mapObjects"), index);
    _objectImage = new GameImage(width, height, TextureCatalog::instance()->get("units"), index);
	_actionImage = new GameImage(32.0f, 32.0f, TextureCatalog::instance()->get("actions"), 0);
    _actionMarker = new EllipseImage(RGBAMake(0.0f, 1.0f, 0.0f, 1.0f), 24.0f, 24.0f, 16, false);
    _selectedActionView = 0;
}

void MapObjectView::destroyed() {
    ViewControllerManager::instance()->remove(this);	
}

void MapObjectView::draw() {
	_objectImage->drawAt(_pos);
}

void MapObjectView::drawActions() {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
        if (!(*it).active) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
		_actionImage->drawAtWithSubTexture((*it).pos, (*it).actionId);
        
        if (_selectedActionView != 0) {
            _actionMarker->drawCenteredAt((*_selectedActionView).pos);
        }        
	}
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void MapObjectView::drawGUI() {
	if (this->_hasFocus) {
        _actionMarker->drawCenteredAt(_pos);
        //this->drawActions();
    }
}

GPoint MapObjectView::getActionPosition(int index) {
	return GPointMake(cos(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index), 
					  sin(ACTION_ANGLE_INITIAL + ACTION_ANGLE_INCREMENT*(float)index)) * ACTION_RADIUS;
}

ActionState* MapObjectView::getTouchedActionState(GPoint point) {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return (*it).statePoint;
		}
	}
	return (0);
}

ActionView* MapObjectView::getTouchedActionView(GPoint point) {
	
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {		
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return &(*it);
		}
	}
	return (0);
}

bool MapObjectView::handleEvent(const TouchEvent& event) {
    ActionState* statePoint;
    
	if (event.type == 3) {
        statePoint = this->getTouchedActionState(event.point);
        
        if (statePoint != 0) {
            //_objectModel->doAction(*statePoint);
        }
	}
    
    if (event.type == 1) {
        _selectedActionView = this->getTouchedActionView(event.point);
	}
	return false;
}

void MapObjectView::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
    this->update();
}

void MapObjectView::update() {
    _state = _objectModel->getState();
	this->updatePosition(_state.pos);
	//this->updateActions();
}

void MapObjectView::updateActions() {
	ActionView actionView;
    
    _actionPoints.clear();
    
    for (std::vector<ActionState>::iterator it = _state.actions.begin(); it != _state.actions.end(); ++it) {
        actionView.pos = transformModelPositionToView((*it).pos);
        actionView.actionId = (*it).actionId;
        actionView.active = (*it).active;
        actionView.statePoint = &(*it);
        _actionPoints.push_back(actionView);
    }	
}

void MapObjectView::updatePosition(const MPoint& pos) {
	_pos.x = 64.0f + ((GLfloat)pos.x * 64.0f + (pos.y % 2) * 32.0f) * 1.5f;
	_pos.y = 64.0f + ((GLfloat)pos.y * 50.0f) * 1.5f;
}

