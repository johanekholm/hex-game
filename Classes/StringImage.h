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
#include <vector>

class TextureMap;

class StringImage {
	TextureMap *_texture;
    std::string _string;
    int _size;
    GLfloat _width, _height;
    GLfloat* _vertices;
    GLfloat* _texCoords;
    GLfloat _red, _green, _blue, _alpha;
    RGBA _color;
    
    void buildVertices();

public:
	static std::vector<int> _fontPixelWidths;
	static std::vector<int> initFontPixelWidths();
	
	~StringImage();
	StringImage(const std::string& string, const RGBA& color);
	void drawAt(const GPoint &pos);
    void drawCenteredAt(const GPoint &pos);
	void drawAtRotated(const GPoint &pos, GLfloat angle);
};

class MultiRowStringImage {
protected:
	std::vector<StringImage*> _strings;
	std::string _string;
    int _rowLength;
	int _numRows;
    RGBA _color;
	GPoint _rowOffset;
	
public:
	~MultiRowStringImage();
	MultiRowStringImage(const std::string& string, const RGBA& color, GLfloat rowLength, GLfloat rowOffset);
	void drawAt(const GPoint &pos);
    void drawCenteredAt(const GPoint &pos);	
};

#endif