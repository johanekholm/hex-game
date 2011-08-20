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

SceneLoader* SceneLoader::_instance = 0;

void SceneLoader::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

SceneLoader::SceneLoader() {
    _viewControllerManager = CentralControl::instance()->getViewControllerManager();
}

void SceneLoader::loadBattleScene(int party1, int party2) {
    ModelManager::instance()->setBattleMap(new HexMapModel(4, 4));
    _viewControllerManager->buildHexMapBattle();
}

void SceneLoader::loadAdventureScene() {
    ModelManager::instance()->setAdventureMap(new HexMapModel(8, 6));
    _viewControllerManager->buildHexMapAdventure();    
}

void SceneLoader::switchToAdventureScene() {
    _viewControllerManager->buildHexMapAdventure();    
}
