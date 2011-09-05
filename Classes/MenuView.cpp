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


MenuView::~MenuView() {
	delete _background;
    _menuItems.clear();
    // TO-DO: implement delete of vectors pointers objects
	_action = 0;
}

MenuView::MenuView(AdventureAction* action, std::vector<MenuChoice> choices) {
	_action = action;
    _width = 320.0f;
    _height = 480.0f;
    _background = new RectangleImage(RGBAMake(0.0f, 8.0f, 0.0f, 0.3f), _width, _height, true);
}

void MenuView::draw() {
}

void MenuView::drawMenuItems() {
	for (std::vector<MenuItemView*>::iterator it = _menuItems.begin(); it != _menuItems.end(); ++it) {
		(*it)->draw();
    }
}

void MenuView::drawGUI() {
	_background->drawAt(GPointMake(0.0f, 0.0f));
    this->drawMenuItems();
}

MenuItemView* MenuView::getTouchedMenuItem(const GPoint& point) {
	
	for (std::vector<MenuItemView*>::iterator it = _menuItems.begin(); it != _menuItems.end(); ++it) {		
		/*if ((*it)->isWithin(point)) {
			return (*it);
		}*/
	}
	return 0;
}

bool MenuView::handleEvent(const TouchEvent& event) {
    MenuItemView* menuItem;
    
	if (event.type == 3) {
        menuItem = this->getTouchedMenuItem(event.point);
        
        if (menuItem != 0) {
            //_action->deliverChoice(menuItem->getChoiceId());
        }
	}
    
	return true;
}

