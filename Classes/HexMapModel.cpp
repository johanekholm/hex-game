//
//  HexMap.m
//  igame
//
//  Created by Johan Ekholm on 2010-08-15.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "HexMapModel.h"
#include "json.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


HexMapModel::~HexMapModel() {
    _hexes.clear();

}

HexMapModel::HexMapModel() : _width(0), _height(0) {}

HexMapModel::HexMapModel(int width, int height) {
    using namespace std;
    HexState state;
	_width = width;
	_height = height;
	_scale = 1.5f;
    string data = "0 0 0 3 3 3 0 0\n1 1 1 1 3 1 1 1\n2 2 1 1 2 2 0 2\n1 2 1 1 1 2 1 2\n0 1 0 2 0 1 0 2\n1 2 1 2 1 0 1 2\n0 1 0 2 0 1 0 2\n1 2 1 2 1 2 1 2\n";
    istringstream dataStream(data);
    vector<string> tokens;
    vector<string>::const_iterator it;
    int number = 0;
    
    copy(istream_iterator<string>(dataStream), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
    
    it = tokens.begin();
    
    for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
            state.pos = MPointMake(j, i);

            if (it != tokens.end()) {
                (stringstream(*it) >> number) ? state.value = number : state.value = 0;
            } else {
                state.value = 0;
            }
            _hexes[i * _width + j] = state;
            ++it;
        }
    }
}

HexMapModel::HexMapModel(int width, int height, const std::string& data) {
    using namespace std;
    HexState state;
	_width = width;
	_height = height;
    istringstream dataStream(data);
    vector<string> tokens;
    vector<string>::const_iterator it;
    int number = 0;
    
    copy(istream_iterator<string>(dataStream), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
    
    it = tokens.begin();
    
    for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
            state.pos = MPointMake(j, i);
            
            if (it != tokens.end()) {
                (stringstream(*it) >> number) ? state.value = number : state.value = 0;
            } else {
                state.value = 0;
            }
            _hexes[i * _width + j] = state;
            ++it;
        }
    }
}

Json::Value HexState::serialize() {
    Json::Value root;
    root["x"] = pos.x;
    root["y"] = pos.y;
    root["v"] = value;    
    
    return root;
}

HexState HexState::deserialize(Json::Value& root) {
    HexState hexState;
    hexState.pos.x = root.get("x", 0.0f).asFloat();
    hexState.pos.y = root.get("y", 0.0f).asFloat();
    hexState.value = root.get("v", 0).asInt();
    return hexState;
}

Json::Value HexMapModel::serialize() {
    Json::Value root;
    Json::Value& hexes = root["hexes"];
    root["width"] = _width;
    root["height"] = _height;
    root["scale"] = _scale;
    
    // serialize hex data
    for (std::map<int, HexState>::iterator it = _hexes.begin(); it != _hexes.end(); it++) {
		hexes[it->first] = (it->second).serialize();
    }
    return root;
}

void HexMapModel::deserialize(Json::Value& root) {
    _width = root.get("width", 0).asInt();
    _height = root.get("height", 0).asInt();
    _scale = root.get("scale", 1.0f).asFloat();

	_hexes.clear();
	
    // deserialize hex data
    for (Json::ValueIterator it = root["hexes"].begin(); it != root["hexes"].end(); it++) {
		_hexes[it.key().asInt()] = HexState::deserialize(*it);
    }
}

std::map<int, HexState> HexMapModel::getAllHexes() {
    return _hexes;
}

int HexMapModel::getHeight() {
    return _height;
}

int HexMapModel::getHexValue(const MPoint& hex) {
    if (_hexes.find(hex.y * _width + hex.x) != _hexes.end()) {
		return _hexes[hex.y * _width + hex.x].value;
    }
	return 0;
}

int HexMapModel::getHexValue(int x, int y) {
    if (_hexes.find(y * _width + x) != _hexes.end()) {
		return _hexes[y * _width + x].value;
	}
	return 0;
}

int HexMapModel::getWidth() {
    return _width;
}

GLfloat HexMapModel::getScale() {
    return _scale;
}
