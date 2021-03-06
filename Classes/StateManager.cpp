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
#include "SceneContext.h"
#include "ScriptManager.h"
#include "ResourceLoader.h"
#include "json.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

std::map<std::string, std::string> StateManager::_inMemoryFileStore = StateManager::initStaticMap();

std::map<std::string, std::string> StateManager::initStaticMap() {
    std::map<std::string, std::string> states;
    
	std::string adventureState = "{'context':{'partyId1':0,'partyId2':0},'map':{'height':8,'hexes':[{'v':0,'x':0,'y':0},{'v':0,'x':1,'y':0},{'v':0,'x':2,'y':0},{'v':3,'x':3,'y':0},{'v':3,'x':4,'y':0},{'v':3,'x':5,'y':0},{'v':0,'x':6,'y':0},{'v':0,'x':7,'y':0},{'v':1,'x':0,'y':1},{'v':1,'x':1,'y':1},{'v':1,'x':2,'y':1},{'v':1,'x':3,'y':1},{'v':3,'x':4,'y':1},{'v':1,'x':5,'y':1},{'v':1,'x':6,'y':1},{'v':1,'x':7,'y':1},{'v':2,'x':0,'y':2},{'v':2,'x':1,'y':2},{'v':1,'x':2,'y':2},{'v':1,'x':3,'y':2},{'v':2,'x':4,'y':2},{'v':2,'x':5,'y':2},{'v':0,'x':6,'y':2},{'v':2,'x':7,'y':2},{'v':1,'x':0,'y':3},{'v':2,'x':1,'y':3},{'v':1,'x':2,'y':3},{'v':1,'x':3,'y':3},{'v':1,'x':4,'y':3},{'v':2,'x':5,'y':3},{'v':1,'x':6,'y':3},{'v':2,'x':7,'y':3},{'v':0,'x':0,'y':4},{'v':1,'x':1,'y':4},{'v':0,'x':2,'y':4},{'v':2,'x':3,'y':4},{'v':0,'x':4,'y':4},{'v':1,'x':5,'y':4},{'v':0,'x':6,'y':4},{'v':2,'x':7,'y':4},{'v':1,'x':0,'y':5},{'v':2,'x':1,'y':5},{'v':1,'x':2,'y':5},{'v':2,'x':3,'y':5},{'v':1,'x':4,'y':5},{'v':0,'x':5,'y':5},{'v':1,'x':6,'y':5},{'v':2,'x':7,'y':5},{'v':0,'x':0,'y':6},{'v':1,'x':1,'y':6},{'v':0,'x':2,'y':6},{'v':2,'x':3,'y':6},{'v':0,'x':4,'y':6},{'v':1,'x':5,'y':6},{'v':0,'x':6,'y':6},{'v':2,'x':7,'y':6},{'v':1,'x':0,'y':7},{'v':2,'x':1,'y':7},{'v':1,'x':2,'y':7},{'v':2,'x':3,'y':7},{'v':1,'x':4,'y':7},{'v':2,'x':5,'y':7},{'v':1,'x':6,'y':7},{'v':2,'x':7,'y':7}],'scale':1.50,'width':8},'mapObjects':[{'actions':[0,1,7,8,9,10],'category':1,'id':1,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':2,'members':[{'ap':6,'equip':null,'hp':10,'id':1,'owner':1,'type':'hero','x':1,'y':1},{'ap':0,'equip':null,'hp':3,'id':1,'owner':1,'type':'channeler','x':1,'y':0}],'owner':1,'visualType':11,'x':1,'y':3},{'actions':[0,1,7,8,9,10],'category':1,'id':2,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':2,'members':[{'ap':0,'equip':null,'hp':4,'id':0,'owner':2,'type':'wildling','x':2,'y':2},{'ap':5,'equip':null,'hp':3,'id':1,'owner':2,'type':'brute','x':2,'y':3}],'owner':2,'visualType':8,'x':3,'y':2},{'actions':[0,1,7,8,9,10],'category':1,'id':3,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':2,'members':[{'ap':3,'equip':null,'hp':4,'id':0,'owner':2,'type':'wildling','x':2,'y':2},{'ap':0,'equip':null,'hp':3,'id':1,'owner':2,'type':'wildling','x':2,'y':3}],'owner':2,'visualType':8,'x':1,'y':6},{'actions':[0,1,7,8,9,10],'category':1,'id':4,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':2,'members':[{'ap':1,'equip':null,'hp':4,'id':0,'owner':2,'type':'wildling','x':2,'y':2},{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'brute','x':2,'y':3}],'owner':2,'visualType':8,'x':7,'y':1},{'actions':null,'category':2,'id':5,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':1,'members':null,'owner':1,'visualType':4,'x':0,'y':0},{'actions':null,'category':3,'id':6,'items':[{'count':1,'type':1},{'count':10,'type':5}],'layer':1,'members':null,'owner':1,'visualType':5,'x':0,'y':3}],'units':null}";
    
    std::string battleState = "{'map':{'height':4,'hexes':[{'v':4,'x':0,'y':0},{'v':5,'x':1,'y':0},{'v':5,'x':2,'y':0},{'v':5,'x':3,'y':0},{'v':5,'x':0,'y':1},{'v':5,'x':1,'y':1},{'v':5,'x':2,'y':1},{'v':5,'x':3,'y':1},{'v':5,'x':0,'y':2},{'v':5,'x':1,'y':2},{'v':5,'x':2,'y':2},{'v':5,'x':3,'y':2},{'v':5,'x':0,'y':3},{'v':5,'x':1,'y':3},{'v':5,'x':2,'y':3},{'v':4,'x':3,'y':3}],'scale':1.0,'width':4},'units':[{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'archer','x':1,'y':1},{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'archer','x':2,'y':1},{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'archer','x':2,'y':2}]}";
	
	std::string dungeon1 = "{'map':{'height':4,'hexes':[{'v':4,'x':0,'y':0},{'v':5,'x':1,'y':0},{'v':5,'x':2,'y':0},{'v':5,'x':3,'y':0},{'v':5,'x':0,'y':1},{'v':5,'x':1,'y':1},{'v':5,'x':2,'y':1},{'v':5,'x':3,'y':1},{'v':5,'x':0,'y':2},{'v':5,'x':1,'y':2},{'v':5,'x':2,'y':2},{'v':5,'x':3,'y':2},{'v':5,'x':0,'y':3},{'v':5,'x':1,'y':3},{'v':5,'x':2,'y':3},{'v':4,'x':3,'y':3}],'scale':1.0,'width':4},'units':[{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'wildling','x':1,'y':1}],'scriptedActions':[{'eventId':1,'actionId':'NEXT_DUNGEON_ROOM','sceneId':'dungeon2.txt'}]}";

	std::string dungeon2 = "{'map':{'height':4,'hexes':[{'v':4,'x':0,'y':0},{'v':5,'x':1,'y':0},{'v':5,'x':2,'y':0},{'v':5,'x':3,'y':0},{'v':5,'x':0,'y':1},{'v':5,'x':1,'y':1},{'v':5,'x':2,'y':1},{'v':5,'x':3,'y':1},{'v':5,'x':0,'y':2},{'v':5,'x':1,'y':2},{'v':5,'x':2,'y':2},{'v':5,'x':3,'y':2},{'v':5,'x':0,'y':3},{'v':5,'x':1,'y':3},{'v':5,'x':2,'y':3},{'v':4,'x':3,'y':3}],'scale':1.0,'width':4},'units':[{'ap':1,'equip':null,'hp':3,'id':8,'owner':2,'type':'brute','x':2,'y':2}],'scriptedActions':[{'eventId':1,'actionId':'NEXT_DUNGEON_ROOM','sceneId':'dungeon3.txt'}]}";
    
	std::string dungeon3 = "{'map':{'height':4,'hexes':[{'v':4,'x':0,'y':0},{'v':5,'x':1,'y':0},{'v':5,'x':2,'y':0},{'v':5,'x':3,'y':0},{'v':5,'x':0,'y':1},{'v':5,'x':1,'y':1},{'v':5,'x':2,'y':1},{'v':5,'x':3,'y':1},{'v':5,'x':0,'y':2},{'v':5,'x':1,'y':2},{'v':5,'x':2,'y':2},{'v':5,'x':3,'y':2},{'v':5,'x':0,'y':3},{'v':5,'x':1,'y':3},{'v':5,'x':2,'y':3},{'v':4,'x':3,'y':3}],'scale':1.0,'width':4},'units':[{'ap':1,'equip':null,'hp':5,'id':8,'owner':2,'type':'hero','x':2,'y':2}],'scriptedActions':[{'eventId':1,'actionId':'END_BATTLE'}]}";

    std::string battleMap = "{'map':{'height':4,'hexes':[{'v':4,'x':0,'y':0},{'v':5,'x':1,'y':0},{'v':5,'x':2,'y':0},{'v':5,'x':3,'y':0},{'v':5,'x':0,'y':1},{'v':5,'x':1,'y':1},{'v':5,'x':2,'y':1},{'v':5,'x':3,'y':1},{'v':5,'x':0,'y':2},{'v':5,'x':1,'y':2},{'v':6,'x':2,'y':2},{'v':5,'x':3,'y':2},{'v':5,'x':0,'y':3},{'v':5,'x':1,'y':3},{'v':5,'x':2,'y':3},{'v':4,'x':3,'y':3}],'scale':1.0,'width':4},'units':null,'scriptedActions':[{'eventId':1,'actionId':'END_BATTLE'}]}";
	
	std::string unitModelTemplates = 
	"{'templates':[{'type':'hero','name':'HERO','actions':[0,1],'defense':5,'power':5,'skill':4,'maxAp':30,'maxHp':10,'visualType':0}, {'type':'soldier','name':'SOLDIER','actions':[0,1],'defense':3,'power':3,'skill':2,'maxAp':30,'maxHp':5,'visualType':1}, {'type':'channeler','name':'CHANNELER','actions':[0,1,5],'defense':3,'power':3,'skill':2,'maxAp':30,'maxHp':3,'visualType':2}, {'type':'archer','name':'ARCHER','actions':[0,2],'defense':3,'power':3,'skill':2,'maxAp':30,'maxHp':3,'visualType':3}, {'type':'wildling','name':'WILDLING','actions':[0,1],'defense':2,'power':2,'skill':3,'maxAp':30,'maxHp':3,'visualType':8}, {'type':'brute','name':'BRUTE','actions':[0,1],'defense':4,'power':3,'skill':3,'maxAp':30,'maxHp':4,'visualType':9}]}";

    // replace single with double qoutes, as per json standard
    std::replace(adventureState.begin(), adventureState.end(), '\'', '"');
    std::replace(battleState.begin(), battleState.end(), '\'', '"');
    std::replace(dungeon1.begin(), dungeon1.end(), '\'', '"');
    std::replace(dungeon2.begin(), dungeon2.end(), '\'', '"');
    std::replace(dungeon3.begin(), dungeon3.end(), '\'', '"');
    std::replace(battleMap.begin(), battleMap.end(), '\'', '"');
    std::replace(unitModelTemplates.begin(), unitModelTemplates.end(), '\'', '"');

    states["newgame.txt"] = adventureState;
    states["battle1.txt"] = battleState;
    states["dungeon1.txt"] = dungeon1;
    states["dungeon2.txt"] = dungeon2;
    states["dungeon3.txt"] = dungeon3;
    states["battleMap1.txt"] = battleMap;
    states["UnitModelTemplates.txt"] = unitModelTemplates;

    return states;
}

void StateManager::load(const std::string& filename) {
    Json::Value state;
    
    DEBUGLOG("Loading state with id: %s", filename.c_str());
    loadStateFromFile(state, filename);
    recreateFromState(state);
}

void StateManager::save(const std::string& filename) {
    Json::Value state;
    
    DEBUGLOG("Saving state with id: %s", filename.c_str());
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

	// serialize context
    root["context"] = SceneContext::instance()->serialize();

    // serialize all units
    for (std::vector<UnitModel*>::iterator it = units.begin(); it != units.end(); ++it) {
        if (*it != 0) {
            unitsNode.append((*it)->serialize());
        }
	}

    state.clear();
    
    // serialize all map objects
    for (std::vector<MapObject*>::iterator it = mapObjects.begin(); it != mapObjects.end(); ++it) {
        if (*it != 0) {
            mapObjectsNode.append((*it)->serialize());
        }
	}
}

void StateManager::loadStateFromFile(Json::Value& root, const std::string& filename) {
    Json::Reader reader;
    std::string jsonString;
    std::map<std::string, std::string>::iterator it;
    
	if (STATEMANAGER_USE_FILESTORE) {
		// load from file system
		if (ResourceLoader::doesFileExist(filename, DirNS::SAVE)) {
			DEBUGLOG("Load from save");
			jsonString = ResourceLoader::loadFileAsString(filename, DirNS::SAVE);			
		} else {
			DEBUGLOG("Load from resource");
			jsonString = ResourceLoader::loadFileAsString(filename, DirNS::RESOURCE);			
		}
		
		reader.parse(jsonString, root);		
	} else {
		// load from in-memory filestore
		it = _inMemoryFileStore.find(filename);
		 
		if (it != _inMemoryFileStore.end()) {
			jsonString = it->second;
			reader.parse(jsonString, root);
		}		
	}
	
}

void StateManager::saveStateToFile(Json::Value& root, const std::string& filename) {
    // To-Do: implement save to file    
    
    Json::FastWriter writer;
    std::string jsonString;
    
    jsonString = writer.write(root);
    DEBUGLOG("State saved with data: %s", jsonString.c_str());
    
	if (STATEMANAGER_USE_FILESTORE) {
		// save to file system
		ResourceLoader::writeStringToFile(jsonString, filename, DirNS::SAVE);		
	} else {
		// save to in-memory filestore
		_inMemoryFileStore[filename] = jsonString;		
	}
}

void StateManager::recreateFromState(Json::Value& root) {
    // recreate map
    ObjectBuilder::createMapFromJson(root["map"]);
	
	// recreate context
	SceneContext::instance()->deserialize(root["context"]);
    
    // recreate units
    for (Json::ValueIterator itr = root["units"].begin(); itr != root["units"].end(); itr++) {
        ObjectBuilder::createUnitFromJson(*itr);
    }

    // recreate map objects
    for (Json::ValueIterator itr = root["mapObjects"].begin(); itr != root["mapObjects"].end(); itr++) {
        ObjectBuilder::createMapObjectFromJson(*itr);
    }
	
	// recreate scripted actions
    for (Json::ValueIterator itr = root["scriptedActions"].begin(); itr != root["scriptedActions"].end(); itr++) {
        ScriptManager::instance()->add(ScriptedAction::createScriptedActionFromJson(*itr));
    }

}

