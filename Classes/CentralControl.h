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
struct TouchEvent;
struct GPoint;

class CentralControl {
	int _mode;
	UnitView* _selectedUnit;
	static CentralControl* _instance;
	InputManager* _input; 
	UnitModel* _unit;
	UnitView* _unitView;
	HexMap* _hexMap;
	
public:
	CentralControl();
	static CentralControl* instance() {
		if (_instance == 0) {
			_instance = new CentralControl();
		} 
		return _instance;
	}
	
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

