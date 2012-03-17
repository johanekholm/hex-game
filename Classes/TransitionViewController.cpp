/*
 *  TransitionViewController.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "TransitionViewController.h"
#include "CentralControl.h"
#include "ControlCallback.h"
#include "ShapeImage.h"
#include "StringImage.h"
#include "ViewControllerManager.h"

TransitionViewController::~TransitionViewController() {
}

TransitionViewController::TransitionViewController(ControlCallback& control) : ViewController(GPointMake(0.0f, 0.0f), 320.0f, 480.0f, MapLayer::ABOVE_GUI), _returnControl(control) {
    _counter = 0;
}

bool TransitionViewController::handleEvent(const TouchEvent& event) {
    return true;
}

/*---------------------------------------------------------------*/

FadeOutTransition::~FadeOutTransition() {
	delete _background;
}

FadeOutTransition::FadeOutTransition(ControlCallback& control) : TransitionViewController(control) {
    _alpha = 0.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, _alpha), _width, _height, true);
}

void FadeOutTransition::draw(const GPoint& cameraPos) {
}

void FadeOutTransition::drawGUI(const GPoint& cameraPos) {
	_background->drawAt(GPointMake(0.0f, 0.0f));
}

void FadeOutTransition::update() {
    _alpha += 0.04f;
        
    if (_alpha > 1.0f) {
        _alpha = 1.0f;
        if (++_counter > 30) {
            ViewControllerManager::instance()->removeSoft(this);
            //CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
            _returnControl.callbackVoid();
            return;            
        }
    }
    _background->setColor(RGBAMake(0.0f, 0.0f, 0.0f, _alpha));
}

/*---------------------------------------------------------------*/

VictoryTransition::~VictoryTransition() {
    delete _background;
	delete _message;
}

VictoryTransition::VictoryTransition(ControlCallback& control) : TransitionViewController(control) {
    
    _alpha = 0.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, _alpha), _width, _height, true);
    _counter = 0;
    _message = new StringImage("VICTORY", RGBAMakeWhite());
}

void VictoryTransition::draw(const GPoint& cameraPos) {
}

void VictoryTransition::drawGUI(const GPoint& cameraPos) {
	_background->drawAt(GPointMake(0.0f, 0.0f));
    _message->drawCenteredAt(GPointMake(160.0f, 240.0f));
}

bool VictoryTransition::handleEvent(const TouchEvent& event) {
    return true;
}

void VictoryTransition::update() {
    _alpha += 0.04f;
    
    if (_alpha > 1.0f) {
        _alpha = 1.0f;
        if (++_counter > 60) {
            ViewControllerManager::instance()->removeSoft(this);
            CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
            _returnControl.callbackVoid();
            return;            
        }
    }
    _background->setColor(RGBAMake(0.0f, 0.0f, 0.0f, _alpha));
}

/*---------------------------------------------------------------*/

