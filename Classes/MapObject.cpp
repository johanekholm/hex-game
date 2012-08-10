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
#include <stdlib.h>

MapObject::~MapObject() {
    this->updateObserversDestroyed();
}

MapObject::MapObject() {}

MapObject::MapObject(int category, MPoint pos, int owner, int layer, int visualType, std::vector<int> actionIds, const std::vector<UnitModel*>& members) {
	_id = 0;
    _category = category;
    _pos = pos;
    _owner = owner;
    _layer = layer;
    _visualType = visualType;
    _memberUnits = members;
    
    for (std::vector<int>::iterator it = actionIds.begin(); it != actionIds.end(); ++it) {
        this->addAction(*it);
    }
    
    this->addItem(new Item(ItemNS::SWORD, 1));
    this->addItem(new Item(ItemNS::SILVER, 10));
}

Json::Value MapObject::serialize() {
    Json::Value root;
    
    Json::Value& actions = root["actions"];
    Json::Value& members = root["members"];
    Json::Value& items = root["items"];
    root["category"] = _category;
    root["owner"] = _owner;
    root["layer"] = _layer;
    root["visualType"] = _visualType;
    root["id"] = _id;
    root["x"] = _pos.x;
    root["y"] = _pos.y;
    root["text"] = _text;
    
    // serialize actions
    for (std::map<int, AdventureAction*>::iterator it = _actions.begin(); it != _actions.end(); ++it) {
        actions.append(it->first);
    }
    
    // serialize members
    for (std::vector<UnitModel*>::iterator it = _memberUnits.begin(); it != _memberUnits.end(); ++it) {
        members.append((*it)->serialize());
    }
	
	// serialize items
	items = this->serializeItems();
    
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
	_text = root.get("text", "no_text").asString();
    
    // deserialize actions
    for (Json::ValueIterator it = root["actions"].begin(); it != root["actions"].end(); it++) {
        this->addAction((*it).asInt());
    }
    
    // deserialize members
    for (Json::ValueIterator it = root["members"].begin(); it != root["members"].end(); it++) {
        UnitModel* unit = new UnitModel();
        unit->deserialize(*it);
        _memberUnits.push_back(unit);
    }
	
	// deserialize items
	this->deserializeItems(root["items"]);

}


AdventureAction* MapObject::addAction(int action) {
	_actions[action] = AdventureAction::build(action, this);
	return _actions[action];
}

UnitModel* MapObject::addMember(UnitModel* unit) {
	_memberUnits.push_back(unit);
	return unit;
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

void MapObject::doAI() {
    bool hasOffensiveAction = false;
    bool hasMovement = false;
    std::vector<ActionState> actionPoints, offensives, movements;
	
    if (_owner == 2 && _category == MapObjectCategory::PARTY) {
        actionPoints = this->getActions();
        
        for (std::vector<ActionState>::iterator it = actionPoints.begin(); it != actionPoints.end(); ++it) {
            switch ((*it).actionType) {
                case ActionNS::TYPE_ATTACK:
                    hasOffensiveAction = true;
                    if ((*it).active) {
                        offensives.push_back(*it);                        
                    }
                    break;
                    
                case ActionNS::TYPE_MOVEMENT:
                    hasMovement = true;
                    movements.push_back(*it);
                    break;
                    
                default:
                    break;
            }
        }
        
        if (hasOffensiveAction) {
            
            // choose offensive action randomly
            if (offensives.size() > 0) {
                this->doAction(offensives.at(rand() % offensives.size()));
            } else {
                // unit has to wait until offensive action becomes active, do nothing this tick
            }
        } else {
            // move about randomly
            if (movements.size() > 0) {
                this->doAction(movements.at(rand() % movements.size()));
            }
        }
    }
}

void MapObject::doTurn() {
	this->doAI();
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

int MapObject::getId() {
    return _id;
}

int MapObject::getLayer() {
    return _layer;
}

UnitModel* MapObject::getMember(int unitId) {
	for (std::vector<UnitModel*>::iterator it = _memberUnits.begin(); it != _memberUnits.end(); ++it) {
        if (*it != 0 && (*it)->getId() == unitId) {
			return *it;
		}
    }
	return 0;
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
	state.owner = _owner;
    state.actions = this->getActions();
    
    return state;
}

std::string MapObject::getText() {
	return _text;
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

std::vector<UnitModel*> MapObject::removeMembers() {
    std::vector<UnitModel*> copy;
	
	copy.assign(_memberUnits.begin(), _memberUnits.end());
    _memberUnits.clear();
    
    return copy;
}

void MapObject::setId(int objectId) {
    _id = objectId;
}



