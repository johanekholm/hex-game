/*
 *  ObjectBuilder.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include "toolkit.h"
#include "json-forwards.h"

class HexMapModel;
class UnitModel;
class MapObject;
class ModelManager;
class ViewControllerManager;

class ObjectBuilder {
    ModelManager* _modelManager;
    ViewControllerManager* _viewControllerManager;
    
    
public:
    ObjectBuilder(ViewControllerManager* viewControllerManager);
    static void createMapFromJson(Json::Value& data);
    static void createMapObjectFromJson(Json::Value& data);
    static void createMapObjectFromTemplate(const std::string& unitClass, int owner, const MPoint& pos);
    static void createUnitFromJson(Json::Value& data);    
    static void createUnitFromTemplate(const std::string& unitClass, int owner, const MPoint& pos);
    static void createMenuItemFromTemplate(const std::string& menuItemClass, const GPoint& pos);

    static UnitModel* produceUnit(const std::string& unitClass, int owner, const MPoint& pos);
    static void registerMap(HexMapModel* mapModel);
    static void registerMapObject(MapObject* object);
    static void registerUnit(UnitModel* unit);
    //static void RegisterMenuItem(MenuItemView* menuItemView, const GPoint& pos);
};


