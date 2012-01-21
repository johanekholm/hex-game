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
#include "UnitModel.h"
#include "Sound.h"
#include "TransitionViewController.h"
#include "ObjectBuilder.h"
#include "MenuView.h"
#include "ScriptManager.h"
#include "StateManager.h"
#include "SceneContext.h"

SceneLoader* SceneLoader::_instance = 0;

SceneLoader* SceneLoader::instance() {
    if (_instance == 0) {
        _instance = new SceneLoader();
    }
    
    return _instance;
}

void SceneLoader::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

SceneLoader::SceneLoader() {
    _currentId = "";
    _previousId = "";
    _rootId = "";
    _isLoaded = false;
    _isPersistent = false;
}

void SceneLoader::loadScene(std::string sceneId, bool isPersistent) {
    DEBUGLOG("Loading scene with id: %s", sceneId.c_str());
    
    if (sceneId == "") {
        return;
    }
    
    this->handleHistory(sceneId);
    
    if (_isLoaded && _isPersistent) {
        StateManager::save(_previousId);
    }
    
    this->clearScene();
    
    StateManager::load(sceneId);
    _isPersistent = isPersistent;
    _isLoaded = true;
    
    // To-Do: scripts should be loaded with state
    ScriptManager::instance()->add(ScriptedAction::build(ScriptedActionNS::END_BATTLE, ModelEventNS::PARTY_WIPEOUT));
}

void SceneLoader::loadPrevious() {
    this->loadScene(_previousId, true);
}

void SceneLoader::loadRoot() {
    this->loadScene(_rootId, true);    
}

void SceneLoader::clearScene() {
    ModelManager::instance()->deleteAllMapObjects();
    ModelManager::instance()->deleteAllUnits();
    ScriptManager::instance()->clear();
    ViewControllerManager::instance()->setCameraPosition(GPointMake(0.0f, 0.0f));
}

void SceneLoader::handleHistory(std::string sceneId) {
    _previousId = _currentId;
    _currentId = sceneId;
    
    if (!_isLoaded) {
        _rootId = _currentId;
    }    
}

void SceneLoader::giveContinousControl(ViewController* control) {
    ViewControllerManager::instance()->add(control);
    ViewControllerManager::instance()->setFocus(control);
}


void SceneLoader::insertUnitsIntoScene(std::vector<UnitModel*>* party1Members, std::vector<UnitModel*>* party2Members) {
	// register passed units
	if (party1Members != 0) {
		for (std::vector<UnitModel*>::iterator it = party1Members->begin(); it != party1Members->end(); ++it) {
			ObjectBuilder::registerUnit(*it);
		}    		
	}
	
	if (party2Members != 0) {
		for (std::vector<UnitModel*>::iterator it = party2Members->begin(); it != party2Members->end(); ++it) {
			ObjectBuilder::registerUnit(*it);
		}		
	}
}

void SceneLoader::removeUnitsFromParties(MapObject* party1, MapObject* party2, std::vector<UnitModel*>* party1Members, std::vector<UnitModel*>* party2Members) {
	
    SceneContext* context = SceneContext::instance();
	
	if (party1 != 0) {
		*party1Members = party1->removeMembers();
		
		// cache party id for later
		context->setPartyId1(party1->getId());
	}

	if (party2 != 0) {
		*party2Members = party2->removeMembers();
	
		// cache party id for later
		context->setPartyId2(party2->getId());
	}
}

void SceneLoader::returnUnitsToParties(std::vector<UnitModel*> units) {
	ModelManager* modelManager = ModelManager::instance();

	MapObject* party1 = modelManager->getMapObjectById(SceneContext::instance()->getPartyId1());
	MapObject* party2 = modelManager->getMapObjectById(SceneContext::instance()->getPartyId1());
	
	// re-insert units to parties
	for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); it++) {
		DEBUGLOG("Moving unit");
		if (party1 != 0 && party1->getOwner() == (*it)->getOwner()) {
			party1->addMember(*it);
			DEBUGLOG("Add to party1");
		} else if (party2 != 0 && party2->getOwner() == (*it)->getOwner()) {
			party2->addMember(*it);
			DEBUGLOG("Add to party2");
		} else {
			delete *it;
			DEBUGLOG("Delete");
		}
	}
	
}

void SceneLoader::loadBattleScene(const std::string& sceneId, MapObject* party1, MapObject* party2) {
    std::vector<UnitModel*> party1Members, party2Members;
	DEBUGLOG("New battle loader");
	
	this->removeUnitsFromParties(party1, party2, &party1Members, &party2Members);
	
    // load new scene, clear model
    this->loadScene(sceneId, false);
	
	this->insertUnitsIntoScene(&party1Members, &party2Members);
    
    CentralControl::instance()->switchMode(ControlMode::BATTLE);
}

void SceneLoader::loadBattleScene(const std::string& mapName, int enemyPartyType, std::vector<UnitModel*> members) {
    HexMapModel* mapModel;
    std::vector<UnitModel*> playerMembers;
    
    
    mapModel = new HexMapModel(4, 4, "4 5 5 5\n5 5 5 5\n5 5 5 5\n5 5 5 4\n");
    
    ModelManager::instance()->setMap(mapModel);
    ViewControllerManager::instance()->setMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.0f));
    
    ViewControllerManager::instance()->removeAllSoftly();

    for (std::vector<UnitModel*>::iterator it = members.begin(); it != members.end(); ++it) {
        ObjectBuilder::registerUnit(*it);
     }
        
    switch (enemyPartyType) {
        case 1:
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(2, 2));
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(3, 2));            
            break;

        case 2:
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(2, 2));            
            //ObjectBuilder::createUnitFromTemplate("archer", 2, MPointMake(2, 3));
            //ObjectBuilder::createUnitFromTemplate("archer", 2, MPointMake(3, 2));            
            break;

        default:
            break;
    }
    
    ScriptManager::instance()->add(ScriptedAction::build(ScriptedActionNS::END_BATTLE, ModelEventNS::PARTY_WIPEOUT));

    Sound::instance()->play("music1");
    
    StateManager::save("state.txt");
}

void SceneLoader::loadDungeonScene(const std::string& sceneId, MapObject* party) {
	std::vector<UnitModel*> partyMembers;
	
	this->removeUnitsFromParties(party, 0, &partyMembers, 0);
	this->loadScene(sceneId, true);
	this->insertUnitsIntoScene(&partyMembers, 0);

    CentralControl::instance()->switchMode(ControlMode::BATTLE);
}

void SceneLoader::loadAdventureScene() {
    this->loadScene("newgame.jsn", true);
    
    /*HexMapModel* mapModel;
    
    mapModel = new HexMapModel(8, 8);
    
    ModelManager::instance()->setMap(mapModel);
    ViewControllerManager::instance()->setMapView(new HexMap(mapModel, TextureCatalog::instance()->get("hexTiles"), 1.5f));
    ObjectBuilder::createMapObjectFromTemplate("party", 1, MPointMake(1, 4));
    ObjectBuilder::createMapObjectFromTemplate("party", 2, MPointMake(3, 2));
    ObjectBuilder::createMapObjectFromTemplate("village", 1, MPointMake(0, 0));
    ObjectBuilder::createMapObjectFromTemplate("dungeon", 1, MPointMake(0, 3));
    
    StateManager::save("temp1.jsn");*/
}

void SceneLoader::returnToAdventureScene() {
	std::vector<UnitModel*> allUnits;
	ModelManager* modelManager = ModelManager::instance();
	
	allUnits = modelManager->removeAllUnits();
	
	this->loadRoot();
	this->returnUnitsToParties(allUnits);
	
	CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
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

