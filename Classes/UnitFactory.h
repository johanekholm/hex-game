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
#include "json-forwards.h"

class UnitModel;
class ModelManager;
class ViewControllerManager;

class UnitFactory {
    ModelManager* _modelManager;
    ViewControllerManager* _viewControllerManager;
    
    
public:
    UnitFactory(ViewControllerManager* viewControllerManager);
    static void createUnitFromJson(Json::Value& unitData);    
    static void createMapObjectFromTemplate(const std::string& unitClass, int owner, const MPoint& pos);
    static void createUnitFromTemplate(const std::string& unitClass, int owner, const MPoint& pos);
    static void createMenuItemFromTemplate(const std::string& menuItemClass, const GPoint& pos);
    static UnitModel* produceUnit(const std::string& unitClass, int owner, const MPoint& pos);
    static void registerUnit(UnitModel* unit);
    //static void RegisterMenuItem(MenuItemView* menuItemView, const GPoint& pos);
};


