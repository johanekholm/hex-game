/*
 *  ViewControllerManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
class ViewController;
struct GPoint;

class ViewControllerManager {
	std::vector<ViewController*> _views;
	
public:
	ViewControllerManager();
	~ViewControllerManager();
    void draw();
    ViewController* getTouched(const GPoint& point);
	
};

