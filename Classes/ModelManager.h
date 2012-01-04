/*
 *  ModelManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <map>
#include "toolkit.h"

class UnitModel;
class HexMapModel;
class MapObject;

class ModelManager {
    static ModelManager* _instance;
	std::map<int, UnitModel*> _units;
	std::map<int, MapObject*> _mapObjects;
    HexMapModel *_battleMap, *_adventureMap;
    int _unitIdCounter, _objectIdCounter;
    
    ModelManager();

public:
    static ModelManager* instance() {
		if (_instance == 0) {
			_instance = new ModelManager();
		}		
		return _instance;
	}
    
    static void destroy();

    void addMapObject(MapObject* object);
	void addUnit(UnitModel*);
    HexMapModel* getAdventureMap();    
    std::vector<MapObject*> getAllMapObjects();
    std::vector<UnitModel*> getAllUnits();
    HexMapModel* getBattleMap();
    UnitModel* getClosestTo(const MPoint& pos);
    int getDistanceToClosestEnemy(int owner, const MPoint& pos);
    MapObject* getMapObjectAtPos(const MPoint& pos);
    int getOwnerWithNoUnits();
    UnitModel* getUnitAtPos(const MPoint& pos);
    UnitModel* getUnitById(int unitId);
    bool mapObjectExistAtPos(int category, const MPoint& pos);
    void removeAllMapObjects();
    void removeAllUnits();
    void removeMapObject(int objectId);
    void removeUnit(int unitId);
    void setAdventureMap(HexMapModel* map);
    void setBattleMap(HexMapModel* map);
    void tick();
};

