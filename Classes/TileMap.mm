//
//  TileMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-04-12.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "TileMap.h"
#import "TextureMap.h"

TileMap::~TileMap() {
	delete [] _vertices;
	delete [] _texCoords;
}

TileMap::TileMap(int aWidth, int aHeight, GLfloat aTileWidth, GLfloat aTileHeight, TextureMap* tex, vector<int>* tileData) {
	int index = 0, vIndex = 0, tIndex = 0;
	GLfloat tx, ty;

	_width = aWidth;
	_height = aHeight;

	_tileSize = GPointMake(aTileWidth, aTileHeight);
	_texture = tex;

	_numVertices = _width * _height * 6;
	_vertices = new GLfloat[_numVertices * 3]; //(GLfloat *)malloc(_size.x * _size.y * 18 * sizeof(GLfloat));
	_texCoords = new GLfloat[_numVertices * 2];			//(GLfloat *)malloc(widthMap * heightMap * 12 * sizeof(GLfloat));

	
	NSLog(@"w: %i, h: %i, numV: %i", _width, _height, _numVertices);
	
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			tx = j * _tileSize.x;
			ty = i * _tileSize.y;
			_vertices[vIndex] = tx;
			_vertices[vIndex + 1] = ty;
			_vertices[vIndex + 2] = 0;
			_vertices[vIndex + 3] = tx + _tileSize.x;
			_vertices[vIndex + 4] = ty;
			_vertices[vIndex + 5] = 0;
			_vertices[vIndex + 6] = tx;
			_vertices[vIndex + 7] = ty + _tileSize.y;
			_vertices[vIndex + 8] = 0;
			_vertices[vIndex + 9] = tx;
			_vertices[vIndex + 10] = ty + _tileSize.y;
			_vertices[vIndex + 11] = 0;
			_vertices[vIndex + 12] = tx + _tileSize.x;
			_vertices[vIndex + 13] = ty;
			_vertices[vIndex + 14] = 0;
			_vertices[vIndex + 15] = tx + _tileSize.x;
			_vertices[vIndex + 16] = ty + _tileSize.y;
			_vertices[vIndex + 17] = 0;
			
			//NSLog(@"vx: %f, vy: %f, index: %i data_i: %i", _vertices[vIndex + 12], _vertices[vIndex + 13], index, i * _width + j);
			
			//[texture getTriangleTexCoordsForSubWithIndex:0 into:&texCoords[tIndex]];
			_texture->getTriangleTexCoordsForSub(&_texCoords[tIndex], tileData->at(index));
			
			index++; //i * j * 18;
			vIndex = index * 18;
			tIndex = index * 12;
			
		}
	}
	
	//[texture getTriangleTexCoordsForSubWithIndex:0 into:texCoords];
	//[texture getTriangleTexCoordsForSubWithIndex:0 into:&texCoords[12]];
	NSLog(@"tx: %f, ty: %f", _texCoords[14], _texCoords[15]);
	
}

void TileMap::draw() {
	
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	
	//[texture getTriangleTexCoordsForSubWithIndex:texIndex into:texCoords];	
	_texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
	glDrawArrays(GL_TRIANGLES, 0, _numVertices);
	
}

/*
@implementation TileMap

- (void) dealloc {
	[texture release];
	[super dealloc];
}

- (id) initWithMapWidth: (int)width andMapHeight: (int)height withTileSize: (CGSize)newTileSize andTexture: (TextureMap*)texMap {
	int index = 0, vIndex = 0, tIndex = 0;
	GLfloat tx, ty;
	
	self = [super init];
	if (self != nil) {
		//mapSize = newMapSize; //CGSizeMake(newMapSize.width, newMapSize.height);

		widthMap = width;
		heightMap = height;
		numVertices = widthMap * heightMap * 6;
		
		tileSize = newTileSize; //CGSizeMake(newTileSize.width, newTileSize.height);;
		position = CGPointMake(0.0f, 0.0f);
		
		texture = [texMap retain];
		
		vertices = (GLfloat *)malloc(widthMap * heightMap * 18 * sizeof(GLfloat));
		texCoords = (GLfloat *)malloc(widthMap * heightMap * 12 * sizeof(GLfloat));
		
		NSLog(@"w: %i, h: %i, numV: %i", widthMap, heightMap, numVertices);
		
		for (int i = 0; i < heightMap; i++) {
			for (int j = 0; j < widthMap; j++) {
				tx = j * tileSize.width;
				ty = i * tileSize.height;
				vertices[vIndex] = tx;
				vertices[vIndex + 1] = ty;
				vertices[vIndex + 2] = 0;
				vertices[vIndex + 3] = tx + tileSize.width;
				vertices[vIndex + 4] = ty;
				vertices[vIndex + 5] = 0;
				vertices[vIndex + 6] = tx;
				vertices[vIndex + 7] = ty + tileSize.height;
				vertices[vIndex + 8] = 0;
				vertices[vIndex + 9] = tx;
				vertices[vIndex + 10] = ty + tileSize.height;
				vertices[vIndex + 11] = 0;
				vertices[vIndex + 12] = tx + tileSize.width;
				vertices[vIndex + 13] = ty;
				vertices[vIndex + 14] = 0;
				vertices[vIndex + 15] = tx + tileSize.width;
				vertices[vIndex + 16] = ty + tileSize.height;
				vertices[vIndex + 17] = 0;
				
				[texture getTriangleTexCoordsForSubWithIndex:0 into:&texCoords[tIndex]];
				
				NSLog(@"vx: %f, vy: %f, index: %i", vertices[vIndex + 12], vertices[vIndex + 13], index);
				index += 1; //i * j * 18;
				vIndex = index * 18;
				tIndex = index * 12;

			}
		}
		
		//[texture getTriangleTexCoordsForSubWithIndex:0 into:texCoords];
		//[texture getTriangleTexCoordsForSubWithIndex:0 into:&texCoords[12]];
		NSLog(@"tx: %f, ty: %f", texCoords[14], texCoords[15]);
	}
	
	
	return self;
}

- (void) draw {
	glLoadIdentity();
	glTranslatef(100.0f, 100.0f, 0.0f);
	
	//[texture getTriangleTexCoordsForSubWithIndex:texIndex into:texCoords];	
	[texture bind];
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	
}

@end
*/