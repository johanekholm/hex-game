/*
 *  MenuView.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "MenuView.h"
#include "geometry.h"
#include "InputManager.h"
#include "ViewControllerManager.h"
#include "toolkit.h"
#include "Action.h"
#include "ShapeImage.h"
#include "StringImage.h"
#include <string>
#include <iostream>

MenuViewController::~MenuViewController() {
	delete _background;
    delete _rootNode;
	_action = 0;
}

MenuViewController::MenuViewController() {
    std::vector<BaseMenuNodeVC*> main, games;
    
    _width = 320.0f;
    _height = 480.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, 0.3f), _width, _height, true);
    
    games.push_back(new LeafMenuNodeVC(this, "GAME 1", 4, GPointMake(160.0f, 80.0f), 80.0f, 32.0f));
    games.push_back(new LeafMenuNodeVC(this, "GAME 2", 5, GPointMake(160.0f, 120.0f), 80.0f, 32.0f));
    games.push_back(new LeafMenuNodeVC(this, "GAME 3", 6, GPointMake(160.0f, 160.0f), 80.0f, 32.0f));
    games.push_back(new BackButtonMenuNodeVC(this, "BACK", GPointMake(160.0f, 200.0f), 80.0f, 32.0f));
    
    main.push_back(new LeafMenuNodeVC(this, "NEW", 1, GPointMake(160.0f, 80.0f), 80.0f, 32.0f));
    main.push_back(new ParentMenuNodeVC(this, "OPEN", games, GPointMake(160.0f, 120.0f), 80.0f, 32.0f));
    main.push_back(new LeafMenuNodeVC(this, "QUIT", 3, GPointMake(160.0f, 160.0f), 80.0f, 32.0f));
    
    _rootNode = new ParentMenuNodeVC(this, "ROOT", main, GPointMake(0.0f, 0.0f), 80.0f, 32.0f);
    _focus = _rootNode;
    _focus->setFocus(true);    
}

MenuViewController::MenuViewController(AdventureAction* action, std::vector<MenuChoice> choices) {
}

void MenuViewController::draw() {
}

void MenuViewController::drawGUI() {
	_background->drawAt(GPointMake(0.0f, 0.0f));
    _focus->drawGUI();
}

void MenuViewController::goUp() {
    this->setFocus(_focus->getParent());
}

bool MenuViewController::handleEvent(const TouchEvent& event) {
    _focus->handleEvent(event);
    return true;
}

void MenuViewController::reportChoice(int choiceId) {
    std::cout << "Reporting Choice: " << choiceId << std::endl;
}

void MenuViewController::setFocus(BaseMenuNodeVC* focus) {
    if (focus != 0) {
        _focus->setFocus(false);
        _focus = focus;
        _focus->setFocus(true);
    }
}

/*---------------------------------------------------------------*/

BaseMenuNodeVC::~BaseMenuNodeVC() {
    delete _button;
    delete _label;
}

BaseMenuNodeVC::BaseMenuNodeVC(MenuViewController* menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height) {
    _menuVC = menuVC;
    _pos = pos;
    _width = width;
    _height = height;
    _choiceId = choiceId;
    _parentNode = 0;
}

void BaseMenuNodeVC::draw() {}

void BaseMenuNodeVC::drawGUI() {
    _button->drawCenteredAt(_pos);
    _label->drawCenteredAt(_pos);
}

BaseMenuNodeVC* BaseMenuNodeVC::getParent() {
    return _parentNode;
}

void BaseMenuNodeVC::setMenu(MenuViewController* menuVC) {
    _menuVC = menuVC;
}

void BaseMenuNodeVC::setParent(BaseMenuNodeVC* parent) {
    _parentNode = parent;
}

/*---------------------------------------------------------------*/

LeafMenuNodeVC::~LeafMenuNodeVC() {
    delete _button;
    delete _label;
}

LeafMenuNodeVC::LeafMenuNodeVC(MenuViewController* menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height) : BaseMenuNodeVC(menuVC, label, choiceId, pos, width, height) {
    
    _button = new RectangleImage(RGBAMake(0.2f, 0.2f, 1.0f, 0.8f), _width, _height, true);
    _label = new StringImage(label, 1.0f, 1.0f, 1.0f, 1.0f);
}

bool LeafMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (event.type == 3 && this->isWithin(event.point)) {
        std::cout << _menuVC << std::endl;
        _menuVC->reportChoice(_choiceId);
        return true;
    }
    return false;
}

/*---------------------------------------------------------------*/

BackButtonMenuNodeVC::BackButtonMenuNodeVC(MenuViewController* menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height) : BaseMenuNodeVC(menuVC, label, -1, pos, width, height) {
    
    _button = new RectangleImage(RGBAMake(1.0f, 0.0f, 0.0f, 0.8f), _width, _height, true);
    _label = new StringImage(label, 1.0f, 1.0f, 1.0f, 1.0f);
}

bool BackButtonMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (event.type == 3 && this->isWithin(event.point)) {
        std::cout << _menuVC << std::endl;
        _menuVC->goUp();
        return true;
    }
    return false;
}

/*---------------------------------------------------------------*/

ParentMenuNodeVC::~ParentMenuNodeVC() {
    delete _button;
    delete _label;
    for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
        delete (*it);
    }
    _subNodes.clear();
}

ParentMenuNodeVC::ParentMenuNodeVC(MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height) : BaseMenuNodeVC(menuVC, label, -1, pos, width, height){

    _button = new RectangleImage(RGBAMake(0.2f, 0.2f, 1.0f, 0.8f), _width, _height, true);
    _label = new StringImage(label, 1.0f, 1.0f, 1.0f, 1.0f);
    _subNodes = subNodes;
    
    for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
        (*it)->setParent(this);
    }
}

void ParentMenuNodeVC::drawGUI() {
    if (!_hasFocus) {
        BaseMenuNodeVC::drawGUI();
    } else {
        for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
            (*it)->drawGUI();
        }
    }
}

bool ParentMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (!_hasFocus) {
        if (event.type == 3 && this->isWithin(event.point)) {
            _menuVC->setFocus(this);
            return true;         
        }
        return false;
    } else {
        std::cout << "Event to subnode " << event.point.x << "," << event.point.y << std::endl;
        for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
            if ((*it)->handleEvent(event)) {
                return true;
            }
        }
    }
    return false;
}


