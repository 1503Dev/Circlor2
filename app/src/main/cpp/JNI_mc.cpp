#include <jni.h>

#ifndef GLOBAL_H
#define TAG "Native/JNI_mc"
#include "../global.h"
#endif

//
// Created by TheChuan1503 on 2025/7/16.
//

extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_mc_LocalPlayer_setPos(JNIEnv *env, jclass clazz, jfloat x, jfloat y,
                                           jfloat z) {
    if (isInGame() && clientInstance->getLocalPlayer()) {
        clientInstance->getLocalPlayer()->setPos(new Vec3(x + 0.5f, y + 1.62f, z + 0.5f));
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_dev1503_circlor_mc_ClientInstance_isInGame(JNIEnv *env, jclass clazz) {
    return isInGame();
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_dev1503_circlor_mc_LocalPlayer__1getPos(JNIEnv *env, jclass clazz) {
    jfloatArray array = env->NewFloatArray(3);

    if (isInGame() && clientInstance->getLocalPlayer() != nullptr) {
        Vec3 *pos = clientInstance->getLocalPlayer()->getPos();
        Vec3 _pos = *pos;
        _pos.x = _pos.x - 0.5f;
        _pos.y = _pos.y - 1.62001f;
        _pos.z = _pos.z - 0.5f;
        jfloat posArray[3] = {_pos.x, _pos.y, _pos.z};
        env->SetFloatArrayRegion(array, 0, 3, posArray);
        return array;
    }

    return array;
}