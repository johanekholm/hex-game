/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include "AssetLoader.h"
#include <android/asset_manager.h>
#include <assert.h>
#include <map>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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

	class SoundPlayer {
	public:
		SLObjectItf fdPlayerObject;
		SLPlayItf fdPlayerPlay;
		SLSeekItf fdPlayerSeek;
	};

	std::map<std::string, SoundPlayer> soundPlayers;
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
	SLresult result;

	std::string filename = name + ".mp3";
    // use asset manager to open asset by filename
    AssetLoader *loader = AssetLoader::instance();
    AAsset *asset = loader->open(filename);

    // the asset might not be found
    if (NULL == asset) {
		LOGI("Asset not found: %s", name.c_str());
		return;
    }

	// open asset as file descriptor
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    assert(0 <= fd);
    AAsset_close(asset);

    // configure audio source
    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, pd->outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    PrivateImplData::SoundPlayer soundPlayer;
    const SLInterfaceID ids[1] = {SL_IID_SEEK};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    result = (*pd->engineEngine)->CreateAudioPlayer(pd->engineEngine, &soundPlayer.fdPlayerObject, &audioSrc, &audioSnk,
            1, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    // realize the player
    result = (*soundPlayer.fdPlayerObject)->Realize(soundPlayer.fdPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the play interface
    result = (*soundPlayer.fdPlayerObject)->GetInterface(soundPlayer.fdPlayerObject, SL_IID_PLAY, &soundPlayer.fdPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);

    // get the seek interface
    result = (*soundPlayer.fdPlayerObject)->GetInterface(soundPlayer.fdPlayerObject, SL_IID_SEEK, &soundPlayer.fdPlayerSeek);
    assert(SL_RESULT_SUCCESS == result);

    pd->soundPlayers[name] = soundPlayer;
}

void Sound::playImpl(const std::string &filename) {
    SLresult result;

    SLPlayItf fdPlayerPlay = pd->soundPlayers[filename].fdPlayerPlay;
    SLSeekItf fdPlayerSeek = pd->soundPlayers[filename].fdPlayerSeek;

    // make sure the asset audio player was created
    if (NULL != fdPlayerPlay) {
        // set the player's marker position to the beginning
        result = (*fdPlayerSeek)->SetPosition(fdPlayerSeek, 0, SL_SEEKMODE_FAST);
        assert(SL_RESULT_SUCCESS == result);

        // set the player's state
        result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
        assert(SL_RESULT_SUCCESS == result);
    }
}
