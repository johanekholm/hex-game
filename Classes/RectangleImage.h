//
//  GameImage.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef RECTANGLEIMAGE_H
#define RECTANGLEIMAGE_H

#include "toolkit.h"

class TextureMap;

class RectangleImage {

protected:
	GLfloat _width, _height;
    GLfloat _red;
    GLfloat _green;
    GLfloat _blue;
    GLfloat _alpha;


public:
    RectangleImage(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void drawAt(const GPoint &pos);
    void drawAtWithSize(const GPoint &pos, GLfloat width, GLfloat height);
};

#endif