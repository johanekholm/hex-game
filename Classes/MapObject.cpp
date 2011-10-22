//
//  MapObject.cpp
//  hex-game
//
//  Created by Johan Ekholm
//

#include "MapObject.h"
#include "Action.h"
#include "ModelManager.h"
#include "HexMapModel.h"
#include "UnitModel.h"
#include "Item.h"
#include <iostream>

MapObject::~MapObject() {
    this->updateObserversDestroyed();
}

MapObject::MapObject(int category, MPoint pos, int owner, std::vector<int> actionIds) {
    _category = category;
    _pos = pos;
    _owner = owner;
    
    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
    
    this->addItem(Item::buildItem(ItemNS::SWORD, 1));
    this->addItem(Item::buildItem(ItemNS::SILVER, 10));
}

AdventureAction* MapObject::addAction(int action) {
	_actions[action] = AdventureAction::build(action, this);
	return _actions[action];
}

bool MapObject::canMoveTo(const MPoint& pos) {
    int terrain = ModelManager::instance()->getAdventureMap()->getHexValue(pos);
    
    return (terrain < 2);
}

void MapObject::doAction(const ActionState& statePoint) {
	if (_actions.find(statePoint.actionId) != _actions.end()) {
		_actions[statePoint.actionId]->doIt(statePoint);
	}    
}

std::vector<ActionState> MapObject::getActions() {
	std::vector<ActionState> actionPoints, temp;
    std::map<int, HexState> hexes;
    std::vector<MapObject*> objects;
    std::vector<MPoint> targets;
    
    hexes = ModelManager::instance()->getAdventureMap()->getAllHexes();
	//objects = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, AdventureAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(0, hexes, objects);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

int MapObject::getOwner() {
    return _owner;
}

MPoint MapObject::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

MapObjectState MapObject::getState() {
    MapObjectState state;
    std::vector<ActionState> actions;
    
    state.pos = _pos;
    state.actions = this->getActions();
    
    return state;
}

bool MapObject::matchesCategory(int category) {
    return (_category == category);
}

void MapObject::move(const MPoint& targetPos) {
    if (this->canMoveTo(targetPos)) {
        _pos = targetPos;        
        this->updateObservers();        
    }
}

void MapObject::setId(int objectId) {
    _id = objectId;
}


/*---------------------------------------------------------------*/


PartyModel::~PartyModel() {
    
}

PartyModel::PartyModel(int category, MPoint pos, int owner, std::vector<int> actionIds, const std::vector<UnitModel*>& members) : MapObject(category, pos, owner, actionIds) {
    _memberUnits = members;
}

std::vector<UnitModel*> PartyModel::getMembers() {
    return _memberUnits;
}




