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

struct HexState {
    MPoint pos;
    int value;
};

class HexMapModel {
	int _width, _height;
    std::map<int, HexState> _hexes;
    
public:
	~HexMapModel();
	HexMapModel(int width, int height);
    HexMapModel(int width, int height, const std::string& data);
    std::map<int, HexState> getAllHexes();
    int getHeight();
    int getHexValue(const MPoint& hex);
    int getHexValue(int x, int y);
    int getWidth();
};


#endif
