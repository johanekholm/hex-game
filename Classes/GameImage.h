//
//  GameImage.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "toolkit.h"

class TextureMap;

class GameImage {
	//GLfloat x, y;
	//GPoint position;
	TextureMap *texture;
	int texIndex;

protected:
	GLfloat width, height;

public:
	GameImage(GLfloat aWidth, GLfloat aHeight, TextureMap* tex, int index);
	//~GameImageCPP();
	void draw();
	void drawAt(const GPoint &pos);
	void drawAtWithSubTexture(const GPoint &pos, int subTexture);
	void drawAtRotatedWithSubTexture(const GPoint &pos, GLfloat angle, int subTexture);
	void setPosition(GPoint point);
};

