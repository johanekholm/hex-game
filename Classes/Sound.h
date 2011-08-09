/*
 *  Sound.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"
#include <string>

class Sound {
public:
    static Sound* instance();

    void add(const std::string &name, const std::string &filename);
    void play(const std::string &name);

protected:
    void playImpl(const std::string &filename);

private:
    Sound();
    ~Sound();
    void initImpl();
    void destroyImpl();

    class PrivateData;
    class PrivateImplData;
    PrivateData *d;
    PrivateImplData *pd;
};

