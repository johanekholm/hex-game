/*
 *  UnitView.cpp
 *
 *  Created by Johan Ekholm.
 *
 */

#include "UnitView.h"

#include "geometry.h"
#include "InputManager.h"
#include "toolkit.h"
#include "ShapeImage.h"
#include "Action.h"
#include "GameImage.h"
#include "TextureCatalog.h"
#include "ViewControllerManager.h"
#include <iostream>

UnitViewController::~UnitViewController() {
	delete _unitImage;
    delete _hpBar;
    delete _apBar;
    delete _hpBarSlot;
    delete _apBarSlot;
	_unitModel = 0;
}

UnitViewController::UnitViewController(UnitModel* model, GLfloat width, GLfloat height, int index) : BaseUnitViewController(width, height, index) {
	_unitModel = model;
	_unitImage = new GameImage(width, height, TextureCatalog::instance()->get("units"), index);
    _hpBar = new RectangleImage(RGBAMake(0.0f, 1.0f, 0.0f, 1.0f), 32.0f, 4.0f, true);
    _apBar = new RectangleImage(RGBAMake(0.0f, 0.0f, 1.0f, 1.0f), 32.0f, 6.0f, true);
    _hpBarSlot = new RectangleImage(RGBAMake(0.5f, 0.5f, 0.5f, 1.0f), 32.0f, 4.0f, true);
    _apBarSlot = new RectangleImage(RGBAMake(0.5f, 0.5f, 0.5f, 1.0f), 32.0f, 6.0f, true);
}

void UnitViewController::draw(const GPoint& cameraPos) {
	_unitImage->drawAt(_pos);
}

void UnitViewController::drawGUI(const GPoint& cameraPos) {
    _hpBarSlot->drawAt(GPointMake(_pos.x - 16.0f, _pos.y + 32.0f));
    _apBarSlot->drawAt(GPointMake(_pos.x- 16.0f, _pos.y + 26.0f));
    
    _hpBar->drawAt(GPointMake(_pos.x - 16.0f, _pos.y + 32.0f));
    _apBar->drawAt(GPointMake(_pos.x- 16.0f, _pos.y + 26.0f));
    
    BaseUnitViewController::drawGUI(cameraPos);
}

bool UnitViewController::handleEvent(const TouchEvent& event) {
    ActionState* statePoint;
    
	if (event.type == 3) {
        statePoint = this->getTouchedActionState(event.point);
        
        if (statePoint != 0) {
            _unitModel->doAction(*statePoint);
        }
	}
    
    if (event.type == 2) {
        _selectedActionView = this->getTouchedActionView(event.point);
	}
	return true;
}

void UnitViewController::updateBars() {
    GLfloat ratioAp, ratioHp, lengthAp, lengthHp;
    RGBA color;
    
    ratioHp = (GLfloat)_state.hp / (GLfloat)_state.maxHp;
    lengthHp = ratioHp * 32.0f;
    
    ratioAp = (GLfloat)_state.ap / (GLfloat)_state.maxAp;
    lengthAp = ratioAp * 32.0f;
    
    if (ratioHp > 0.75f) {
        color.makeGreen(); 
    } else if (ratioHp > 0.25f) {
        color.makeYellow(); 
    } else {
        color.makeRed(); 
    }
    
    _hpBar->setSize(lengthHp, 4.0f);
    _hpBar->setColor(color);
    
    _apBar->setSize(lengthAp, 6.0f);
}

void UnitViewController::updateState() {
    _state = _unitModel->getState();
	this->updatePosition(_state.pos);
	this->updateActions(_state.actions);
	this->updateBars();
}


