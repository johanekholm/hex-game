/*
 *  MapObjectView.cpp
 *
 *  Created by Johan Ekholm.
 *
 */

#include "MapObjectView.h"
#include "geometry.h"
#include "TextureCatalog.h"
#include "InputManager.h"
#include "ViewControllerManager.h"
#include "toolkit.h"
#include "Action.h"
#include "ShapeImage.h"
#include "GameImage.h"

#include <math.h>
#include <iostream>


MapObjectView::~MapObjectView() {
	delete _objectImage;
	_objectModel = 0;
}

MapObjectView::MapObjectView(MapObject* model, GLfloat width, GLfloat height, int index, int layer) : BaseUnitViewController(width, height, index, layer) {
	_objectModel = model;
    _objectImage = new GameImage(width, height, TextureCatalog::instance()->get("units"), index);
}

void MapObjectView::draw(const GPoint& cameraPos) {
	_objectImage->drawAt(_pos - cameraPos);
}

bool MapObjectView::handleEvent(const TouchEvent& event) {
    ActionState* statePoint;
        
	if (event.type == 3) {
        statePoint = this->getTouchedActionState(event.translatedPoint);
        
        if (statePoint != 0) {
            _objectModel->doAction(*statePoint);
            _selectedActionView = 0;
            return true;
        }
	}
    
    if (event.type == 1) {
        _selectedActionView = this->getTouchedActionView(event.translatedPoint);
        return true;
	}
	return false;
}

void MapObjectView::updateState() {
    _state = _objectModel->getState();
	this->updatePosition(_state.pos);
	this->updateActions(_state.actions);
}
