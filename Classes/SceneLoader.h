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
#include <vector>
#include "toolkit.h"

class ViewController;
class MenuViewController;
class TransitionViewController;
class UnitModel;
class MapObject;

class SceneLoader {
	static SceneLoader* _instance;
    std::string _currentId;
    std::string _previousId;
    std::string _rootId;
    bool _isPersistent;
    bool _isLoaded;
    
	SceneLoader();
    void handleHistory(std::string sceneId);
    
public:
	static SceneLoader* instance();
	static void destroy();
	
    void clearScene();
    void loadScene(std::string sceneId, bool isPersistent);
    void loadPrevious();
    void loadRoot();
    
	void insertUnitsIntoScene(std::vector<UnitModel*>* party1Members, std::vector<UnitModel*>* party2Members);
	void removeUnitsFromParties(MapObject* party1, MapObject* party2, std::vector<UnitModel*> *party1Members, std::vector<UnitModel*> *party2Members);
	void returnUnitsToParties(std::vector<UnitModel*> units);
    
	void loadAdventureScene();
	void returnToAdventureScene();
	void loadBattleScene(const std::string& sceneId, MapObject* party1, MapObject* party2);
    void loadBattleScene(const std::string& mapName, int enemyPartyType, std::vector<UnitModel*> members);
	void loadDungeonScene(const std::string& sceneId, MapObject* party);
    void returnFromMenu();
    void switchToMainMenu();
    void switchToMenu(MenuViewController* menu);
    void switchToTransition(TransitionViewController* transition);
    void giveContinousControl(ViewController* control);
};

#endif