//
//  TileMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-12.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
//#import <Foundation/Foundation.h>
//#import "TextureMap.h"

#include "toolkit.h"

using namespace std;

class TextureMap;

class TileMap {
	int _width, _height;
	GPoint _tileSize;
	TextureMap *_texture;
	GLfloat *_vertices;
	GLfloat *_texCoords;
	int _numVertices;
	
public:
	~TileMap();
	TileMap(int aWidth, int aHeight, GLfloat aTileWidth, GLfloat aTileHeight, TextureMap* tex, vector<int>* tileData);
	//~GameImageCPP();
	void draw();
	//void drawAt(const GPoint &pos);
	//void setPosition(GPoint point);
};


/*
@interface TileMap : NSObject {
	CGPoint position;
	//CGSize mapSize; 
	int widthMap, heightMap;
	int numVertices;
	CGSize tileSize;
	TextureMap *texture;
	NSArray *map;
	GLfloat *vertices;
	GLfloat *texCoords;
	
}

- (id) initWithMapWidth: (int)width andMapHeight: (int)height withTileSize: (CGSize)newTileSize andTexture: (TextureMap*)texMap;
- (void) draw;

@end
*/

#endif