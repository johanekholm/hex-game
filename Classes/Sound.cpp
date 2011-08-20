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
#include <map>
#include <list>

typedef std::list<std::string> stringlist;
typedef std::map<std::string, stringlist> soundmap;

class Sound::PrivateData {
public:
    soundmap catalog;
    static Sound *instance;
};

Sound *Sound::PrivateData::instance = 0;

Sound::Sound() {
    d = new PrivateData;
    initImpl();
}

Sound::~Sound() {
    delete d;
}

void Sound::add(const std::string &name, const std::string &filename) {
    stringlist list;
    if (d->catalog.count(name)) {
        list = d->catalog[name];
    }
    list.push_back(filename);
    d->catalog[name] = list;
    this->initFile(filename);
}

void Sound::play(const std::string &name) {
    soundmap::const_iterator it = d->catalog.find(name);
    if (it == d->catalog.end()) {
        std::cout << "No sound added for '" << name << "'" << std::endl;
        return;
    }
    playImpl(d->catalog[name].front());
}

Sound* Sound::instance() {
    if (PrivateData::instance == 0) {
        PrivateData::instance = new Sound();
    }
    return PrivateData::instance;
}


