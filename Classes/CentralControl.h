/*
 *  CentralControl.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

class InputManager;
class HexMap;
class ViewController;
class ViewControllerManager;
class ModelManager;
class StringImage;
class ObjectBuilder;

struct TouchEvent;
struct GPoint;

namespace ControlMode {
	const int BATTLE = 1;
	const int BATTLE_FOCUS = 2;
	const int ADVENTURE = 3;
	const int MENU = 4;
};

class CentralControl {
private:
	class PrivateData;
	PrivateData *d;
	CentralControl();

public:
	static CentralControl* instance();
	static void destroy();

	void draw();
	void handleEventAdventureNormal(const TouchEvent& event);
	void handleEventMenu(const TouchEvent& event);
	void handleEventNormal(const TouchEvent& event);
	void handleEventFocus(const TouchEvent& event);
	void init();
	void nextTurn();
	void switchMode(int mode);
	void touchesBegan(const GPoint& touchPoint);
	void touchesMoved(const GPoint& touchPoint);
	void touchesEnded(const GPoint& touchPoint);
	void touchesCancelled(const GPoint& touchPoint);
	void update();
};
