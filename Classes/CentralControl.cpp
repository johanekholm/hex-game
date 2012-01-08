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
#include "ObjectBuilder.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "ViewController.h"
#include "ViewControllerManager.h"
#include "SceneLoader.h"
#include "StateManager.h"
#include "toolkit.h"
#include "geometry.h"

#include <cstdlib>
#include <iostream>
#include <ctime>



CentralControl* CentralControl::_instance = 0;

CentralControl* CentralControl::instance() {
    if (_instance == 0) {
        _instance = new CentralControl();
    } 
    return _instance;
}

void CentralControl::destroy() {
	if (_instance != 0) {
        
        delete _instance->_stringImage;
        delete _instance->_input;
        delete _instance->_viewControllerManager;
        ModelManager::destroy();
        delete _instance->_objectBuilder;
        
        
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
    sound->add("music1", "grotta");

    SceneLoader::instance()->loadAdventureScene();
    
    _viewControllerManager = ViewControllerManager::instance();
    _objectBuilder = new ObjectBuilder(_viewControllerManager);
	_input = new InputManager();
    
    this->switchMode(ControlMode::ADVENTURE);
}

void CentralControl::update() {
	TouchEvent event;
    int loser = 0;
	
    if (_mode == ControlMode::BATTLE || _mode == ControlMode::BATTLE_FOCUS) {
        if (++_timer >= 30) {
            _timer = 0;
            ModelManager::instance()->tick();
            
            /*loser = ModelManager::instance()->getOwnerWithNoUnits();
            if (loser != 0) {
                this->switchMode(ControlMode::ADVENTURE);
                SceneLoader::instance()->switchToAdventureScene();
                MessageView::add(GPointMake(160.0f, 240.0f), "VICTORY!");
            }*/
        }        
    }
    
	if (_input->hasEvent()) {
		
		event = _input->popEvent();
		event.translatedPoint = _viewControllerManager->adjustForCamera(event.point);
        
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

			case ControlMode::MENU:
				this->handleEventMenu(event);				
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
        case ControlMode::MENU:
            
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
    GPoint anchor;
    
    focus = _viewControllerManager->getFocus();
    
    if (focus != 0) {
        caughtEvent = focus->handleEvent(event);
    } 
    
    if (!caughtEvent) {
        switch (event.type) {
            case 1:
                break;

            case 2:
                anchor = event.previousPoint;
                _viewControllerManager->moveCamera(anchor - event.point);
                break;

            case 3:
                selection = _viewControllerManager->getTouched(event.translatedPoint);
                _viewControllerManager->setFocus(selection);
                break;
                
            default:
                break;
        }
    }
}

void CentralControl::handleEventMenu(const TouchEvent& event) {
    ViewController* focus;
    
    focus = _viewControllerManager->getFocus();
    
    if (focus != 0) {
        focus->handleEvent(event);
    }
}

void CentralControl::handleEventNormal(const TouchEvent& event) {
    ViewController* selection = 0;
	 
	if (event.type == 1) {
        selection = _viewControllerManager->getTouched(event.translatedPoint);
	 
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
