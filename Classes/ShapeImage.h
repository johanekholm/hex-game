//
//  RectangleImage.h
//  hex-game
//
//  Created by Johan Ekholm
//

#ifndef RECTANGLEIMAGE_H
#define RECTANGLEIMAGE_H

#include "toolkit.h"

class TextureMap;


class ShapeImage {
    
protected:
	GLfloat _width, _height;
    RGBA _color;
    bool _filled;
    GLfloat* _vertices;
    int _numVertices;
    int _vertexArrayType;

public:
    virtual ~ShapeImage();
    ShapeImage(const RGBA& color, GLfloat width, GLfloat height, bool filled);
	void drawAt(const GPoint &pos);
	void drawCenteredAt(const GPoint &pos);
    virtual void buildVertices() = 0;
    void setColor(const RGBA& color);
    void setSize(GLfloat width, GLfloat height);
};


class EllipseImage : public ShapeImage {
protected:
    int _segments;
    
public:
    EllipseImage(const RGBA& color, GLfloat width, GLfloat height, int segments, bool filled);
    void buildVertices();

};


class RectangleImage : public ShapeImage {
protected:
    
public:
    RectangleImage(const RGBA& color, GLfloat width, GLfloat height, bool filled);
    void buildVertices();
};



/*class RectangleImage {

protected:
	GLfloat _width, _height;
    GLfloat _red;
    GLfloat _green;
    GLfloat _blue;
    GLfloat _alpha;


public:
    RectangleImage();
    RectangleImage(const RGBA& color);
    RectangleImage(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void drawAsEllipse(int segments, const GPoint& pos, bool filled, GLfloat width, GLfloat height);
	void drawAt(const GPoint &pos);
    void drawAtWithSize(const GPoint &pos, GLfloat width, GLfloat height);
    void setColor(const RGBA& color);
};*/

#endif