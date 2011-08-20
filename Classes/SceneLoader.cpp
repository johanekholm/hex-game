/*
 *  SceneLoader.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm
 *
 */

#include "SceneLoader.h"

SceneLoader* SceneLoader::_instance = 0;

void SceneLoader::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

void SceneLoader::loadBattleScene(int party1, int party2) {
    
}

void SceneLoader::loadAdventureScene() {
    
}
