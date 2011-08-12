//
//  HexMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "HexMap.h"
#include "TextureMap.h"
#include "HexMapModel.h"

HexMap::~HexMap() {
	delete [] _vertices;
	delete [] _texCoords;
}

HexMap::HexMap(HexMapModel* model, TextureMap* tex, GLfloat scale, int aWidth, int aHeight) {
	int index = 0, vIndex = 0, tIndex = 0;
	GLfloat tx, ty;

	_width = aWidth;
	_height = aHeight;

	_texture = tex;

	_numVertices = _width * _height * 3*4;
	_vertices = new GLfloat[_numVertices * 3];
	_texCoords = new GLfloat[_numVertices * 2];

	
	//NSLog(@"w: %i, h: %i, numV: %i", _width, _height, _numVertices);
	
	
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
	
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			tx = j * (HEX_WIDTH + 0.0f) + (i % 2) * HEX_WIDTH / 2;
			ty = i * (HEX_HEIGHT + HEX_POINTINESS);
			
			_vertices[vIndex + 0] = vertices[0] + tx;
			_vertices[vIndex + 3] = vertices[3] + tx;
			_vertices[vIndex + 6] = vertices[6] + tx;
			_vertices[vIndex + 9] = vertices[9] + tx;
			_vertices[vIndex + 12] = vertices[12] + tx;
			_vertices[vIndex + 15] = vertices[15] + tx;
			_vertices[vIndex + 18] = vertices[18] + tx;
			_vertices[vIndex + 21] = vertices[21] + tx;
			_vertices[vIndex + 24] = vertices[24] + tx;
			_vertices[vIndex + 27] = vertices[27] + tx;
			_vertices[vIndex + 30] = vertices[30] + tx;
			_vertices[vIndex + 33] = vertices[33] + tx;

			_vertices[vIndex + 1] = vertices[1] + ty;
			_vertices[vIndex + 4] = vertices[4] + ty;
			_vertices[vIndex + 7] = vertices[7] + ty;
			_vertices[vIndex + 10] = vertices[10] + ty;
			_vertices[vIndex + 13] = vertices[13] + ty;
			_vertices[vIndex + 16] = vertices[16] + ty;
			_vertices[vIndex + 19] = vertices[19] + ty;
			_vertices[vIndex + 22] = vertices[22] + ty;
			_vertices[vIndex + 25] = vertices[25] + ty;
			_vertices[vIndex + 28] = vertices[28] + ty;
			_vertices[vIndex + 31] = vertices[31] + ty;
			_vertices[vIndex + 34] = vertices[34] + ty;

			_vertices[vIndex + 2] = 0;
			_vertices[vIndex + 5] = 0;
			_vertices[vIndex + 8] = 0;
			_vertices[vIndex + 11] = 0;
			_vertices[vIndex + 14] = 0;
			_vertices[vIndex + 17] = 0;
			_vertices[vIndex + 20] = 0;
			_vertices[vIndex + 23] = 0;
			_vertices[vIndex + 26] = 0;
			_vertices[vIndex + 29] = 0;
			_vertices[vIndex + 32] = 0;
			_vertices[vIndex + 35] = 0;

			_texture->getHexTexCoordsForSub(&_texCoords[tIndex], model->getHexValue(j, i), HEX_TEX_Y1);
			
			index++;
			vIndex = index * 36;
			tIndex = index * 24;
		}
	}
	
	
	_texture->getHexTexCoordsForSub(_texCoords, 1, HEX_TEX_Y1); //tileData->at(index));

	//NSLog(@"sq1: %i, h: %i, numV: %i", _width, _height, _numVertices);
	//NSLog(@"sq1 - x: %f, y: %f", _texCoords[6], _texCoords[7]);
	//NSLog(@"sq2 - x: %f, y: %f", _texCoords[8], _texCoords[9]);
	//NSLog(@"sq3 - x: %f, y: %f", _texCoords[10], _texCoords[11]);
	//NSLog(@"sq4 - x: %f, y: %f", _texCoords[16], _texCoords[17]);


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

std::vector<MPoint> HexMap::getAllHexes() {
    std::vector<MPoint> v;
    
    for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
            v.push_back(MPointMake(j, i));
        }
    }
    return v;
}

