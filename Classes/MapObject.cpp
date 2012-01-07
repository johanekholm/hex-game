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
#include "json.h"
#include <iostream>

MapObject::~MapObject() {
    this->updateObserversDestroyed();
}

MapObject::MapObject() {}

MapObject::MapObject(int category, MPoint pos, int owner, int layer, int visualType, std::vector<int> actionIds, const std::vector<UnitModel*>& members) {
    _category = category;
    _pos = pos;
    _owner = owner;
    _layer = layer;
    _visualType = visualType;
    _memberUnits = members;
    
    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
    
    this->addItem(Item::buildItem(ItemNS::SWORD, 1));
    this->addItem(Item::buildItem(ItemNS::SILVER, 10));
}

Json::Value MapObject::serialize() {
    Json::Value root;
    
    Json::Value& actions = root["actions"];
    root["category"] = _category;
    root["owner"] = _owner;
    root["layer"] = _layer;
    root["visualType"] = _visualType;
    root["id"] = _id;
    root["x"] = _pos.x;
    root["y"] = _pos.y;
    
    // serialize actions
    for (std::map<int, AdventureAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        actions.append(it->first);
    }
    
    return root;
}

void MapObject::deserialize(Json::Value& root) {
    _category = root.get("category", 0).asInt();
    _owner = root.get("owner", 0).asInt();
    _layer = root.get("layer", 0).asInt();
    _visualType = root.get("visualType", 0).asInt();
    _id = root.get("id", 0).asInt();
    _pos.x = root.get("x", 0.0f).asFloat();
    _pos.y = root.get("y", 0.0f).asFloat();
    
    // deserialize actions
    for (Json::ValueIterator it = root["actions"].begin(); it != root["actions"].end(); it++) {
        this->addAction((*it).asInt());
    }
}


AdventureAction* MapObject::addAction(int action) {
	_actions[action] = AdventureAction::build(action, this);
	return _actions[action];
}

bool MapObject::canMoveTo(const MPoint& pos) {
    int terrain = ModelManager::instance()->getMap()->getHexValue(pos);
    
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
    
    hexes = ModelManager::instance()->getMap()->getAllHexes();
	//objects = ModelManager::instance()->getAllUnits();
    
	for (std::map<int, AdventureAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        
        temp = (it->second)->getActionPoints(0, hexes, objects);
        actionPoints.insert(actionPoints.end(), temp.begin(), temp.end());
    }
    
	return actionPoints;
}

int MapObject::getLayer() {
    return _layer;
}

std::vector<UnitModel*> MapObject::getMembers() {
    return _memberUnits;
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

int MapObject::getVisualType() {
    return _visualType;
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



