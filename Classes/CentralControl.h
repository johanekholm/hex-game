/*
 *  CentralControl.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

class InputManager;
class UnitView;
class UnitModel;
class HexMap;
class ViewController;
class ViewControllerManager;
class ModelManager;

struct TouchEvent;
struct GPoint;

class CentralControl {
	static CentralControl* _instance;
	int _mode;
	ViewController* _selectedViewController;
	InputManager* _input; 
    ModelManager* _modelManager;
    ViewControllerManager* _viewControllerManager;
	HexMap* _hexMap;

    //~CentralControl();
    CentralControl();

public:
	static CentralControl* instance() {
		if (_instance == 0) {
			_instance = new CentralControl();
		} 
		return _instance;
	}
    
    static void destroy();
    
    
	
	void update();
	void draw();
	void handleEventNormal(const TouchEvent& event);
	void handleEventFocus(const TouchEvent& event);

	void touchesBegan(const GPoint& touchPoint);
	void touchesMoved(const GPoint& touchPoint);
	void touchesEnded(const GPoint& touchPoint);
	void touchesCancelled(const GPoint& touchPoint);
		
public:
	void switchMode(int mode);
};

