/*
 *  SceneLoader.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm
 *
 */

#include "SceneLoader.h"
#include "ModelManager.h"
#include "CentralControl.h"
#include "HexMapModel.h"
#include "ViewControllerManager.h"
#include "TextureCatalog.h"
#include "HexMap.h"
#include "HexMapModel.h"
#include "UnitFactory.h"

SceneLoader* SceneLoader::_instance = 0;

void SceneLoader::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

SceneLoader::SceneLoader() {
    
}

void SceneLoader::loadBattleScene() {
    HexMapModel* mapModel;
    
    mapModel = new HexMapModel(4, 4);

    ModelManager::instance()->setBattleMap(mapModel);
    ViewControllerManager::instance()->pushMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.0f));
}

void SceneLoader::loadAdventureScene() {
    HexMapModel* mapModel;
    
    mapModel = new HexMapModel(8, 6);
    
    ModelManager::instance()->setAdventureMap(mapModel);
    ViewControllerManager::instance()->setMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.5f));
}

void SceneLoader::switchToAdventureScene() {
    ModelManager::instance()->removeAllUnits();
    ViewControllerManager::instance()->popMapView();
    
    UnitFactory::produceAndRegisterMapObject("village", 1, MPointMake(1, 1));
    UnitFactory::produceAndRegisterMapObject("village", 1, MPointMake(0, 0));
}
