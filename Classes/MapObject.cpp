//
//  MapObject.cpp
//  hex-game
//
//  Created by Johan Ekholm
//

#include "MapObject.h"
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

MapObjectState MapObject::getState() {
    MapObjectState state;
    std::vector<ActionState> actions;
    
    state.pos = _pos;
    this->availableActions(state.actions);
    
    return state;
}

void MapObject::setId(int objectId) {
    _id = objectId;
}




PartyModel::~PartyModel() {
    
}

PartyModel::PartyModel(MPoint pos, int allegiance, const std::vector<UnitModel*>& members) : MapObject(pos, allegiance) {
    _members = members;
}
