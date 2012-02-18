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
#include "HexMapModel.h"
#include "MapObject.h"
#include "EventManager.h"
#include "geometry.h"
#include <iostream>

ModelManager* ModelManager::_instance = 0;

ModelManager* ModelManager::instance() {
    if (_instance == 0) {
        _instance = new ModelManager();
    }		
    return _instance;
}

void ModelManager::destroy() {
	if (_instance != 0) {
       	for (std::map<int, MapObject*>::iterator it = _instance->_mapObjects.begin(); it != _instance->_mapObjects.end(); ++it) {
            delete it->second;
        }
		_instance->_mapObjects.clear();

       	for (std::map<int, UnitModel*>::iterator it = _instance->_units.begin(); it != _instance->_units.end(); ++it) {
            delete it->second;
        }
		_instance->_units.clear();
        
        delete _instance->_map;
		delete _instance;
		_instance=0;
	}
}

ModelManager::ModelManager() {
    _map = 0;
	_unitIdCounter = 0;
    _objectIdCounter = 0;
	_abortMapObjectIteration = false;
}

void ModelManager::addMapObject(MapObject* object) {
	int objectId;
	
    if (object == 0) {
        return;
    }
    
	objectId = object->getId();

    if (objectId > 0) {
		if (_objectIdCounter < objectId) {
			_objectIdCounter = objectId;
		}
		
		_mapObjects[objectId] = object;
	} else {
		_objectIdCounter++;
		_mapObjects[_objectIdCounter] = object;
		object->setId(_objectIdCounter);
	}
}

void ModelManager::addUnit(UnitModel* unit) {
    if (unit == 0) {
        return;
    }
    
    _unitIdCounter++;
    _units[_unitIdCounter] = unit;
    unit->setId(_unitIdCounter);
}

HexMapModel* ModelManager::getMap() {
    return _map;
}

std::vector<MapObject*> ModelManager::getAllMapObjects() {
    std::vector<MapObject*> objectVector;
    
    for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end(); ++it) {
		if (it->second != 0) {
            objectVector.push_back(it->second);
        }
	}
    
    return objectVector;    
}

std::vector<UnitModel*> ModelManager::getAllUnits() {
    std::vector<UnitModel*> unitVector;
    
    for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
            unitVector.push_back(it->second);
        }
	}
    
    return unitVector;
}

UnitModel* ModelManager::getClosestTo(const MPoint& pos) {
    int distance = 0;
    int minDistance = 99;
    UnitModel* closestUnit = 0;
    
	for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
            distance = hexDistance(pos, it->second->getPosition());
            if (it->second->getOwner() == 1 && distance < minDistance) {
                minDistance = distance;
                closestUnit = it->second;
            }            
        }
	}
    return closestUnit;
}

int ModelManager::getDistanceToClosestEnemy(int owner, const MPoint& pos) {
    int distance = 0;
    int minDistance = 99;
    
	for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0 && it->second->getOwner() != owner) {
            distance = hexDistance(pos, it->second->getPosition());
            if (distance < minDistance) {
                minDistance = distance;
            }            
        }
	}
    return minDistance;
}

MapObject* ModelManager::getMapObjectById(int mapObjectId) {
    std::map<int, MapObject*>::iterator it;
    
    it = _mapObjects.find(mapObjectId);
    
    if (it != _mapObjects.end()) {
        return it->second;
    } else {
        return 0;
    }
}

MapObject* ModelManager::getMapObjectAtPos(const MPoint& pos) {
	for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end(); ++it) {
		if (it->second != 0) {
            if (it->second->getPosition() == pos) {
                return it->second;
            }
        }
	}
    return 0;
}

MapObject* ModelManager::getFirstMapObjectWithOwner(int owner) {
	for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end(); ++it) {
		if (it->second != 0) {
            if (it->second->getOwner() == owner) {
                return it->second;
            }
        }
	}
    return 0;	
}


int ModelManager::getOwnerWithNoUnits() {
    bool owner1 = false;
    bool owner2 = false;
    
    for (std::map<int, UnitModel*>::const_iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
            if (it->second->getOwner() == 1) {
                owner1 = true;
            } else if (it->second->getOwner() == 2) {
                owner2 = true;
            }
        }
	}
    
    if (!owner1) {
        return 1;
    } else if (!owner2) {
        return 2;
    } else {
        return 0;
    }
}

UnitModel* ModelManager::getUnitAtPos(const MPoint& pos) {
	for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
            if (it->second->getPosition() == pos) {
                return it->second;
            }
        }
	}
    return 0;
}

UnitModel* ModelManager::getUnitById(int unitId) {
    std::map<int, UnitModel*>::iterator it;
    
    it = _units.find(unitId);
    
    if (it != _units.end()) {
        return it->second;
    } else {
        return 0;
    }
}

bool ModelManager::mapObjectExistAtPos(int category, const MPoint& pos) {
	for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end(); ++it) {
		if (it->second != 0 && it->second->getPosition() == pos && it->second->matchesCategory(category)) {
            return true;
        }
	}
    return false;
}


void ModelManager::deleteAllMapObjects() {
    for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end(); ++it) {
		delete it->second;
	}
    _mapObjects.clear();
	_abortMapObjectIteration = true;
}

void ModelManager::deleteAllUnits() {
    for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		delete it->second;
	}
	_units.clear();
}

std::vector<UnitModel*> ModelManager::removeAllUnits() {
	std::vector<UnitModel*> unitVector;
	
    for (std::map<int, UnitModel*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
			it->second->updateObserversDestroyed();
            unitVector.push_back(it->second);
        }
	}
	_units.clear();
	return unitVector;
}


void ModelManager::removeMapObject(int objectId) {
    delete _mapObjects[objectId];
    _mapObjects.erase(objectId);
}

void ModelManager::removeUnit(int unitId) {
    int owner = _units[unitId]->getOwner();
    
    delete _units[unitId];
    _units.erase(unitId);

    // check for wipe-out
    for (std::map<int, UnitModel*>::const_iterator it = _units.begin(); it != _units.end(); ++it) {
		if (it->second != 0) {
            if (it->second->getOwner() == owner) {
                return;
            }
        }
	}
    
    ModelEvent event;
    event.type = ModelEventNS::PARTY_WIPEOUT;
    
    EventManager::instance()->publishEvent(event);
}

void ModelManager::setMap(HexMapModel* map) {
    delete _map;
    _map = map;
}

void ModelManager::tick() {
    std::map<int, UnitModel*>::iterator it, next;
    
	for (it = _units.begin(); it != _units.end();) {
        if (it->second != 0) {
            it->second->tick();
            it++;
        } else {
            _units.erase(it);
        }
	}
}

void ModelManager::doTurn() {
	_abortMapObjectIteration = false;
	
	for (std::map<int, MapObject*>::iterator it = _mapObjects.begin(); it != _mapObjects.end() && !_abortMapObjectIteration; it++) {
		it->second->doTurn();
	}
}

MapObject* ModelManager::unregisterMapObject(int objectId) {
	//_mapObjects[objectId]->updateObserversDestroyed();
	_mapObjects.erase(objectId);
}
