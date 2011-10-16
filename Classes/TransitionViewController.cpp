/*
 *  TransitionViewController.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "TransitionViewController.h"
#include "CentralControl.h"
#include "ShapeImage.h"
#include "ViewControllerManager.h"

TransitionViewController::~TransitionViewController() {
	delete _background;
}

TransitionViewController::TransitionViewController() : ViewController(GPointMake(0.0f, 0.0f), 320.0f, 480.0f, MapLayer::ABOVE_GUI) {
    
    _alpha = 0.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, _alpha), _width, _height, true);
    _counter = 0;
}

void TransitionViewController::draw(const GPoint& cameraPos) {
}

void TransitionViewController::drawGUI(const GPoint& cameraPos) {
	_background->drawAt(GPointMake(0.0f, 0.0f));
}

bool TransitionViewController::handleEvent(const TouchEvent& event) {
    return true;
}

void TransitionViewController::update() {
    _alpha += 0.04f;
        
    if (_alpha > 1.0f) {
        _alpha = 1.0f;
        if (++_counter > 30) {
            ViewControllerManager::instance()->removeSoft(this);
            CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
            return;            
        }
    }
    _background->setColor(RGBAMake(0.0f, 0.0f, 0.0f, _alpha));
}

/*---------------------------------------------------------------*/

