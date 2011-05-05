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

void UnitFactory::produceAndRegisterUnit(const std::string& unitClass, int owner, const MPoint& pos) {
    UnitModel* unit;
    UnitView* view;
    
    unit = new UnitModel(pos.x, pos.y);
	view = new UnitView(unit, 64.0f, 64.0f, 0);

    unit->addObserver(view);
    ModelManager::instance()->add(unit);
    _viewControllerManager->add(view);

}

