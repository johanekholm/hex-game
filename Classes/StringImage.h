//
//  StringImage.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef STRINGIMAGE_H
#define STRINGIMAGE_H

#include "toolkit.h"
#include <string>

class TextureMap;

class StringImage {
	TextureMap *_texture;
    std::string _string;
    int _size;
    GLfloat _width;
    GLfloat* _vertices;
    GLfloat* _texCoords;
    GLfloat _red, _green, _blue, _alpha;
    RGBA _color;
    
    void buildVertices();

public:
	~StringImage();
	StringImage(const std::string& string, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	StringImage(const std::string& string, const RGBA& color);
	void drawAt(const GPoint &pos);
    void drawCenteredAt(const GPoint &pos);
	void drawAtRotated(const GPoint &pos, GLfloat angle);
};

#endif