#include <jni.h>
#include "CentralControl.h"
#include "toolkit.h"

extern "C" {
	JNIEXPORT void JNICALL Java_com_hexgame_game_GameGLView_touchBegan(JNIEnv * env, jobject obj, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_hexgame_game_GameGLView_touchEnded(JNIEnv * env, jobject obj, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_hexgame_game_GameGLView_touchBegan(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
	CentralControl::instance()->touchesBegan(GPointMake(x * 320.0/480.0, y * 480.0/800.0));
}

JNIEXPORT void JNICALL Java_com_hexgame_game_GameGLView_touchEnded(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
	CentralControl::instance()->touchesEnded(GPointMake(x * 320.0/480.0, y * 480.0/800.0));
}