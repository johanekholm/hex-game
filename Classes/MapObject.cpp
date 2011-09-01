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
#include <iostream>

MapObject::~MapObject() {

}

MapObject::MapObject(MPoint pos, int allegiance) {
    _pos = pos;
    _allegiance = allegiance;
}

void MapObject::availableActions(std::vector<ActionState>& actions) {
    
    return;
}

void MapObject::doAction(const ActionState& statePoint) {

}


MapObjectState MapObject::getState() {
    MapObjectState state;
    std::vector<ActionState> actions;
    
    state.pos = _pos;
    //this->availableActions(state.actions);
    
    return state;
}

void MapObject::setId(int objectId) {
    _id = objectId;
}




PartyModel::~PartyModel() {
    
}

PartyModel::PartyModel(MPoint pos, int allegiance, std::vector<int> actionIds, const std::vector<UnitModel*>& members) : MapObject(pos, allegiance) {
    _members = members;
    
    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
}

void PartyModel::availableActions(std::vector<ActionState>& actions) {
    //actions = Action::getAdventureActions(this);
    return;
}

AdventureAction* PartyModel::addAction(int action) {
	_actions[action] = AdventureAction::build(action, this);
	return _actions[action];
}

void PartyModel::doAction(const ActionState& statePoint) {
	if (_actions.find(statePoint.actionId) != _actions.end()) {
		_actions[statePoint.actionId]->doIt(statePoint);
	}    
}

std::vector<ActionState> PartyModel::getActions() {
	std::vector<ActionState> actionPoints, temp;
    std::map<int, HexState> hexes;
    std::vector<PartyModel*> parties;
    std::vector<MPoint> targets;
        
    hexes = ModelManager::instance()->getAdventureMap()->getAllHexes();
	//objects = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, AdventureAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(0, hexes, parties);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

MPoint PartyModel::getPosition() {
	return MPointMake(_pos.x, _pos.y);
}

MapObjectState PartyModel::getState() {
    MapObjectState state;
    
    state.pos = _pos;
    state.actions = this->getActions();
    
    return state;
}

void PartyModel::move(const MPoint& targetPos) {
     _pos = targetPos;        
	
	this->updateObservers();
}



