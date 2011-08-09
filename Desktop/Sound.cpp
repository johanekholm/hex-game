/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Micke Prag on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include <iostream>

class Sound::PrivateImplData {
public:
};

void Sound::initImpl() {
    pd = new PrivateImplData;
}

void Sound::destroyImpl() {
    delete pd;
}

void Sound::playImpl(const std::string &filename) {
    std::cout << "Playing sound: " << filename << std::endl;
}
