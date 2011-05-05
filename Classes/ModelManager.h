/*
 *  ModelManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "toolkit.h"

class UnitModel;

class ModelManager {
    static ModelManager* _instance;
	std::vector<UnitModel*> _units;
	
    ModelManager();

public:
    static ModelManager* instance() {
		if (_instance == 0) {
			_instance = new ModelManager();
		}		
		return _instance;
	}
    
    static void destroy();

	void add(UnitModel*);
    void tick();
    UnitModel* getUnitAtPos(const MPoint& pos);
};

