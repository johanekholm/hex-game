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
    static ViewControllerManager* _instance;
	std::vector<ViewController*> _views;

    ViewControllerManager();

public:
    static ViewControllerManager* instance() {
		if (_instance == 0) {
			_instance = new ViewControllerManager();
		}		
		return _instance;
	}
    
    static void destroy();

	//~ViewControllerManager();
	void add(ViewController* view);
    void remove(ViewController* view);
    void draw();
    void drawGUI();
    ViewController* getTouched(const GPoint& point);	
};

