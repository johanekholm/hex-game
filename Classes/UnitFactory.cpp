/*
 *  UnitFactory.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "UnitFactory.h"
#include "ModelManager.h"
#include "ViewControllerManager.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "Action.h"
#include "MapObject.h"
#include "MapObjectView.h"

UnitFactory::UnitFactory(ViewControllerManager* viewControllerManager) {
    //_modelManager = modelManager; 
    _viewControllerManager = viewControllerManager;
}

void UnitFactory::produceAndRegisterMapObject(const std::string& objectType, int owner, const MPoint& pos) {
    MapObject* object;
    MapObjectView* view;
    std::vector<UnitModel*> units;
    std::vector<int> actions;
    int image;
    
    if (objectType == "village") {
        image = 0;
        object = new MapObject(pos, owner);
    } else if (objectType == "party") {
        image = 1;
        actions.push_back(ADV_ACTION_MOVE);
        units.push_back(produceUnit("soldier", owner, MPointMake(0,0)));
        units.push_back(produceUnit("soldier", owner, MPointMake(1,0)));
        object = new PartyModel(pos, owner, actions, units);
    } else {
        return;
    }
    
    view = new MapObjectView(object, 64.0f, 64.0f, image);
    object->addObserver(view);
    
    ModelManager::instance()->addMapObject(object);
    ViewControllerManager::instance()->add(view);
}

UnitModel* UnitFactory::produceUnit(const std::string& unitClass, int owner, const MPoint& pos) {
    UnitModel* unit;
    int hp, ap, power, skill, defense, image;
    std::vector<int> actions;
    
    if (unitClass == "swordsman") {
        hp = 5; ap = 3; power = 3; skill = 2; defense = 3; image = 0;
        actions.push_back(ACTION_MOVE); actions.push_back(ACTION_STRIKE);
    } else if (unitClass == "soldier") {
        hp = 4; ap = 3; power = 2; skill = 2; defense = 2; image = 1;
        actions.push_back(ACTION_MOVE); actions.push_back(ACTION_STRIKE);
    } else if (unitClass == "archer") {
        hp = 4; ap = 3; power = 2; skill = 3; defense = 1; image = 2;
        actions.push_back(ACTION_MOVE); actions.push_back(ACTION_FIRE);
    } else if (unitClass == "channeler") {
        hp = 4; ap = 5; power = 2; skill = 3; defense = 1; image = 3;
        actions.push_back(ACTION_MOVE); actions.push_back(ACTION_BURN); actions.push_back(ACTION_HEAL);
    } else {
        return 0;
    }
    
    unit = new UnitModel(pos.x, pos.y, owner, hp, ap, power, skill, defense, actions, image);
    
    return unit;
}


void UnitFactory::produceAndRegisterUnit(const std::string& unitClass, int owner, const MPoint& pos) {
    UnitModel* unit;
    UnitView* view;
    
    unit = produceUnit(unitClass, owner, pos);
	view = new UnitView(unit, 64.0f, 64.0f, unit->getVisualType());

    unit->addObserver(view);
    ModelManager::instance()->addUnit(unit);
    ViewControllerManager::instance()->add(view);

}

