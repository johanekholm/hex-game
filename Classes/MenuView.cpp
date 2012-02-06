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
#include "ControlCallback.h"
#include "ShapeImage.h"
#include "StringImage.h"
#include <string>
#include <iostream>

MenuViewController::~MenuViewController() {
	delete _background;
    delete _rootNode;
}

MenuViewController::MenuViewController() : ViewController(GPointMake(0.0f, 0.0f), 320.0f, 480.0f, MapLayer::ABOVE_GUI) {
    std::vector<BaseMenuNodeVC*> main, games;
    
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, 0.3f), _width, _height, true);
    
    games.push_back(new LeafMenuNodeVC(this, "GAME 1", 4, GPointMake(160.0f, 80.0f), 80.0f, 32.0f));
    games.push_back(new LeafMenuNodeVC(this, "GAME 2", 5, GPointMake(160.0f, 120.0f), 80.0f, 32.0f));
    games.push_back(new LeafMenuNodeVC(this, "GAME 3", 6, GPointMake(160.0f, 160.0f), 80.0f, 32.0f));
    games.push_back(new BackButtonMenuNodeVC(this, "BACK", GPointMake(160.0f, 200.0f), 80.0f, 32.0f));
    
    main.push_back(new LeafMenuNodeVC(this, "NEW", 1, GPointMake(160.0f, 80.0f), 80.0f, 32.0f));
    main.push_back(new ParentMenuNodeVC(this, "OPEN", games, GPointMake(160.0f, 120.0f), 80.0f, 32.0f));
    
    _rootNode = new ParentMenuNodeVC(this, "ROOT", main, GPointMake(0.0f, 0.0f), 80.0f, 32.0f);
    _focus = _rootNode;
    _focus->setFocus(true);    
}

void MenuViewController::draw(const GPoint& cameraPos) {
}

void MenuViewController::drawGUI(const GPoint& cameraPos) {
	_background->drawAt(GPointMake(0.0f, 0.0f));
    _focus->drawGUI(cameraPos);
}

void MenuViewController::goUp() {
    this->setFocus(_focus->getParent());
}

bool MenuViewController::handleEvent(const TouchEvent& event) {
    _focus->handleEvent(event);
    return true;
}

void MenuViewController::reportChoice(int choiceId) {

}

void MenuViewController::setFocus(BaseMenuNodeVC* focus) {
    if (focus != 0) {
        _focus->setFocus(false);
        _focus = focus;
        _focus->setFocus(true);
    }
}

/*---------------------------------------------------------------*/

ChoiceMenuVC::ChoiceMenuVC(ControlCallback& control, std::vector<MenuChoice> choices) : _returnControl(control) {
    std::vector<BaseMenuNodeVC*> nodes;
    int counter = 1;
    GLfloat yStart = 440.0f;
    
    _width = 320.0f;
    _height = 480.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, 0.3f), _width, _height, true);
    
    nodes.push_back(new LeafMenuNodeVC(this, "CANCEL", -1, GPointMake(160.0f, yStart), 120.0f, 25.0f));

    for (std::vector<MenuChoice>::iterator it = choices.begin(); it != choices.end(); ++it) {
        nodes.push_back(new LeafMenuNodeVC(this, (*it).label, (*it).choiceId, GPointMake(160.0f, yStart - counter*30.0f), 120.0f, 25.0f));
        counter++;
    }


    _rootNode = new ParentMenuNodeVC(this, "ROOT", nodes, GPointMake(0.0f, 0.0f), 80.0f, 32.0f);
    _focus = _rootNode;
    _focus->setFocus(true);    
}

void ChoiceMenuVC::reportChoice(int choiceId) {
    _returnControl.callbackNumber(choiceId);
}


/*---------------------------------------------------------------*/

TextboxMenuVC::TextboxMenuVC(ControlCallback& control, const std::string& text, const std::string& buttonLabel) : _returnControl(control) {
    std::vector<BaseMenuNodeVC*> nodes;
    GLfloat yStart = 340.0f;
    
    _width = 320.0f;
    _height = 480.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 0.0f, 0.0f, 0.3f), _width, _height, true);
	
    nodes.push_back(new LeafMenuNodeVC(this, buttonLabel, -1, GPointMake(160.0f, yStart), 120.0f, 25.0f));
    nodes.push_back(new TextMenuNodeVC(this, text, GPointMake(160.0f, yStart - 100.0f), 240.0f, 60.0f));
	
    _rootNode = new ParentMenuNodeVC(this, "ROOT", nodes, GPointMake(0.0f, 0.0f), 80.0f, 32.0f);
    _focus = _rootNode;
    _focus->setFocus(true);    
}

void TextboxMenuVC::reportChoice(int choiceId) {
    _returnControl.callbackVoid();
}


/*---------------------------------------------------------------*/

BaseMenuNodeVC::~BaseMenuNodeVC() {
    delete _button;
    delete _label;
}

BaseMenuNodeVC::BaseMenuNodeVC(MenuViewController* menuVC, const std::string& label, int choiceId, const GPoint& pos, GLfloat width, GLfloat height)  : ViewController(pos, width, height, MapLayer::ABOVE_GUI) {
    _menuVC = menuVC;
    _choiceId = choiceId;
    _parentNode = 0;
}

void BaseMenuNodeVC::draw(const GPoint& cameraPos) {}

void BaseMenuNodeVC::drawGUI(const GPoint& cameraPos) {
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
    
    _button = new RectangleImage(RGBAMake(0.2f, 0.2f, 1.0f, 1.0f), _width, _height, true);
    _label = new StringImage(label, RGBAMakeWhite());
}

bool LeafMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (event.type == 3 && this->isWithin(event.point)) {
        _menuVC->reportChoice(_choiceId);
        return true;
    }
    return false;
}

/*---------------------------------------------------------------*/

BackButtonMenuNodeVC::BackButtonMenuNodeVC(MenuViewController* menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height) : BaseMenuNodeVC(menuVC, label, -1, pos, width, height) {
    
    _button = new RectangleImage(RGBAMake(1.0f, 0.0f, 0.0f, 0.8f), _width, _height, true);
    _label = new StringImage(label, RGBAMakeWhite());
}

bool BackButtonMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (event.type == 3 && this->isWithin(event.point)) {
        _menuVC->goUp();
        return true;
    }
    return false;
}

/*---------------------------------------------------------------*/

TextMenuNodeVC::TextMenuNodeVC(MenuViewController* menuVC, const std::string& label, const GPoint& pos, GLfloat width, GLfloat height) : BaseMenuNodeVC(menuVC, label, -1, pos, width, height) {
    
    _button = new RectangleImage(RGBAMake(0.5f, 0.5f, 0.5f, 1.0f), _width, _height, true);
    _label = new StringImage(label, RGBAMakeWhite());
}

bool TextMenuNodeVC::handleEvent(const TouchEvent& event) {
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
    _label = new StringImage(label, RGBAMakeWhite());
    _subNodes = subNodes;
    
    for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
        (*it)->setParent(this);
    }
}

void ParentMenuNodeVC::drawGUI(const GPoint& cameraPos) {
    if (!_hasFocus) {
        BaseMenuNodeVC::drawGUI(cameraPos);
    } else {
        for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
            (*it)->drawGUI(cameraPos);
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
        for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
            if ((*it)->handleEvent(event)) {
                return true;
            }
        }
    }
    return false;
}

/*---------------------------------------------------------------*/

ActionMenuNodeVC::~ActionMenuNodeVC() {
	
}

ActionMenuNodeVC::ActionMenuNodeVC(MenuActionCallback& action, MenuViewController* menuVC, const std::string& label, const std::vector<BaseMenuNodeVC*>& subNodes, const GPoint& pos, GLfloat width, GLfloat height) : ParentMenuNodeVC(menuVC, label, subNodes, pos, width, height), _action(action) {

}

void ActionMenuNodeVC::buildSubNodes() {
	std::vector<MenuChoice> choices = _action.getChoices();
	int counter = 1;
    GLfloat yStart = 440.0f;
    
    _subNodes.push_back(new LeafMenuNodeVC(_menuVC, "CANCEL", -1, GPointMake(160.0f, yStart), 120.0f, 25.0f));
	
    for (std::vector<MenuChoice>::iterator it = choices.begin(); it != choices.end(); ++it) {
        _subNodes.push_back(new LeafMenuNodeVC(_menuVC, (*it).label, (*it).choiceId, GPointMake(160.0f, yStart - counter*30.0f), 120.0f, 25.0f));
        counter++;
    }
	
	for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
        (*it)->setParent(this);
    }
}

bool ActionMenuNodeVC::handleEvent(const TouchEvent& event) {
    if (!_hasFocus) {
        if (event.type == 3 && this->isWithin(event.point)) {
			if (_action.isInputRequired()) {
				this->buildSubNodes();
				_menuVC->setFocus(this);
				return true;         				
			} else {
				_action.callbackVoid();
			}
        }
        return false;
    } else {
        for (std::vector<BaseMenuNodeVC*>::iterator it = _subNodes.begin(); it != _subNodes.end(); ++it) {
            if ((*it)->handleEvent(event)) {
                return true;
            }
        }
    }
    return false;	
}

/*---------------------------------------------------------------*/


