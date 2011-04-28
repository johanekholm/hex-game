/*
 *  CentralControl.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CentralControl.h"
#include "TextureCatalog.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "HexMap.h"
#include "InputManager.h"
#include "toolkit.h"

CentralControl* CentralControl::_instance = 0;

CentralControl::CentralControl() {
	_mode = 1;
	
	TextureCatalog* catalog = TextureCatalog::instance();
		
	_hexMap = new HexMap(catalog->get("hexTiles"), 4, 4, 80.0f, 80.0f);
	
	_unit = new UnitModel(0, 0);
	_unitView = new UnitView(64.0f, 64.0f, catalog->get("units"), 0);
	_unit->registerView(_unitView);
	
	_input = new InputManager();
}

void CentralControl::update() {
	TouchEvent event;
	
	if (_input->hasEvent()) {
		
		event = _input->popEvent();
		
		// dispatch event to current event handler
		switch (this->_mode) {
			case 1:
				this->handleEventNormal(event);
				break;
				
			case 2:
				this->handleEventFocus(event);				
				break;
				
		}
	}
}

void CentralControl::draw() {
	
	switch(_mode) {
		case 1:
			_hexMap->draw();
			_unitView->draw();
			break;
		case 2:
			_hexMap->draw();
			_unitView->draw();
			_unitView->drawActions();
			break;
	}
	
}

void CentralControl::handleEventNormal(const TouchEvent& event) {
	/*SubViewController* selection = 0;
	 
	 if (event.type == 1) {
	 selection = viewControlManager->getTouched(event.point);
	 
	 if (selection != 0) {
	 _selectedViewController = selection;
	 this->switchMode(2);
	 }
	 }*/
	
	if (event.type == 1) {
		if (_unitView->wasTouched(event.point)) {
			_selectedUnit = _unitView;
			this->switchMode(2);
		}
	}
}

void CentralControl::handleEventFocus(const TouchEvent& event) {
	int action = 0;
	
	if (event.type == 3) {
		if ((action = _unitView->touchedAction(event.point)) > -1) {
			_unit->doAction(action);
		}
		this->switchMode(1);
	}
	
}

void CentralControl::touchesBegan(const GPoint& touchPoint) {
	_input->touchesBegan(touchPoint);
}

void CentralControl::touchesMoved(const GPoint& touchPoint) {
	_input->touchesMoved(touchPoint);
}

void CentralControl::touchesEnded(const GPoint& touchPoint) {
	_input->touchesEnded(touchPoint);
}

void CentralControl::touchesCancelled(const GPoint& touchPoint) {
	_input->touchesCancelled(touchPoint);
}


void CentralControl::switchMode(int mode) {
	_mode = mode;
}
