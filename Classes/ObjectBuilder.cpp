/*
 *  ObjectBuilder.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "ObjectBuilder.h"
#include "ModelManager.h"
#include "ViewControllerManager.h"
#include "UnitModel.h"
#include "UnitView.h"
#include "Action.h"
#include "MapObject.h"
#include "MapObjectView.h"
#include "HexMapModel.h"
#include "HexMap.h"
#include "TextureCatalog.h"


#include "json.h"

ObjectBuilder::ObjectBuilder(ViewControllerManager* viewControllerManager) {
    //_modelManager = modelManager; 
    _viewControllerManager = viewControllerManager;
}

void ObjectBuilder::createMapObjectFromTemplate(const std::string& objectType, int owner, const MPoint& pos) {
    MapObject* object;
    std::vector<UnitModel*> units;
    std::vector<int> actions;
    int image = 0;
    int layer = 0;
    
    if (objectType == "village") {
        image = 4; layer = MapLayer::BUILDING;
        object = new MapObject(MapObjectCategory::CITY, pos, owner, layer, image, actions, units);
    } else if (objectType == "dungeon") {
        image = 5; layer = MapLayer::BUILDING;
        object = new MapObject(MapObjectCategory::DUNGEON, pos, owner, layer, image, actions, units);
    } else if (objectType == "party") {
        layer = MapLayer::UNIT;
        if (owner == 1) {
            image = 1;
            units.push_back(produceUnit("hero", owner, MPointMake(1,1)));
            units.push_back(produceUnit("archer", owner, MPointMake(1,0)));
        } else {
            image = 6;
            units.push_back(produceUnit("soldier", owner, MPointMake(2,2)));
            units.push_back(produceUnit("archer", owner, MPointMake(2,3)));
        }
        
        actions.push_back(ActionNS::AACTION_MOVE); actions.push_back(ActionNS::AACTION_FIGHT);
        actions.push_back(ActionNS::AACTION_CITY); actions.push_back(ActionNS::AACTION_ENTERDUNGEON);
        actions.push_back(ActionNS::AACTION_INVENTORY); actions.push_back(ActionNS::AACTION_PARTYOPTIONS);
        object = new MapObject(MapObjectCategory::PARTY, pos, owner, layer, image, actions, units);
    } else {
        return;
    }
    
    registerMapObject(object);
}

UnitModel* ObjectBuilder::produceUnit(const std::string& unitClass, int owner, const MPoint& pos) {
    UnitModel* unit;
    
    unit = new UnitModel(unitClass, pos, owner); 
    
    return unit;
}


void ObjectBuilder::createUnitFromTemplate(const std::string& unitClass, int owner, const MPoint& pos) {
    UnitModel* unit;
    UnitViewController* view;
    
    unit = produceUnit(unitClass, owner, pos);
	view = new UnitViewController(unit, 64.0f, 64.0f, unit->getVisualType());

    unit->addObserver(view);
    ModelManager::instance()->addUnit(unit);
    ViewControllerManager::instance()->add(view);

}

void ObjectBuilder::registerMap(HexMapModel* mapModel) {
    ModelManager::instance()->setMap(mapModel);
    ViewControllerManager::instance()->setMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), mapModel->getScale()));
}

void ObjectBuilder::registerMapObject(MapObject* object) {
    MapObjectView* view = new MapObjectView(object, 64.0f, 64.0f, object->getVisualType(), object->getLayer());
    object->addObserver(view);
    
    ModelManager::instance()->addMapObject(object);
    ViewControllerManager::instance()->add(view);
}

void ObjectBuilder::registerUnit(UnitModel* unit) {
    UnitViewController* view;
    
	view = new UnitViewController(unit, 64.0f, 64.0f, unit->getVisualType());
    
    unit->addObserver(view);
    ModelManager::instance()->addUnit(unit);
    ViewControllerManager::instance()->add(view);
}

void ObjectBuilder::createMapFromJson(Json::Value& data) {
    HexMapModel* mapModel = new HexMapModel();
    mapModel->deserialize(data);
    
    registerMap(mapModel);
}

void ObjectBuilder::createMapObjectFromJson(Json::Value& data) {
    MapObject* object = new MapObject();
    object->deserialize(data);
    
    registerMapObject(object);
}

void ObjectBuilder::createUnitFromJson(Json::Value& data) {
    UnitModel* unit = new UnitModel();
    unit->deserialize(data);
    
    registerUnit(unit);
}

