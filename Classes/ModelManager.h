/*
 *  ModelManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-05-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
class UnitModel;

class ModelManager {
	std::vector<UnitModel*> _units;
	
public:
	ModelManager();
	~ModelManager();
	void add(UnitModel*);
    void tick();
};

