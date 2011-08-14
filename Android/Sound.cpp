/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "HexGame", __VA_ARGS__))

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
	LOGI("Playing sound: %s", filename.c_str());
}
