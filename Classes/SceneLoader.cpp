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
    
	ViewControllerManager::instance()->setFocus(0);
	
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
	bool isEmptyParty1 = true;
	bool isEmptyParty2 = true;	
	ModelManager* modelManager = ModelManager::instance();

	MapObject* party1 = modelManager->getFirstMapObjectWithOwner(1); 
	//modelManager->getMapObjectById(SceneContext::instance()->getPartyId1());
	MapObject* party2 = modelManager->getFirstMapObjectWithOwner(2);  
	//modelManager->getMapObjectById(SceneContext::instance()->getPartyId2());
	
	// re-insert units to parties
	for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); it++) {
		if (party1 != 0 && party1->getOwner() == (*it)->getOwner()) {
			party1->addMember(*it);
			isEmptyParty1 = false;
		} else if (party2 != 0 && party2->getOwner() == (*it)->getOwner()) {
			party2->addMember(*it);
			isEmptyParty2 = false;
		} else {
			delete *it;
		}
	}
	
	if (isEmptyParty1) {
		if (party1->getOwner() != 1) {
			modelManager->deleteMapObject(party1->getId());			
		}
	}

	if (isEmptyParty2) {
		if (party2->getOwner() != 1) {
			modelManager->deleteMapObject(party2->getId());			
		}
	}
	
}

void SceneLoader::loadBattleScene(const std::string& sceneId, MapObject* party1, MapObject* party2) {
    std::vector<UnitModel*> party1Members, party2Members;
		
	ModelManager::instance()->unregisterMapObject(party1->getId());
	ModelManager::instance()->unregisterMapObject(party2->getId());

	this->removeUnitsFromParties(party1, party2, &party1Members, &party2Members);

    // load new scene, clear model
    this->loadScene(sceneId, false);

	party1->move(party1->getPosition() * -1);
	party2->move(party2->getPosition() * -1);

	this->insertUnitsIntoScene(&party1Members, &party2Members);

    ModelManager::instance()->addMapObject(party1);
    ModelManager::instance()->addMapObject(party2);
	
    CentralControl::instance()->switchMode(ControlMode::BATTLE);
}

void SceneLoader::loadBattleSceneFromTemplate(const std::string& mapName, MapObject* party1, int enemyPartyType) {
	this->loadBattleScene(mapName, party1, 0);
	
    switch (enemyPartyType) {
        case 1:
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(2, 2));
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(3, 2));            
            break;

        case 2:
            ObjectBuilder::createUnitFromTemplate("soldier", 2, MPointMake(2, 2));            
            ObjectBuilder::createUnitFromTemplate("archer", 2, MPointMake(2, 3));
            break;

        default:
            break;
    }
    
    Sound::instance()->play("music1");
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
    ObjectBuilder::createMapObjectFromTemplate("party", 1, MPointMake(1, 1));
    ObjectBuilder::createMapObjectFromTemplate("party", 2, MPointMake(3, 2));
    ObjectBuilder::createMapObjectFromTemplate("party", 2, MPointMake(1, 6));
    ObjectBuilder::createMapObjectFromTemplate("party", 2, MPointMake(7, 1));
    ObjectBuilder::createMapObjectFromTemplate("village", 1, MPointMake(0, 0));
    ObjectBuilder::createMapObjectFromTemplate("dungeon", 1, MPointMake(0, 3));
    
    StateManager::save("temp1.jsn");*/
}

void SceneLoader::returnToAdventureScene() {
	std::vector<UnitModel*> allUnits;
	ModelManager* modelManager = ModelManager::instance();
	
	allUnits = modelManager->unregisterAllUnits();

	this->returnUnitsToParties(allUnits);

	MapObject* party1 = modelManager->getFirstMapObjectWithOwner(1); 
	MapObject* party2 = modelManager->getFirstMapObjectWithOwner(2);  
	
	if (party1 != 0) {
		ModelManager::instance()->unregisterMapObject(party1->getId());		
	}

	if (party2 != 0) {
		ModelManager::instance()->unregisterMapObject(party2->getId());		
	}

	this->loadRoot();
	
	ModelManager::instance()->addMapObject(party1);
    ModelManager::instance()->addMapObject(party2);

	if (party1 != 0) {
		party1->move(party1->getPosition() * -1);
	}
	
	if (party2 != 0) {
		party2->move(party2->getPosition() * -1);		
	}
	
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
	CentralControl::instance()->switchMode(ControlMode::ADVENTURE);                
}

