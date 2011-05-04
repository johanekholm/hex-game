/*
 *  ModelManager.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ModelManager.h"
#include "UnitModel.h"

ModelManager::ModelManager() {

}

ModelManager::~ModelManager() {
	_units.clear();	
}

void ModelManager::add(UnitModel* unit) {
	_units.push_back(unit);
}

void ModelManager::tick() {
	for (std::vector<UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		(*it)->tick();
	}
}

