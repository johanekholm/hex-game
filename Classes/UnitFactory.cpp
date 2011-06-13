/*
 *  UnitFactory.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "UnitFactory.h"
#include "ModelManager.h"
#include "ViewControllerManager.h"
#include "UnitModel.h"
#include "UnitView.h"

UnitFactory::UnitFactory(ViewControllerManager* viewControllerManager) {
    //_modelManager = modelManager; 
    _viewControllerManager = viewControllerManager;
}

void UnitFactory::produceAndRegisterUnit(const std::string& unitClass, int owner, const MPoint& pos, int direction) {
    UnitModel* unit;
    UnitView* view;
    int hp, ap, power, skill, defense, image;
    
    if (unitClass == "swordsman") {
        hp = 3; ap = 3; power = 3; skill = 2; defense = 3; image = 0;
    } else if (unitClass == "soldier") {
        hp = 2; ap = 3; power = 2; skill = 2; defense = 2; image = 1;
    } else if (unitClass == "archer") {
        hp = 2; ap = 3; power = 2; skill = 3; defense = 1; image = 2;
    } else {
        return;
    }
    
    unit = new UnitModel(pos.x, pos.y, direction, owner, hp, ap, power, skill, defense);
    //unit = new UnitModel(pos.x, pos.y, direction, owner);
	view = new UnitView(unit, 64.0f, 64.0f, image);

    unit->addObserver(view);
    ModelManager::instance()->add(unit);
    _viewControllerManager->add(view);

}

