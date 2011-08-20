//
//  MapObject.h
//  hex-game
//
//  Created by Johan Ekholm
//

#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "toolkit.h"

class MapObject {
    
protected:
    MPoint _pos;
    int _allegiance;
    int _id;
    
public:
    virtual ~MapObject();
    MapObject(MPoint pos, int allegiance);
    
    void setId(int id);
};

/*
class Party : public MapObject {
protected:
    
public:

};


class Building : public MapObject {
protected:
    
public:
};
 */

#endif