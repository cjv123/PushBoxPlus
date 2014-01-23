#ifndef _ANDROIDINTERFACE_H
#define _ANDROIDINTERFACE_H

#ifdef ANDROID
#include <cstddef>
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void JNICALL Java_zfteam_game_pushbox_PushBoxPlus_initJVM
		(JNIEnv *env, jclass);
#ifdef __cplusplus
}
#endif
#endif

void set_adview_visible(int showflag);
void share_to_freinds();

#endif
