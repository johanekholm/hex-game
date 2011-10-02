/*
 *  MapObjectView.h
 *
 *  Created by Johan Ekholm.
 *
 */

#include "IObserver.h"
#include "ViewController.h"
#include "MapObject.h"
#include "BaseUnitViewController.h"
#include <vector>

class GameImage;
class ShapeImage;
struct ActionView;
struct ActionState;

class MapObjectView : public BaseUnitViewController {
	MapObject* _objectModel;
    MapObjectState _state;
	GameImage* _objectImage;

public:
	~MapObjectView();
	MapObjectView(MapObject* model, GLfloat width, GLfloat height, int index);
	void draw(const GPoint& cameraPos);
	bool handleEvent(const TouchEvent& event);
	void updateState();
};

