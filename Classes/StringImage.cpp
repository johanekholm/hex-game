//
//  GameImage.mm
//  igame
//
//  Created by Johan Ekholm on 2010-04-11.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "StringImage.h"
#include "TextureMap.h"
#include "TextureCatalog.h"
#include <iostream>

StringImage::~StringImage() {
    delete [] _texCoords;
    delete [] _vertices;
}

StringImage::StringImage(const std::string& string, const RGBA& color) {
    _color = color;
    _string = string;
    _size = _string.length();
    _texture = TextureCatalog::instance()->get("font");

    this->buildVertices();
}

void StringImage::buildVertices() {
    int character;
    GLfloat caret = 0.0f;
    GLfloat scale = 0.5f;
    GLfloat height = 29.0f;
    GLfloat width = 32.0f;
    GLfloat texWidth = 0.06f;
    GLfloat texHeight = 0.07617f;
    GLfloat tx = 0.04f;
    GLfloat ty = 0.0f;
    int charWidthsPixels[] = {10,10,14,22,18,30,24,6,12,12,16,24,10,12,8,14,   18,16,17,16,18,17,17,17,17,17, 12,12,18,26,18,   
        16,26,  20,20,20,20,20,18,20,20,12,14,  20,18,22,20,22,20, 24,20,18,20,20,20,26,
        20,19,18};
    int charRow[] = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,
        2,2,2 };
    GLfloat charWidths[64];
    GLfloat charTexPos[64];
    GLfloat charTexPosY[64];
    int texRow;
    const char* cString;
    
    cString = _string.c_str();
    
    _texCoords = new GLfloat [_size*12];    
    _vertices = new GLfloat [_size*18];
    
    charTexPos[0] = 0.0f;
    
    for (int i=0; i<59; i++) {
        charWidths[i] = charWidthsPixels[i]/512.0f;
        
        charTexPosY[i] = charRow[i] * texHeight;
        
        if (i > 0) {
            if (charRow[i] > charRow[i-1]) {
                charTexPos[i] = 0;
            } else {
                charTexPos[i] = charTexPos[i-1] + charWidths[i-1];            
            }
        }
        texRow = (int)(charTexPos[i]+charWidths[i]);
        
    }
    
    for (int i=0; i < _size; i++) {
        character = cString[i] - 32;
        tx = charTexPos[character];
        ty = charTexPosY[character];
        texWidth = charWidths[character];
        
        _texCoords[i*12] = tx;
        _texCoords[i*12 + 1] = ty;
        _texCoords[i*12 + 2] = tx + texWidth;
        _texCoords[i*12 + 3] = ty;
        _texCoords[i*12 + 4] = tx;
        _texCoords[i*12 + 5] = ty + texHeight;
        _texCoords[i*12 + 6] = tx;
        _texCoords[i*12 + 7] = ty + texHeight;
        _texCoords[i*12 + 8] = tx + texWidth;
        _texCoords[i*12 + 9] = ty;	
        _texCoords[i*12 + 10] = tx + texWidth;
        _texCoords[i*12 + 11] = ty + texHeight;
    }
    
    
    height *= scale;
    
    for (int i=0; i < _size; i++) {
        character = cString[i] - 32;
        
        width = charWidthsPixels[character] * scale;
        
        _vertices[i*18 + 0] = caret + 0.0f;
        _vertices[i*18 + 1] = -height/2.0f;
        _vertices[i*18 + 2] = 0.0f;
        
        _vertices[i*18 + 3] = caret + width;
        _vertices[i*18 + 4] = -height/2.0f;
        _vertices[i*18 + 5] = 0.0f;
        
        _vertices[i*18 + 6] = caret + 0.0f;
        _vertices[i*18 + 7] = height/2.0f;
        _vertices[i*18 + 8] = 0.0f;
        
        _vertices[i*18 + 9] = caret + 0.0f;
        _vertices[i*18 + 10] = height/2.0f;
        _vertices[i*18 + 11] = 0.0f;
        
        _vertices[i*18 + 12] = caret + width;
        _vertices[i*18 + 13] = -height/2.0f;
        _vertices[i*18 + 14] = 0.0f;
        
        _vertices[i*18 + 15] = caret + width;
        _vertices[i*18 + 16] = height/2.0f;
        _vertices[i*18 + 17] = 0.0f;
        
        caret += width;
        
    }
    
    _width = caret;
}

void StringImage::drawAt(const GPoint &pos) {
	
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	
	_texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
	glDrawArrays(GL_TRIANGLES, 0, _size*6);	
}


void StringImage::drawCenteredAt(const GPoint &pos) {
	glLoadIdentity();
	glTranslatef(pos.x - _width/2.0f, pos.y, 0.0f);
    glColor4f(_color.red, _color.green, _color.blue, _color.alpha);
    
	_texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
	glDrawArrays(GL_TRIANGLES, 0, _size*6);	
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void StringImage::drawAtRotated(const GPoint &pos, GLfloat angle) {
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	
	_texture->bind();
	
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
	glDrawArrays(GL_TRIANGLES, 0, _size*6);	
}


