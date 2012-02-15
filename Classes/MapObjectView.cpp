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
	ActionView* action;
        
	if (event.type == 3) {
		action = this->getTouchedActionView(event);
        
        if (action != 0) {
            _objectModel->doAction(*(action->statePoint));
            _selectedActionView = 0;
            return true;
        }
	}
    
    if (event.type == 1) {
        _selectedActionView = this->getTouchedActionView(event);
        return true;
	}
	return false;
}

void MapObjectView::updateState() {
    _state = _objectModel->getState();
	this->updatePosition(_state.pos);
	this->updateActions(_state.actions);
}
