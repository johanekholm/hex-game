/*
 *  BaseUnitViewController.cpp
 *
 *  Created by Johan Ekholm.
 *
 */

#include "BaseUnitViewController.h"

#include "Action.h"
#include "GameImage.h"
#include "InputManager.h"
#include "ShapeImage.h"
#include "TextureCatalog.h"
#include "ViewControllerManager.h"
#include "toolkit.h"

#include <iostream>


BaseUnitViewController::~BaseUnitViewController() {
	delete _actionImage;
    delete _actionMarker;
    _actionPoints.clear();
}

BaseUnitViewController::BaseUnitViewController(GLfloat width, GLfloat height, int index) {
    _width = width;
    _height = height;
	_actionImage = new GameImage(32.0f, 32.0f, TextureCatalog::instance()->get("actions"), 0);
    _actionMarker = new EllipseImage(RGBAMake(0.0f, 1.0f, 0.0f, 1.0f), 24.0f, 24.0f, 16, false);
    _selectedActionView = 0;
}

void BaseUnitViewController::destroyed() {
    ViewControllerManager::instance()->remove(this);	
}

void BaseUnitViewController::drawActions(const GPoint& cameraPos) {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
        if (!(*it).active) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
		_actionImage->drawAtWithSubTexture((*it).pos - cameraPos, (*it).actionId);
        
        if (_selectedActionView != 0) {
            _actionMarker->drawCenteredAt((*_selectedActionView).pos - cameraPos);
        }        
	}
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void BaseUnitViewController::drawGUI(const GPoint& cameraPos) {
	if (this->_hasFocus) {
        _actionMarker->drawCenteredAt(_pos - cameraPos);
        this->drawActions(cameraPos);
    }
}

ActionState* BaseUnitViewController::getTouchedActionState(GPoint point) {
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return (*it).statePoint;
		}
	}
	return 0;
}

ActionView* BaseUnitViewController::getTouchedActionView(GPoint point) {
	
	for (std::vector<ActionView>::iterator it = _actionPoints.begin(); it != _actionPoints.end(); ++it) {		
		if (PointWithin(point, (*it).pos, 32.0f)) {
			return &(*it);
		}
	}
	return (0);
}

void BaseUnitViewController::setFocus(bool hasFocus) {
    _hasFocus = hasFocus;
    this->updateState();
}

void BaseUnitViewController::updateActions(std::vector<ActionState>& actionStates) {
	ActionView actionView;
    
    _actionPoints.clear();
    
    for (std::vector<ActionState>::iterator it = actionStates.begin(); it != actionStates.end(); ++it) {
        if ((*it).targetType == ActionNS::TARGET_SELF) {
            actionView.pos = GPointMake(60.0f, 400.0f);
        } else {
            actionView.pos = ViewControllerManager::instance()->transformModelPositionToView((*it).pos);            
        }
        actionView.actionId = (*it).actionId;
        actionView.active = (*it).active;
        actionView.statePoint = &(*it);
        _actionPoints.push_back(actionView);
    }	
}

void BaseUnitViewController::updatePosition(const MPoint& pos) {
    _pos = ViewControllerManager::instance()->transformModelPositionToView(pos);
}

