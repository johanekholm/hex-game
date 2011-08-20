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

void MapObject::setId(int objectId) {
    _id = objectId;
}

