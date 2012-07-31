/*
 *  SceneContext.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneContext.h"
#include "Item.h"
#include "json.h"
#include <iostream>

class SceneContext::PrivateData {
public:
	static SceneContext* instance;
	int partyId1;	
	int partyId2;
	ItemHandler itemStack;
};

SceneContext* SceneContext::PrivateData::instance = 0;

SceneContext* SceneContext::instance() {
    if (PrivateData::instance == 0) {
        PrivateData::instance = new SceneContext();
    } 
    return PrivateData::instance;
}

void SceneContext::destroy() {
	if (PrivateData::instance != 0) {
        
		delete PrivateData::instance;
		PrivateData::instance=0;
	}
}

SceneContext::SceneContext() {
	d = new PrivateData();
	d->partyId1 = 0;
	d->partyId2 = 0;
}

Json::Value SceneContext::serialize() {
    Json::Value root;
    
    root["partyId1"] = d->partyId1;
    root["partyId2"] = d->partyId2;
	root["items"] = d->itemStack.serializeItems();
    
    return root;
}

void SceneContext::deserialize(Json::Value& root) {
    d->partyId1 = root.get("partyId1", 0).asInt();
    d->partyId2 = root.get("partyId2", 0).asInt();
	d->itemStack.deserializeItems(root["items"]);
}

ItemHandler* SceneContext::getItemStack() {
	return &(d->itemStack);
}

void SceneContext::setPartyId1(int partyId) {
	d->partyId1 = partyId;
}

void SceneContext::setPartyId2(int partyId) {
	d->partyId2 = partyId;	
}

int SceneContext::getPartyId1() {
	return d->partyId1;
}

int SceneContext::getPartyId2() {
	return d->partyId2;	
}
