//
//  TileMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-04-12.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TextureMap.h"

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
