/*
 *  UnitFactory.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include "toolkit.h"

class UnitModel;
class ModelManager;
class ViewControllerManager;

class UnitFactory {
    ModelManager* _modelManager;
    ViewControllerManager* _viewControllerManager;
    
    
public:
    UnitFactory(ViewControllerManager* viewControllerManager);
    void produceAndRegisterUnit(const std::string& unitClass, int owner, const MPoint& pos, int direction);
};


