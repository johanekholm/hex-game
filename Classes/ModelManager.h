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
    HexMapModel *_map;
    int _unitIdCounter, _objectIdCounter;
    bool _abortMapObjectIteration;
	
    ModelManager();

public:
    static ModelManager* instance();
    static void destroy();

    void addMapObject(MapObject* object);
	void addUnit(UnitModel* unit);
	
    void deleteAllMapObjects();
    void deleteAllUnits();
    void deleteMapObject(int objectId);
    void deleteUnit(int unitId);

	void doTurn();

    std::vector<MapObject*> getAllMapObjects();
    std::vector<UnitModel*> getAllUnits();
    UnitModel* getClosestTo(const MPoint& pos);
    int getDistanceToClosestEnemy(int owner, const MPoint& pos);
    MapObject* getFirstMapObjectWithOwner(int owner);
    HexMapModel* getMap();    
    MapObject* getMapObjectAtPos(const MPoint& pos);
    MapObject* getMapObjectById(int objectId);
    int getOwnerWithNoUnits();
    UnitModel* getUnitAtPos(const MPoint& pos);
    UnitModel* getUnitById(int unitId);
    bool mapObjectExistAtPos(int category, const MPoint& pos);

    void setMap(HexMapModel* map);
    void tick();

    std::vector<UnitModel*> unregisterAllUnits();
	MapObject* unregisterMapObject(int objectId);
};

