//
//  RectangleImage.cpp
//  hex-game
//
//  Created by Johan Ekholm
//

#include "RectangleImage.h"

RectangleImage::RectangleImage() {
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
