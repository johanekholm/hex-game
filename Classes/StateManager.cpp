/*
 *  StateManager.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "StateManager.h"
#include "ModelManager.h"
#include "UnitModel.h"
#include "UnitFactory.h"
#include "json.h"
#include <string>
#include <vector>
#include <iostream>

std::map<std::string, std::string> StateManager::_inMemoryFileStore = StateManager::initStaticMap();

std::map<std::string, std::string> StateManager::initStaticMap() {
    std::map<std::string, std::string> emptyMap;
    
    return emptyMap;
}

void StateManager::load(const std::string& filename) {
    Json::Value state;
    
    loadStateFromFile(state, filename);
    recreateFromState(state);
}

void StateManager::save(const std::string& filename) {
    Json::Value state;
    
    createState(state);
    saveStateToFile(state, filename);
}

void StateManager::createState(Json::Value& root) {
    Json::Value& unitsNode = root["units"];
    Json::Value* unitState;
    std::vector<UnitModel*> units;
    ModelManager* modelManager = ModelManager::instance();
    Json::FastWriter writer;
    
    units = modelManager->getAllUnits();

    for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
        if (*it != 0) {
            unitState = new Json::Value();
            (*it)->serialize(*unitState);
            unitsNode.append(*unitState);
        }
	}
}

void StateManager::loadStateFromFile(Json::Value& root, const std::string& filename) {
    // To-Do: implement load from file
    Json::Reader reader;
    std::string jsonString;
    std::map<std::string, std::string>::iterator it;
    
    it = _inMemoryFileStore.find(filename);
    
    if (it != _inMemoryFileStore.end()) {
        jsonString = it->second;
        reader.parse(jsonString, root);
    }

}

void StateManager::saveStateToFile(Json::Value& root, const std::string& filename) {
    // To-Do: implement save to file    
    
    Json::FastWriter writer;
    std::string jsonString;
    
    jsonString = writer.write(root);
    DEBUGLOG("State saved with data: %s", jsonString.c_str());
    
    // save to in-memory filestore
    _inMemoryFileStore[filename] = jsonString;
}

void StateManager::recreateFromState(Json::Value& root) {
    // To-Do: implement loading objects
    
    // recreate units
    for (Json::ValueIterator itr = root["units"].begin(); itr != root["units"].end(); itr++) {
        UnitFactory::createUnitFromJson(*itr);
    }
}

