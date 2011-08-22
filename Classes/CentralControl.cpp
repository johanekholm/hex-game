/*
 *  CentralControl.cpp
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
#include "Sound.h"
#include "ModelManager.h"
#include "StringImage.h"
#include "TextureCatalog.h"
#include "UnitFactory.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "ViewController.h"
#include "ViewControllerManager.h"
#include "SceneLoader.h"
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
        delete _instance->_viewControllerManager;
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

    Sound *sound = Sound::instance();
    sound->add("strike", "slash1");
    sound->add("fire", "fireball1");

    SceneLoader::instance()->loadAdventureScene();
    SceneLoader::instance()->loadBattleScene();
    
    _viewControllerManager = ViewControllerManager::instance();
    _unitFactory = new UnitFactory(_viewControllerManager);
	_input = new InputManager();
    
    //_unitFactory->produceAndRegisterUnit("swordsman", 1, MPointMake(0, 0));
    //_unitFactory->produceAndRegisterUnit("soldier", 2, MPointMake(1, 0));
    //_unitFactory->produceAndRegisterUnit("archer", 2, MPointMake(2, 1));
    //_unitFactory->produceAndRegisterUnit("channeler", 1, MPointMake(0, 1));
    
    SceneLoader::instance()->switchToAdventureScene();
    this->switchMode(ControlMode::ADVENTURE);
}

void CentralControl::update() {
	TouchEvent event;
    int loser = 0;
	
    if (_mode == ControlMode::BATTLE || _mode == ControlMode::BATTLE_FOCUS) {
        if (++_timer >= 200) {
            _timer = 0;
            ModelManager::instance()->tick();
            
            loser = ModelManager::instance()->getOwnerWithNoUnits();
            if (loser != 0) {
                this->switchMode(ControlMode::ADVENTURE);
                SceneLoader::instance()->switchToAdventureScene();
                MessageView::add(GPointMake(160.0f, 240.0f), "VICTORY!");
            }
        }        
    }
    
	if (_input->hasEvent()) {
		
		event = _input->popEvent();
		
		// dispatch event to current event handler
		switch (this->_mode) {
			case ControlMode::ADVENTURE:
				this->handleEventAdventureNormal(event);
				break;

			case ControlMode::BATTLE:
				this->handleEventNormal(event);
				break;
				
			case ControlMode::BATTLE_FOCUS:
				this->handleEventFocus(event);				
                break;
				
		}
	}
    _viewControllerManager->update();
}

void CentralControl::draw() {
	
	switch(_mode) {
		case ControlMode::BATTLE:
		case ControlMode::BATTLE_FOCUS:
        case ControlMode::ADVENTURE:
            _viewControllerManager->drawMap();
			_viewControllerManager->draw();
            _viewControllerManager->drawGUI();
			break;
	}
	
}

void CentralControl::handleEventAdventureNormal(const TouchEvent& event) {
    ViewController* selection = 0;
    ViewController* focus;
    bool caughtEvent = false;
    
    focus = _viewControllerManager->getFocus();
    
    if (focus != 0) {
        caughtEvent = focus->handleEvent(event);
    } 
    
    if (!caughtEvent) {
        switch (event.type) {
            case 1:
                selection = _viewControllerManager->getTouched(event.point);
                std::cout << selection << std::endl;
                _viewControllerManager->setFocus(selection);
                break;

            case 3:                
                break;
                
            default:
                break;
        }
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
