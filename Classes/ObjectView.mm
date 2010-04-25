/*
 *  ObjectView.cpp
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameImage.h"
#include "IDrawable.h"
#include "ObjectView.h"

ObjectView::ObjectView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index) : GameImage(aWidth, aHeight, tex, index){
	_object = NULL;
}

void ObjectView::setObject(IDrawable *object) {
	_object = object;
}

void ObjectView::draw() {
	GPoint pos;
	
	if (_object == NULL) {
		pos = GPointMake(30.0f, 30.0f);
	} else {
		pos = _object->getDrawPosition();
	}
	
	//pos = _object->getDrawPosition();
	//pos = GPointMake(100.0f, 100.0f);
	
	this->drawAt(pos);
}