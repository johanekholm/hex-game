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
#include "geometry.h"
#include <iostream>

ModelManager* ModelManager::_instance = 0;

void ModelManager::destroy() {
	if (_instance != 0) {
		_instance->_units.clear();
        delete _instance->_map;
		delete _instance;
		_instance=0;
	}
}

ModelManager::ModelManager() {
    
}

void ModelManager::add(UnitModel* unit) {
    if (unit == 0) {
        return;
    }
    
    _unitIdCounter++;
    _units[_unitIdCounter] = unit;
    unit->setId(_unitIdCounter);
}


void ModelManager::remove(int unitId) {
    delete _units[unitId];
    _units.erase(unitId);

    // "soft" deletion does not disturb iterators, leave pointer in map but set it to 0
    //_units[unitId] = 0;
}
    
void ModelManager::setMap(HexMapModel* map) {
    _map = map;
}

HexMapModel* ModelManager::getMap() {
    return _map;
}

void ModelManager::tick() {
    std::map<int, UnitModel*>::iterator it, next;
    
    std::cout << "--- Tick ---" << std::endl;
	for (it = _units.begin(); it != _units.end();) {
        //std::cout << "Iterator pointer: " << *it << std::endl;
        
        // unit can destroy itself thus corrupting the iterator, therefore another iterator the next item is needed
        next = it;
        next++;
        
        if (it->second != 0) {
            it->second->tick();
            it = next;
        } else {
            _units.erase(it);
            it = next;
        }
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
                std::cout << "Unit found at distance: " << minDistance << std::endl;
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
    std::cout << "Closest enemy: " << minDistance << std::endl;
    return minDistance;
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
