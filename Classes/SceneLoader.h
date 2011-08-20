/*
 *  TextureCatalog.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <map>
#include <string>
#include "toolkit.h"

class ViewControllerManager;

class SceneLoader {
	static SceneLoader* _instance;
    ViewControllerManager* _viewControllerManager;
    
	SceneLoader();

public:
	static SceneLoader* instance() {
		if (_instance == 0) {
			_instance = new SceneLoader();
		}
		
		return _instance;
	}
	
	static void destroy();
	
    void loadBattleScene(int party1, int party2);
    void loadAdventureScene();
    void switchToAdventureScene();
};

#endif