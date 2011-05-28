//
//  HexMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "HexMapModel.h"


HexMapModel::~HexMapModel() {
}

HexMapModel::HexMapModel(int width, int height) {
    HexState state;
	_width = width;
	_height = height;
    
    for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
            state.pos = MPointMake(j, i);
            state.value = 1;
            _hexes[i * _width + j] = state;
        }
    }
}

std::map<int, HexState> HexMapModel::getAllHexes() {
    return _hexes;
}

