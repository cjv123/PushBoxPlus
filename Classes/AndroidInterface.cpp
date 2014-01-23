#include "AndroidInterface.h"
#include <cocos2d.h>
using namespace cocos2d;

#ifdef ANDROID
JavaVM* g_javaVM = NULL;
jclass g_cls;

JNIEXPORT void JNICALL Java_zfteam_game_pushbox_PushBoxPlus_initJVM(JNIEnv *env, jclass)
{
	CCLOG("init jvm bengin");
	env->GetJavaVM(&g_javaVM);
	CCLOG("init jvm end");
}

jmethodID getMethodID(JNIEnv* env, const char* method, const char* parames)
{
	jclass g_cls =  env->FindClass("zfteam/game/pushbox/PushBoxPlus");
	if (!g_cls)
	{
		CCLOG("zfteam/game/pushbox/PushBoxPlus class is not found.");
		return NULL;
	}
	jmethodID methodId = env->GetStaticMethodID(g_cls, method,parames);
	return methodId;
}

#endif

void set_adview_visible(int showflag)
{
#ifdef ANDROID
	JNIEnv *env;
	g_javaVM->AttachCurrentThread(&env, NULL);
	jmethodID methodId = getMethodID(env,"setAdViewVisable","(I)V");

	if (!methodId)
	{
		CCLOG("setAdViewVisable function is not found.");
		return;
	}

	env->CallStaticObjectMethod( g_cls, methodId,showflag);
#endif
}


void share_to_freinds()
{
#ifdef ANDROID
	JNIEnv *env;
	g_javaVM->AttachCurrentThread(&env, NULL);
	jmethodID methodId = getMethodID(env,"shareToFreinds","()V");

	if (!methodId)
	{
		CCLOG("shareToFreinds function is not found.");
		return;
	}

	env->CallStaticObjectMethod(g_cls, methodId);
#endif
}


