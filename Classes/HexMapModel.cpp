//
//  HexMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "HexMapModel.h"
#include <cstdlib>
#include <iostream>


HexMapModel::~HexMapModel() {
}

HexMapModel::HexMapModel(int width, int height) {
    HexState state;
	_width = width;
	_height = height;
    
    for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
            state.pos = MPointMake(j, i);
            state.value = rand() % 2;
            _hexes[i * _width + j] = state;
        }
    }
}

std::map<int, HexState> HexMapModel::getAllHexes() {
    return _hexes;
}

int HexMapModel::getHexValue(const MPoint& hex) {
    return _hexes[hex.y * _width + hex.x].value;
}

int HexMapModel::getHexValue(int x, int y) {
    //std::cout << "Hex (" << x << ", " << y << "): " << _hexes[y * _width + x].value << std::endl;
    return _hexes[y * _width + x].value;
}

