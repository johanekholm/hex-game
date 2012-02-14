//
//  HexMap.h
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef HEXMAPMODEL_H
#define HEXMAPMODEL_H


#include <vector>
#include <map>
#include <string>

#include "toolkit.h"
#include "json-forwards.h"

struct HexState {
    MPoint pos;
    int value;
    
    Json::Value serialize();
    static HexState deserialize(Json::Value& root);    
};

class HexMapModel {
	int _width, _height;
    GLfloat _scale;
    std::map<int, HexState> _hexes;
    
public:
	~HexMapModel();
    HexMapModel();
	HexMapModel(int width, int height);
    HexMapModel(int width, int height, const std::string& data);
    Json::Value serialize();
    void deserialize(Json::Value& root);
    
    std::map<int, HexState> getAllHexes();
    int getHeight();
    int getHexValue(const MPoint& hex);
    int getHexValue(int x, int y);
    GLfloat getScale();
    int getWidth();
};


#endif
