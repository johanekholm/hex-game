/*
 *  CentralControl.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CentralControl.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "HexMap.h"
#include "InputManager.h"

CentralControl* CentralControl::_instance = 0;

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
			//NSLog(@"unit selected");
		}
	}
}

void CentralControl::handleEventFocus(const TouchEvent& event) {
	int action = 0;
	
	if (event.type == 3) {
		if ((action = _unitView->touchedAction(event.point)) > -1) {
			//NSLog(@"action: %i", action);
			
			_unit->doAction(action);
		}
		this->switchMode(1);
	}
	
}

void CentralControl::switchMode(int mode) {
	_mode = mode;
}