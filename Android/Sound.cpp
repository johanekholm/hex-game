/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include <assert.h>
#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "HexGame", __VA_ARGS__))

class Sound::PrivateImplData {
public:
	// sound Engine data
	SLObjectItf engineObject;
	SLEngineItf engineEngine;
	SLObjectItf outputMixObject;
	
	// player data (I think we need one of these for each sound)
	SLObjectItf fdPlayerObject;
	SLPlayItf fdPlayerPlay;
	SLSeekItf fdPlayerSeek;
};

void Sound::initImpl() {
    pd = new PrivateImplData;

    SLresult result;

    // create engine
    result = slCreateEngine(&pd->engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);

    // realize the engine
    result = (*pd->engineObject)->Realize(pd->engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the engine interface, which is needed in order to create other objects
    result = (*pd->engineObject)->GetInterface(pd->engineObject, SL_IID_ENGINE, &pd->engineEngine);
    assert(SL_RESULT_SUCCESS == result);

    // create output mix
    result = (*pd->engineEngine)->CreateOutputMix(pd->engineEngine, &pd->outputMixObject, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);

    // realize the output mix
    result = (*pd->outputMixObject)->Realize(pd->outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
}

void Sound::destroyImpl() {
    delete pd;
}

void Sound::initFile(const std::string &name) {
	
}

void Sound::playImpl(const std::string &filename) {
	LOGI("Playing sound: %s", filename.c_str());
}
