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

namespace MapObjectNS {
    const int PARTY = 1;
    const int CITY = 2;
    const int DUNGEON = 3;
}

//struct ActionState;
class Item;

namespace MapObjectCategory {
    const int PARTY = 1;
    const int BUILDING = 2;
    const int CITY = 2;
    const int DUNGEON = 3;
};

struct MapObjectState {
    MPoint pos;
    std::vector<ActionState> actions;
};

class MapObject : public Observable {
    
protected:
    int _category;
    MPoint _pos;
    int _id;
    int _owner;
    std::map<int, AdventureAction*> _actions;
    std::map<int, Item*> _items;
    

public:
    virtual ~MapObject();
    MapObject(int category, MPoint pos, int owner, std::vector<int> actionIds);
    MPoint getPosition();
    AdventureAction* addAction(int action);
    bool canMoveTo(const MPoint& pos);
	void doAction(const ActionState& statePoint);
    std::vector<ActionState> getActions();
    int getOwner();
    virtual MapObjectState getState();
    bool matchesCategory(int category);
    void move(const MPoint& targetPos);
    void setId(int id);
    void addItem(Item* item);
};


class PartyModel : public MapObject {
    
protected:
    std::vector<UnitModel*> _members;

    
public:
    ~PartyModel();
    PartyModel(int category, MPoint pos, int owner, std::vector<int> actionIds, const std::vector<UnitModel*>& members);
    void move(const MPoint& targetPos);
};

/*
class BuildingModel : public MapObject, public Observable {
protected:
    
public:
};
 */

#endif