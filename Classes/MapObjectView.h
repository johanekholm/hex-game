/*
 *  UnitView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "IObserver.h"
#include "ViewController.h"
#include "MapObject.h"

#include <vector>

//class MapObject;
//struct MapObjectState;
class GameImage;
class ShapeImage;
struct ActionView;
struct ActionState;

class MapObjectView : public ViewController, public IObserver {
	MapObject* _objectModel;
    MapObjectState _state;
	GameImage* _objectImage;
    GameImage* _actionImage;
    ShapeImage *_actionMarker;
    ActionView* _selectedActionView;
    std::vector<ActionView> _actionPoints;


	void drawActions();	
	GPoint getActionPosition(int index);
    ActionState* getTouchedActionState(GPoint point);
    ActionView* getTouchedActionView(GPoint point);
	void updateActions();
	void updatePosition(const MPoint& pos);

public:
	~MapObjectView();
	MapObjectView(MapObject* model, GLfloat width, GLfloat height, int index);
    void destroyed();
	void draw();
	void drawGUI();
	bool handleEvent(const TouchEvent& event);
    void setFocus(bool hasFocus);
	void updateState();
};

