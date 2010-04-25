/*
 *  ObjectView.h
 *  igame
 *
 *  Created by Johan Ekholm on 2010-04-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

class IDrawable;
class GameImage;

class ObjectView : public GameImage {
	IDrawable *_object;
	
public:
	ObjectView(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	void setObject(IDrawable *object);
	void draw();
};