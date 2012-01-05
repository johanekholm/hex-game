/*
 *  StateManager.cpp
 *
 *  Created by Johan Ekholm
 *
 */

#include "StateManager.h"
#include "ModelManager.h"
#include "UnitModel.h"
#include "MapObject.h"
#include "ObjectBuilder.h"
#include "HexMapModel.h"
#include "json.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

std::map<std::string, std::string> StateManager::_inMemoryFileStore = StateManager::initStaticMap();

std::map<std::string, std::string> StateManager::initStaticMap() {
    std::map<std::string, std::string> states;
    
    std::string initialState = "{'map':{'height':8,'hexes':[{'v':0,'x':0,'y':0},{'v':0,'x':1,'y':0},{'v':0,'x':2,'y':0},{'v':3,'x':3,'y':0},{'v':3,'x':4,'y':0},{'v':3,'x':5,'y':0},{'v':0,'x':6,'y':0},{'v':0,'x':7,'y':0},{'v':1,'x':0,'y':1},{'v':1,'x':1,'y':1},{'v':1,'x':2,'y':1},{'v':1,'x':3,'y':1},{'v':3,'x':4,'y':1},{'v':1,'x':5,'y':1},{'v':1,'x':6,'y':1},{'v':1,'x':7,'y':1},{'v':2,'x':0,'y':2},{'v':2,'x':1,'y':2},{'v':1,'x':2,'y':2},{'v':1,'x':3,'y':2},{'v':2,'x':4,'y':2},{'v':2,'x':5,'y':2},{'v':0,'x':6,'y':2},{'v':2,'x':7,'y':2},{'v':1,'x':0,'y':3},{'v':2,'x':1,'y':3},{'v':1,'x':2,'y':3},{'v':1,'x':3,'y':3},{'v':1,'x':4,'y':3},{'v':2,'x':5,'y':3},{'v':1,'x':6,'y':3},{'v':2,'x':7,'y':3},{'v':0,'x':0,'y':4},{'v':1,'x':1,'y':4},{'v':0,'x':2,'y':4},{'v':2,'x':3,'y':4},{'v':0,'x':4,'y':4},{'v':1,'x':5,'y':4},{'v':0,'x':6,'y':4},{'v':2,'x':7,'y':4},{'v':1,'x':0,'y':5},{'v':2,'x':1,'y':5},{'v':1,'x':2,'y':5},{'v':2,'x':3,'y':5},{'v':1,'x':4,'y':5},{'v':0,'x':5,'y':5},{'v':1,'x':6,'y':5},{'v':2,'x':7,'y':5},{'v':0,'x':0,'y':6},{'v':1,'x':1,'y':6},{'v':0,'x':2,'y':6},{'v':2,'x':3,'y':6},{'v':0,'x':4,'y':6},{'v':1,'x':5,'y':6},{'v':0,'x':6,'y':6},{'v':2,'x':7,'y':6},{'v':1,'x':0,'y':7},{'v':2,'x':1,'y':7},{'v':1,'x':2,'y':7},{'v':2,'x':3,'y':7},{'v':1,'x':4,'y':7},{'v':2,'x':5,'y':7},{'v':1,'x':6,'y':7},{'v':2,'x':7,'y':7}],'width':8},'mapObjects':[{'category':1,'id':1,'layer':2,'owner':1,'visualType':1,'x':0,'y':3},{'category':1,'id':2,'layer':2,'owner':2,'visualType':6,'x':3,'y':2},{'category':2,'id':3,'layer':1,'owner':1,'visualType':4,'x':0,'y':0},{'category':3,'id':4,'layer':1,'owner':1,'visualType':5,'x':0,'y':3}],'units':null}";
    
    std::replace( initialState.begin(), initialState.end(), '\'', '"');
    states["newgame.jsn"] = initialState;

    DEBUGLOG("Initial state: %s", initialState.c_str());
    
    return states;
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
    Json::Value& mapObjectsNode = root["mapObjects"];
    Json::Value state;
    std::vector<UnitModel*> units;
    std::vector<MapObject*> mapObjects;
    HexMapModel* map;
    ModelManager* modelManager = ModelManager::instance();
    Json::FastWriter writer;
    
    units = modelManager->getAllUnits();
    mapObjects = modelManager->getAllMapObjects();
    map = modelManager->getMap();
    
    // serialize map
    root["map"] = map->serialize();
    
    // serialize all units
    for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
        if (*it != 0) {
            //unitState = new Json::Value();
            (*it)->serialize(state);
            unitsNode.append(state);
        }
	}

    state.clear();
    
    // serialize all map objects
    for (std::vector<MapObject*>::iterator it = mapObjects.begin(); it != mapObjects.end(); ++it) {
        if (*it != 0) {
            (*it)->serialize(state);
            mapObjectsNode.append(state);
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
    // recreate map
    ObjectBuilder::createMapFromJson(root["map"]);
    
    // recreate units
    for (Json::ValueIterator itr = root["units"].begin(); itr != root["units"].end(); itr++) {
        ObjectBuilder::createUnitFromJson(*itr);
    }

    // recreate map objects
    for (Json::ValueIterator itr = root["mapObjects"].begin(); itr != root["mapObjects"].end(); itr++) {
        ObjectBuilder::createMapObjectFromJson(*itr);
    }
}

