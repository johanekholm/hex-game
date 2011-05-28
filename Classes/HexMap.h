//
//  HexMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef HEXMAP_H
#define HEXMAP_H

#define HEX_WIDTH 64.0f
#define HEX_HALF_WIDTH HEX_WIDTH / 2.0f
#define HEX_HEIGHT 36.0f
#define HEX_HALF_HEIGHT HEX_HEIGHT / 2.0f
#define HEX_POINTINESS 14.0f
#define HEX_TOTAL_HEIGHT HEX_HEIGHT + HEX_POINTINESS * 2.0f

#define HEX_TEX_Y1 HEX_POINTINESS / (HEX_TOTAL_HEIGHT)
#define HEX_TEX_Y2 1.0f - HEX_TEX_Y1


#include <vector>
//#import <Foundation/Foundation.h>
//#import "TextureMap.h"

#include "toolkit.h"

using namespace std;

class TextureMap;

class HexMap {
	int _width, _height;
	GPoint _tileSize;
	TextureMap *_texture;
	GLfloat *_vertices;
	GLfloat *_texCoords;
	int _numVertices;
	
public:
	~HexMap();
	HexMap(TextureMap* tex, int aWidth, int aHeight, GLfloat aHexWidth, GLfloat aHexHeight);  //int aWidth, int aHeight, GLfloat aHexWidth, GLfloat aHexHeight, TextureMap* tex, vector<int>* tileData);
	//~GameImageCPP();
	void draw();
    std::vector<MPoint> getAllHexes();
	//void drawAt(const GPoint &pos);
	//void setPosition(GPoint point);
};


#endif
