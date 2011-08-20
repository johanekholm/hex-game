/*
 *  Sound.h
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <jni.h>

class AAsset;

class AssetLoader {
public:
	AAsset* open (const std::string &filename);
	
	static AssetLoader *instance();
	void realize(JNIEnv *env, jobject assetManager);
protected:

private:
    AssetLoader();
    ~AssetLoader();
    class PrivateData;
    PrivateData *d;
};

