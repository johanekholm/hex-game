/*
 *  AssetLoader.cpp
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AssetLoader.h"

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "HexGame", __VA_ARGS__))

class AssetLoader::PrivateData {
public:
	AAssetManager *manager;
    static AssetLoader *instance;
};

AssetLoader *AssetLoader::PrivateData::instance = 0;

AssetLoader::AssetLoader() {
    d = new PrivateData;
    d->manager = 0;
}

AssetLoader::~AssetLoader() {
    delete d;
}

AAsset *AssetLoader::open(const std::string &filename) {
	if (!d->manager) {
		LOGI("Manager not found, aborting!");
		return 0;
	}
	return AAssetManager_open(d->manager, filename.c_str(), AASSET_MODE_UNKNOWN);
}

AssetLoader* AssetLoader::instance() {
    if (PrivateData::instance == 0) {
        PrivateData::instance = new AssetLoader();
    }
    return PrivateData::instance;
}

void AssetLoader::realize(JNIEnv *env, jobject assetManager) {
    d->manager = AAssetManager_fromJava(env, assetManager);
    if (!d->manager) {
 	   LOGI("Failed to Realize AssetLoader");
 	}
}   
