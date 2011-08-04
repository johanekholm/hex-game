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
    GLfloat* _vertices;
    GLfloat* _texCoords;

public:
	~StringImage();
	StringImage(const std::string& string);
	void drawAt(const GPoint &pos);
	void drawAtRotated(const GPoint &pos, GLfloat angle);
};

#endif