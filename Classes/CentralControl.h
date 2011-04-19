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

class CentralControl {
	int _mode;
	UnitView* _selectedUnit;
	static CentralControl* _instance;
	InputManager* _input; 
	UnitModel* _unit;
	UnitView* _unitView;
	HexMap* _hexMap;
	
	CentralControl() { }
	CentralControl(InputManager* input, UnitModel* unit, UnitView* unitView, HexMap* hexMap) {
		_mode = 1;
		_input = input;
		_unit = unit; 
		_unitView = unitView; 
		_hexMap = hexMap; 
	}
	
	
public:
	static CentralControl* setup(InputManager* input, UnitModel* unit, UnitView* unitView, HexMap* hexMap) {
		if (_instance == 0) {
			_instance = new CentralControl(input, unit, unitView, hexMap);
		} 
		return _instance;
	}
	
	static CentralControl* instance() {
		return _instance;
	}
	
	void update();
	void draw();
	void handleEventNormal(const TouchEvent& event);
	void handleEventFocus(const TouchEvent& event);

public:
	void switchMode(int mode);
};

