//
//  HexMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "HexMap.h"
#import "TextureMap.h"



//GLfloat* getHexGeometry() {
//}




HexMap::~HexMap() {
	delete [] _vertices;
	delete [] _texCoords;
}

HexMap::HexMap(TextureMap* tex) {	//int aWidth, int aHeight, GLfloat aHexWidth, GLfloat aHexHeight, TextureMap* tex, vector<int>* tileData) {
	//int index = 0, vIndex = 0, tIndex = 0;
	/* GLfloat tx, ty;

	_width = aWidth;
	_height = aHeight;

	_tileSize = GPointMake(aHexWidth, aHexHeight);
	 */
	_texture = tex;

	_numVertices = 3*4; //_width * _height * 6;
	_vertices = new GLfloat[_numVertices * 3]; //(GLfloat *)malloc(_size.x * _size.y * 18 * sizeof(GLfloat));
	_texCoords = new GLfloat[_numVertices * 2];			//(GLfloat *)malloc(widthMap * heightMap * 12 * sizeof(GLfloat));

	
	NSLog(@"w: %i, h: %i, numV: %i", _width, _height, _numVertices);
	
	
	GLfloat vertices[4*3*3] = { -HEX_HALF_WIDTH, -HEX_HALF_HEIGHT, 0, // top triangle
		0, -HEX_HALF_HEIGHT - HEX_POINTINESS, 0,
		HEX_HALF_WIDTH, -HEX_HALF_HEIGHT, 0,
		-HEX_HALF_WIDTH, -HEX_HALF_HEIGHT, 0, // top half of square
		HEX_HALF_WIDTH, -HEX_HALF_HEIGHT, 0,
		HEX_HALF_WIDTH, HEX_HALF_HEIGHT, 0,
		-HEX_HALF_WIDTH, -HEX_HALF_HEIGHT, 0, // bottom half of square
		HEX_HALF_WIDTH, HEX_HALF_HEIGHT, 0,
		-HEX_HALF_WIDTH, HEX_HALF_HEIGHT, 0,
		-HEX_HALF_WIDTH, HEX_HALF_HEIGHT, 0, // bottom triangle
		HEX_HALF_WIDTH, HEX_HALF_HEIGHT, 0,
		0, HEX_HALF_HEIGHT + HEX_POINTINESS, 0
	};
	
/*	GLfloat texCoords[4*3*2] = { 0.0f, HEX_TEX_Y1, // top triangle
		0.5f, 0.0f,
		1.0f, HEX_TEX_Y1,
		0.0f, HEX_TEX_Y1, // top half of square
		1.0f, HEX_TEX_Y1,
		1.0f, HEX_TEX_Y2,
		0.0f, HEX_TEX_Y1, // bottom half of square
		1.0f, HEX_TEX_Y2,
		0.0f, HEX_TEX_Y2,
		0.0f, HEX_TEX_Y2, // bottom triangle
		1.0f, HEX_TEX_Y2,
		0.5f, 1.0f
	};

	for (int i = 0; i < 4*3*2; i++) {
		texCoords[i] = texCoords[i] / 2.0f;
	}
*/	
	
	memcpy(_vertices, vertices, sizeof(vertices));
	//memcpy(_texCoords, texCoords, sizeof(texCoords));
	
	_texture->getHexTexCoordsForSub(_texCoords, 1, HEX_TEX_Y1); //tileData->at(index));

	//NSLog(@"sq1: %i, h: %i, numV: %i", _width, _height, _numVertices);
	NSLog(@"sq1 - x: %f, y: %f", _texCoords[6], _texCoords[7]);
	NSLog(@"sq2 - x: %f, y: %f", _texCoords[8], _texCoords[9]);
	NSLog(@"sq3 - x: %f, y: %f", _texCoords[10], _texCoords[11]);
	NSLog(@"sq4 - x: %f, y: %f", _texCoords[16], _texCoords[17]);

	/*int mapData[] = { 0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0  };
	
	int tx, ty, odd, i;
	GLfloat px, py;
	*/
	
	
	/*
	for (i = 0; i < 16; i++) {
		tx = i % 4;
		ty = i / 4;
		even = ty % 2;
		px = tx * HEX_WIDTH + even * HEX_HALF_WIDTH;
		py = ty * HEX_HEIGHT;
		glTranslatef(px, py, 0);
	}*/
	

}

void HexMap::draw() {
	
//	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(64.0f, 64.0f, 0.0f);
	
	_texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
	glDrawArrays(GL_TRIANGLES, 0, _numVertices);
//	glEnable(GL_TEXTURE_2D);	

}

