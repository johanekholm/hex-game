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
#include "MapObject.h"
#include "Sound.h"
#include "TransitionViewController.h"
#include "UnitFactory.h"
#include "MenuView.h"
#include "ScriptManager.h"
#include "StateManager.h"

SceneLoader* SceneLoader::_instance = 0;

void SceneLoader::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

SceneLoader::SceneLoader() {
    
}

void SceneLoader::giveContinousControl(ViewController* control) {
    ViewControllerManager::instance()->add(control);
    ViewControllerManager::instance()->setFocus(control);
}

void SceneLoader::loadBattleScene() {
    HexMapModel* mapModel;
    
    mapModel = new HexMapModel(4, 4);

    ModelManager::instance()->setBattleMap(mapModel);
    ViewControllerManager::instance()->pushMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.0f));

    ModelManager::instance()->removeAllMapObjects();

    UnitFactory::produceAndRegisterUnit("swordsman", 1, MPointMake(1, 0));
    UnitFactory::produceAndRegisterUnit("archer", 1, MPointMake(0, 0));
    UnitFactory::produceAndRegisterUnit("soldier", 2, MPointMake(1, 1));
    UnitFactory::produceAndRegisterUnit("channeler", 2, MPointMake(2, 1));
    
    // Sound::instance()->play("music1");
}

void SceneLoader::loadBattleScene(const std::string& mapName, int enemyPartyType, std::vector<UnitModel*> members) {
    HexMapModel* mapModel;
    std::vector<UnitModel*> playerMembers;
    
    mapModel = new HexMapModel(4, 4, "4 5 5 5\n5 5 5 5\n5 5 5 5\n5 5 5 4\n");
    
    ModelManager::instance()->setBattleMap(mapModel);
    ViewControllerManager::instance()->pushMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.0f));
    
    ViewControllerManager::instance()->removeAllSoftly();

    for (std::vector<UnitModel*>::iterator it = members.begin(); it != members.end(); ++it) {
        UnitFactory::registerUnit(*it);
     }
        
    switch (enemyPartyType) {
        case 1:
            UnitFactory::produceAndRegisterUnit("soldier", 2, MPointMake(2, 2));
            UnitFactory::produceAndRegisterUnit("soldier", 2, MPointMake(3, 2));            
            break;

        case 2:
            UnitFactory::produceAndRegisterUnit("soldier", 2, MPointMake(2, 2));            
            UnitFactory::produceAndRegisterUnit("archer", 2, MPointMake(2, 3));
            UnitFactory::produceAndRegisterUnit("archer", 2, MPointMake(3, 2));            
            break;

        default:
            break;
    }
    
    ScriptManager::instance()->add(ScriptedAction::build(ScriptedActionNS::END_BATTLE, ModelEventNS::PARTY_WIPEOUT));

    Sound::instance()->play("music1");
    
    StateManager::save("state.txt");
}

void SceneLoader::loadAdventureScene() {
    HexMapModel* mapModel;
    
    mapModel = new HexMapModel(8, 8);
    
    ModelManager::instance()->setAdventureMap(mapModel);
    ViewControllerManager::instance()->setMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.5f));
    UnitFactory::produceAndRegisterMapObject("party", 1, MPointMake(1, 4));
    UnitFactory::produceAndRegisterMapObject("party", 2, MPointMake(3, 2));
    UnitFactory::produceAndRegisterMapObject("village", 1, MPointMake(0, 0));
    UnitFactory::produceAndRegisterMapObject("dungeon", 1, MPointMake(0, 3));

}

void SceneLoader::switchToAdventureScene() {
    ModelManager::instance()->removeAllUnits();
    ViewControllerManager::instance()->popMapView();
    
    /*UnitFactory::produceAndRegisterMapObject("party", 1, MPointMake(1, 4));
    UnitFactory::produceAndRegisterMapObject("party", 2, MPointMake(3, 2));
    UnitFactory::produceAndRegisterMapObject("village", 1, MPointMake(0, 0));
    UnitFactory::produceAndRegisterMapObject("dungeon", 1, MPointMake(0, 3));
     */
}

void SceneLoader::switchToMainMenu() {
    ViewController* menu;
    
    menu = new MenuViewController();
    ViewControllerManager::instance()->add(menu);
    ViewControllerManager::instance()->setFocus(menu);
}

void SceneLoader::switchToMenu(MenuViewController* menu) {
    ViewControllerManager::instance()->add(menu);
    ViewControllerManager::instance()->setFocus(menu);
}

void SceneLoader::switchToTransition(TransitionViewController* transition) {
    ViewControllerManager::instance()->add(transition);
    ViewControllerManager::instance()->setFocus(transition);
}

void SceneLoader::returnFromMenu() {
    ViewControllerManager* viewControllerManager = ViewControllerManager::instance();
    viewControllerManager->remove(viewControllerManager->getFocus());
}

