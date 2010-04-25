//
//  GameImage.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES1/gl.h>
#include "toolkit.h"

class TextureMap;

class GameImage {
	//GLfloat x, y;
	GPoint position;
	GLfloat width, height;
	TextureMap *texture;
	int texIndex;

public:
	GameImage(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	//~GameImageCPP();
	void draw();
	void drawAt(const GPoint &pos);
	void setPosition(GPoint point);
};

