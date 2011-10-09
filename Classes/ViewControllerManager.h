/*
 *  ViewControllerManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "toolkit.h"
class ViewController;
class HexMap;
class ShapeImage;

class ViewControllerManager {
    static ViewControllerManager* _instance;
	std::vector<ViewController*> _views;
    HexMap *_mapView, *_pushedMapView;
    ViewController* _focus;
    GPoint _cameraPos;
    ShapeImage *_hudBackground;

    ViewControllerManager();

public:
    static ViewControllerManager* instance() {
		if (_instance == 0) {
			_instance = new ViewControllerManager();
		}		
		return _instance;
	}
    
    static void destroy();

	void add(ViewController* view);
    GPoint adjustForCamera(const GPoint& pos);
    void centerCamera(const GPoint& pos);
    void draw();
    void drawGUI();
    void drawMap();
    ViewController* getFocus();
    ViewController* getTouched(const GPoint& point);	
    void moveCamera(const GPoint& pos);
    void popMapView();
    void purge();
    void pushMapView(HexMap* mapView);
    void remove(ViewController* view);
    void removeSoft(ViewController* view);
    void setCameraPosition(const GPoint& pos);
    void setFocus(ViewController* view);
    void setMapView(HexMap* mapView);
    GPoint transformModelPositionToView(const MPoint& pos);
    void translateToCamera();
    void translateToCameraAndPosition(const GPoint& pos);
    void update();
};

