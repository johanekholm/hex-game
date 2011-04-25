/*
 *  UnitModel.mm
 *  igame
 *
 *  Created by Johan Ekholm on 2010-08-26.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "UnitModel.h"
//#include "IUnitView.h"
#include "Action.h"
#include "geometry.h"

UnitModel::~UnitModel() {
	//delete _actions;
}

UnitModel::UnitModel(int x, int y) {
	_pos.x = x;
	_pos.y = y;
	_direction = GEOM_DIR_E;
	addAction(0);
	addAction(1);
	addAction(2);
	addAction(3);
}


/*
void UnitModel::registerAction(Action *aAction) {
	actions->add(aAction->register(this));
	
}

*/


Action* UnitModel::addAction(int action) {
	_actions[action] = new Action(action, this);
	return _actions[action];
}

void UnitModel::doAction(int action) {
	if (_actions.find(action) != _actions.end()) {
		_actions[action]->doIt();
	}
	
	
}

bool UnitModel::spendAP(int cost) {
	if (_ap >= cost) {
		_ap -= cost;
		return true;
	} else {
		return false;
	}
}

MPoint UnitModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

int UnitModel::getDirection() {
	return _direction;
}

std::vector<int> UnitModel::getActions() {
	vector<int> v;
	
	for(std::map<int, Action*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        v.push_back(it->first);
    }
    
	return v;
}


void UnitModel::move(int distance) {

	MPoint v = getHexVector(_direction, _pos);
	_pos += v;
	
	//NSLog(@"moved - x: %i, y: %i", v.x, v.y);
	
	this->updateObservers();
}

void UnitModel::rotate(int rotation) {
	_direction += rotation;
	
	if (_direction < 1) {
		_direction += 6;
	} else if (_direction > 6) {
		_direction -= 6;
	}
	
	this->updateObservers();
	
	//NSLog(@"direction: %i", _direction);
}

void UnitModel::strike() {
	
}

/*void UnitModel::registerView(IUnitView* view) {
	_view = view;
	this->updateViews();
}*/


/*void UnitModel::updateViews() {
	_view->updatePosition(_pos, _direction);
}*/

