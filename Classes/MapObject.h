//
//  MapObject.h
//  hex-game
//
//  Created by Johan Ekholm
//

#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "Observable.h"
#include "Action.h"
#include "toolkit.h"
#include <vector>

//struct ActionState;

struct MapObjectState {
    MPoint pos;
    std::vector<ActionState> actions;
};

class MapObject : public Observable {
    
protected:
    MPoint _pos;
    int _allegiance;
    int _id;
    
public:
    virtual ~MapObject();
    MapObject(MPoint pos, int allegiance);
    virtual void availableActions(std::vector<ActionState>& actions);
    MapObjectState getState();    
    void setId(int id);
};


class PartyModel : public MapObject {
    
protected:
    std::vector<UnitModel*> _members;
    std::map<int, AdventureAction*> _actions;

    
public:
    ~PartyModel();
    PartyModel(MPoint pos, int allegiance, const std::vector<UnitModel*>& members);
    void availableActions(std::vector<ActionState>& actions);
    AdventureAction* addAction(int action);
	void doAction(const ActionState& statePoint);
    MPoint getPosition();

};

/*
class BuildingModel : public MapObject, public Observable {
protected:
    
public:
};
 */

#endif