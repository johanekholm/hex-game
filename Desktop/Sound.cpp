/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Micke Prag on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include <QCoreApplication>
#include <QSound>
#include <QDir>

class Sound::PrivateImplData {
public:
    QDir dir;
};

void Sound::initImpl() {
    pd = new PrivateImplData;
    pd->dir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_MAC)
    if (pd->dir.dirName() == "MacOS") {
        pd->dir.cdUp();
        pd->dir.cd("Resources");
    }
#endif
}

void Sound::destroyImpl() {
    delete pd;
}

void Sound::initFile(const std::string &filename) {
}

void Sound::playImpl(const std::string &filename) {
    QSound::play(pd->dir.filePath(QString(filename.c_str()) + ".wav"));
}
