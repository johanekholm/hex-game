#include <string>
#include <jni.h>
#include "CentralControl.h"
#include "TextureCatalog.h"
#include "ResourceLoader.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "HexGame", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "HexGame", __VA_ARGS__))


extern "C" {
    JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_init(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_draw(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_update(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_addTexture(JNIEnv * env, jobject obj, jstring textureName, jint textureId, jint tiles);
	JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_loadResources( JNIEnv* env, jobject callingObj);
};

JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_init(JNIEnv * env, jobject obj)
{
	CentralControl::instance();
}

JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_update(JNIEnv * env, jobject obj)
{
	CentralControl::instance()->update();
}

JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_draw(JNIEnv * env, jobject obj)
{
	CentralControl::instance()->draw();
}

JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_addTexture(JNIEnv * env, jobject obj, jstring textureName, jint textureId, jint tiles) {
	TextureCatalog* textureCatalog = TextureCatalog::instance();
	
	const char * tempStr = env->GetStringUTFChars(textureName, 0);
	std::string tName(tempStr);
	env->ReleaseStringUTFChars(textureName, tempStr);
	textureCatalog->addAndLoad(tName, textureId, tiles);
}

JNIEXPORT void JNICALL Java_com_hexgame_game_OpenGLRenderer_loadResources( JNIEnv* env, jobject callingObject) {
	ResourceLoader resourceLoader;
	resourceLoader.setEnv(env);
	resourceLoader.setCallingObject(callingObject);
	resourceLoader.load();
}
