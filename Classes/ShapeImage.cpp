//
//  RectangleImage.cpp
//  hex-game
//
//  Created by Johan Ekholm
//

#include "ShapeImage.h"
#include <cmath>
#include <iostream>
#include <cstring>
#define PI 3.14159265

ShapeImage::~ShapeImage() {
    delete [] _vertices;
}

ShapeImage::ShapeImage(const RGBA& color, GLfloat width, GLfloat height, bool filled) {
    _width = width;
    _height = height;
    _color = color;
    _filled = filled;
    _vertices = 0;
    _numVertices = 0;
}

void ShapeImage::drawAt(const GPoint &pos) {
    glDisable(GL_TEXTURE_2D);
    glColor4f(_color.red, _color.green, _color.blue, _color.alpha);
	glLoadIdentity();
	glTranslatef(pos.x + _width/2.0f, pos.y + _height/2.0f, 0.0f);
	
	glVertexPointer(2, GL_FLOAT, 0, _vertices);
	glDrawArrays(GL_TRIANGLES, 0, _numVertices);	
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
}

void ShapeImage::drawCenteredAt(const GPoint &pos) {
    glDisable(GL_TEXTURE_2D);
    glColor4f(_color.red, _color.green, _color.blue, _color.alpha);
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	
	glVertexPointer(2, GL_FLOAT, 0, _vertices);
	glDrawArrays(_vertexArrayType, 0, _numVertices);	
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);    
}

void ShapeImage::setColor(const RGBA& color) {
    _color = color;
}

void ShapeImage::setSize(GLfloat width, GLfloat height) {
    if (width != _width || height != _height) {
        _width = width;
        _height = height;
        this->buildVertices();        
    }
}

// ----------------------------------------------- //

EllipseImage::EllipseImage(const RGBA& color, GLfloat width, GLfloat height, int segments, bool filled) : ShapeImage(color, width, height, filled) {
    _segments = segments;
    _numVertices = segments;
    _vertexArrayType = (filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP;
    this->buildVertices();
}

void EllipseImage::buildVertices() {
    int count=0;

    if (_vertices != 0) {
        delete [] _vertices;
    }
    
    _vertices = new GLfloat[_segments*2];
    
    for (GLfloat i = 0; i < 360.0f; i+=(360.0f/_segments)) {
        _vertices[count++] = (cos(i*PI/180)*_width);
        _vertices[count++] = (sin(i*PI/180)*_height);
    }
}



// ----------------------------------------------- //


RectangleImage::RectangleImage(const RGBA& color, GLfloat width, GLfloat height, bool filled) : ShapeImage(color, width, height, true) {
    _numVertices = 6;
    _vertexArrayType = GL_TRIANGLES;
    this->buildVertices();
}

void RectangleImage::buildVertices() {    
    if (_vertices != 0) {
        delete [] _vertices;
    }
    
    _vertices = new GLfloat[_numVertices*2];
    
	GLfloat	vertices[] = {	-_width/2.0f, -_height/2.0f,
                            _width/2.0f, -_height/2.0f,
                            -_width/2.0f, _height/2.0f,
                            -_width/2.0f, _height/2.0f,
                            _width/2.0f, -_height/2.0f,
                            _width/2.0f, _height/2.0f };
    
    memcpy(_vertices, vertices, sizeof(vertices));
}



// ----------------------------------------------- //

/*RectangleImage::RectangleImage() {
    _red = 1.0f;
    _green = 1.0f;
    _blue = 1.0f;
    _alpha = 1.0f;
}

RectangleImage::RectangleImage(const RGBA& color) {
    _red = color.red;
    _green = color.green;
    _blue = color.blue;
    _alpha = color.alpha;
}

RectangleImage::RectangleImage(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    _red = red;
    _green = green;
    _blue = blue;
    _alpha = alpha;
}

void RectangleImage::drawAsEllipse(int segments, const GPoint& pos, bool filled, GLfloat width, GLfloat height) {
    GLfloat vertices[segments*2];
    int count=0;
    
    for (GLfloat i = 0; i < 360.0f; i+=(360.0f/segments)) {
        vertices[count++] = (cos(i*PI/180)*width);
        vertices[count++] = (sin(i*PI/180)*height);
        //std::cout << vertices[count-2] << ", " << vertices[count-1] << std::endl;
    }


    glDisable(GL_TEXTURE_2D);
    glColor4f(_red, _green, _blue, _alpha);
	glLoadIdentity();
    glTranslatef(pos.x, pos.y, 0.0f);
    glVertexPointer (2, GL_FLOAT , 0, vertices); 
    glDrawArrays ((filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, segments);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

}

void RectangleImage::drawAt(const GPoint &pos) {
	GLfloat	vertices[] = {	-_width/2.0f, -_height/2.0f, 0.0f,
		_width/2.0f, -_height/2.0f, 0.0f,
		-_width/2.0f, _height/2.0f, 0.0f,
		-_width/2.0f, _height/2.0f, 0.0f,
		_width/2.0f, -_height/2.0f, 0.0f,		
		_width/2.0f, _height/2.0f, 0.0f };
	
    glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
    glEnable(GL_TEXTURE_2D);
}

void RectangleImage::drawAtWithSize(const GPoint &pos, GLfloat width, GLfloat height) {
	GLfloat	vertices[] = {	0.0f, 0.0f, 0.0f,
		width, 0.0f, 0.0f,
		0.0f, height, 0.0f,
		0.0f, height, 0.0f,
		width, 0.0f, 0.0f,		
		width, height, 0.0f };

    glDisable(GL_TEXTURE_2D);
    glColor4f(_red, _green, _blue, _alpha);
    
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0.0f);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);	

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
}

void RectangleImage::setColor(const RGBA& color) {
    _red = color.red;
    _green = color.green;
    _blue = color.blue;
    _alpha = color.alpha;
}
*/

