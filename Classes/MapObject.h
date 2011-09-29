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

namespace MapObjectCategory {
    const int PARTY = 1;
    const int BUILDING = 2;
};

struct MapObjectState {
    MPoint pos;
    std::vector<ActionState> actions;
};

class MapObject : public Observable {
    
protected:
    int _category;
    MPoint _pos;
    int _allegiance;
    int _id;
    std::map<int, AdventureAction*> _actions;
    

public:
    virtual ~MapObject();
    MapObject(int category, MPoint pos, int allegiance, std::vector<int> actionIds);
    MPoint getPosition();
    AdventureAction* addAction(int action);
    bool canMoveTo(const MPoint& pos);
	void doAction(const ActionState& statePoint);
    std::vector<ActionState> getActions();
    virtual MapObjectState getState();
    bool matchesCategory(int category);
    void move(const MPoint& targetPos);
    void setId(int id);
};


class PartyModel : public MapObject {
    
protected:
    std::vector<UnitModel*> _members;

    
public:
    ~PartyModel();
    PartyModel(int category, MPoint pos, int allegiance, std::vector<int> actionIds, const std::vector<UnitModel*>& members);
    void move(const MPoint& targetPos);
};

/*
class BuildingModel : public MapObject, public Observable {
protected:
    
public:
};
 */

#endif