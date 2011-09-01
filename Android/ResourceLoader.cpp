/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-08-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceLoader.h"
#include "AssetLoader.h"
#include "toolkit.h"

#include <android/asset_manager.h>
#include <assert.h>

class ResourceLoader::PrivateData {
public:
	JNIEnv *env;
	jobject callingObject;
	jobject gl;
};

ResourceLoader::ResourceLoader()
:AbstractResourceLoader(), d(new PrivateData)
{
}

ResourceLoader::~ResourceLoader() {
	delete d;
}

GLuint ResourceLoader::loadTexture(const std::string &filename) {
    jstring javaFilename = d->env->NewStringUTF(filename.c_str());
    jclass javaOpenGLRenderer = d->env->GetObjectClass(d->callingObject);
    jmethodID javaLoadTexture = d->env->GetMethodID(javaOpenGLRenderer, "loadTexture", "(Ljavax/microedition/khronos/opengles/GL10;Ljava/lang/String;)I");
    jint result = d->env->CallIntMethod(d->callingObject, javaLoadTexture, d->gl, javaFilename);

	return (GLuint)result;
}

void ResourceLoader::setEnv(JNIEnv* env) {
	d->env = env;
}
void ResourceLoader::setCallingObject(jobject callingObject) {
	d->callingObject = callingObject;
}
void ResourceLoader::setGL(jobject gl) {
	d->gl = gl;
}

std::string ResourceLoader::loadLuaScript(const std::string &scriptname) {
	const std::string &filename = scriptname + ".lua";
	AssetLoader *loader = AssetLoader::instance();
    AAsset *asset = loader->open(filename);

    // the asset might not be found
    if (NULL == asset) {
		DEBUGLOG("Asset not found: %s", filename.c_str());
		return NULL;
    }

	size_t length = AAsset_getLength(asset);
	const char *buffer = (char *)AAsset_getBuffer(asset);

	std::string script(buffer, length);

	if (buffer = NULL) {
		DEBUGLOG("Buffer is empty");
	}

	AAsset_close(asset);
	return script;
}