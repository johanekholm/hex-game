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


class CentralControl::PrivateData {
public:
	static CentralControl* instance;
	int mode;
	int timer;
	InputManager* input;
	ModelManager* modelManager;
	ViewControllerManager* viewControllerManager;
	ObjectBuilder* objectBuilder;
	StringImage* stringImage;
	bool turnEnded;
};

CentralControl* CentralControl::PrivateData::instance = 0;

CentralControl* CentralControl::instance() {
	if (PrivateData::instance == 0) {
		PrivateData::instance = new CentralControl();
		PrivateData::instance->init();
	}
	return PrivateData::instance;
}

void CentralControl::destroy() {
	if (PrivateData::instance != 0) {
		delete PrivateData::instance->d->stringImage;
		delete PrivateData::instance->d->input;
		delete PrivateData::instance->d->viewControllerManager;
		ModelManager::destroy();
		delete PrivateData::instance->d->objectBuilder;
		delete PrivateData::instance;
		PrivateData::instance=0;
	}
}

CentralControl::CentralControl() {
    d = new PrivateData();
	d->mode = 1;
	d->timer = 200;
}

void CentralControl::draw() {
	switch(d->mode) {
		case ControlMode::BATTLE:
		case ControlMode::BATTLE_FOCUS:
		case ControlMode::ADVENTURE:
		case ControlMode::MENU:
			d->viewControllerManager->drawMap();
			d->viewControllerManager->draw();
			d->viewControllerManager->drawGUI();
			break;
	}
}

void CentralControl::handleEventAdventureNormal(const TouchEvent& event) {
	ViewController* selection = 0;
	ViewController* focus;
	bool caughtEvent = false;
	GPoint anchor;
    
	focus = d->viewControllerManager->getFocus();
    
	if (focus != 0) {
	    caughtEvent = focus->handleEvent(event);
	}
    
	if (!caughtEvent) {
		switch (event.type) {
			case 1:
				break;

			case 2:
				anchor = event.previousPoint;
				d->viewControllerManager->moveCamera(anchor - event.point);
				break;

			case 3:
				selection = d->viewControllerManager->getTouched(event.translatedPoint);
				d->viewControllerManager->setFocus(selection);
				break;
                
			default:
				break;
		}
	}
}

void CentralControl::handleEventMenu(const TouchEvent& event) {
	ViewController* focus;

	focus = d->viewControllerManager->getFocus();

	if (focus != 0) {
		focus->handleEvent(event);
	}
}

void CentralControl::handleEventNormal(const TouchEvent& event) {
	ViewController* selection = 0;
	 
	if (event.type == 1) {
		selection = d->viewControllerManager->getTouched(event.translatedPoint);
		if (selection != 0) {
			d->viewControllerManager->setFocus(selection);
			this->switchMode(2);
		}
	}
}

void CentralControl::handleEventFocus(const TouchEvent& event) {
	ViewController* focus;

	focus = d->viewControllerManager->getFocus();

	if (focus != 0) {
		focus->handleEvent(event);
	}
	
	if (event.type == 3 && d->mode == ControlMode::BATTLE_FOCUS) {
	    d->viewControllerManager->setFocus(0);
		this->switchMode(ControlMode::BATTLE);
	}	
}

void CentralControl::init() {
	srand (time(NULL));

	Sound *sound = Sound::instance();
	sound->add("strike", "slash1");
	sound->add("fire", "fireball1");
	sound->add("music1", "grotta");

	SceneLoader::instance()->loadAdventureScene();

	d->viewControllerManager = ViewControllerManager::instance();
	d->objectBuilder = new ObjectBuilder(d->viewControllerManager);
	d->input = new InputManager();

	this->switchMode(ControlMode::ADVENTURE);
}

void CentralControl::touchesBegan(const GPoint& touchPoint) {
	d->input->touchesBegan(touchPoint);
}

void CentralControl::touchesMoved(const GPoint& touchPoint) {
	d->input->touchesMoved(touchPoint);
}

void CentralControl::touchesEnded(const GPoint& touchPoint) {
	d->input->touchesEnded(touchPoint);
}

void CentralControl::touchesCancelled(const GPoint& touchPoint) {
	d->input->touchesCancelled(touchPoint);
}

void CentralControl::switchMode(int mode) {
	d->mode = mode;
}

void CentralControl::update() {
	TouchEvent event;
	int loser = 0;

	if (d->mode == ControlMode::BATTLE || d->mode == ControlMode::BATTLE_FOCUS) {
		if (++d->timer >= 30) {
			d->timer = 0;
			ModelManager::instance()->tick();

			/*loser = ModelManager::instance()->getOwnerWithNoUnits();
			if (loser != 0) {
				this->switchMode(ControlMode::ADVENTURE);
				SceneLoader::instance()->switchToAdventureScene();
				MessageView::add(GPointMake(160.0f, 240.0f), "VICTORY!");
			}*/
		}
	}

	if (d->input->hasEvent()) {
		event = d->input->popEvent();
		event.translatedPoint = d->viewControllerManager->adjustForCamera(event.point);

		// dispatch event to current event handler
		switch (d->mode) {
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
	d->viewControllerManager->update();
}

void CentralControl::nextTurn() {
	d->turnEnded = true;
}
