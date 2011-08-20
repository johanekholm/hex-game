/*
 *  CentralControl.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CentralControl.h"

#include "HexMap.h"
#include "HexMapModel.h"
#include "InputManager.h"
#include "MessageView.h"
#include "ModelManager.h"
#include "StringImage.h"
#include "TextureCatalog.h"
#include "UnitFactory.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "ViewController.h"
#include "ViewControllerManager.h"
#include "toolkit.h"
#include "geometry.h"

#include <cstdlib>
#include <iostream>
#include <ctime>



CentralControl* CentralControl::_instance = 0;

void CentralControl::destroy() {
	if (_instance != 0) {
        
        delete _instance->_stringImage;
        delete _instance->_input;
        delete _instance->_hexMap;
        delete _instance->_viewControllerManager;
        //delete _instance->_modelManager;
        ModelManager::destroy();
        delete _instance->_unitFactory;
        
        
		delete _instance;
		_instance=0;
	}
}

CentralControl::CentralControl() {
    _mode = 1;
	_timer = 200;
    
    srand (time(NULL));

	TextureCatalog* catalog = TextureCatalog::instance();
	
    ModelManager::instance()->setBattleMap(new HexMapModel(4, 4));
    
	_hexMap = new HexMap(ModelManager::instance()->getBattleMap(), catalog->get("hexTiles"), 1.0f);
    _viewControllerManager = ViewControllerManager::instance();
    _unitFactory = new UnitFactory(_viewControllerManager);
	_input = new InputManager();
    
    _unitFactory->produceAndRegisterUnit("swordsman", 1, MPointMake(0, 0));
    _unitFactory->produceAndRegisterUnit("soldier", 2, MPointMake(2, 0));
    _unitFactory->produceAndRegisterUnit("archer", 2, MPointMake(2, 1));
    _unitFactory->produceAndRegisterUnit("channeler", 1, MPointMake(0, 1));
    
    _stringImage = new StringImage("HIJKLMNOPQRSTUVWXYZ", 1.0f, 1.0f, 1.0f, 1.0f);
    
}

void CentralControl::update() {
	TouchEvent event;
	
    if (++_timer >= 300) {
        _timer = 0;
        ModelManager::instance()->tick();
    }
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
    _viewControllerManager->update();
}

void CentralControl::draw() {
	
	switch(_mode) {
		case 1:
			_hexMap->draw();
			_viewControllerManager->draw();
            _viewControllerManager->drawGUI();
            //_stringImage->drawAt(GPointMake(20.0f, 80.0f));
			break;
		case 2:
			_hexMap->draw();
			_viewControllerManager->draw();
            _viewControllerManager->drawGUI();
			break;
	}
	
}

void CentralControl::handleEventNormal(const TouchEvent& event) {
    ViewController* selection = 0;
	 
	if (event.type == 1) {
        selection = _viewControllerManager->getTouched(event.point);
	 
        if (selection != 0) {
            _viewControllerManager->setFocus(selection);
            this->switchMode(2);
        }
    }
}

void CentralControl::handleEventFocus(const TouchEvent& event) {
    ViewController* focus;
    
    focus = _viewControllerManager->getFocus();
    
    if (focus != 0) {
        focus->handleEvent(event);
    }
	
	if (event.type == 3) {
        _viewControllerManager->setFocus(0);
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
