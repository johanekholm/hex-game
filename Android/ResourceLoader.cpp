/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-08-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceLoader.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "HexGame", __VA_ARGS__))

class ResourceLoader::PrivateData {
public:
	JNIEnv *env;
	jobject callingObject;
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
    jmethodID javaLoadTexture = d->env->GetStaticMethodID(javaOpenGLRenderer, "loadTexture", "(Ljava/lang/String;)I");
    jint result = d->env->CallStaticIntMethod(javaOpenGLRenderer, javaLoadTexture, javaFilename);

	return (GLuint)result;
}

void ResourceLoader::setEnv(JNIEnv* env) {
	d->env = env;
}
void ResourceLoader::setCallingObject(jobject callingObject) {
	d->callingObject = callingObject;
}
