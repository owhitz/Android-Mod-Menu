#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Includes/Utils.hpp"

uintptr_t libBase = 0;
#define OFF_LOCALPLAYER 0xD7E8
#define OFF_ENTITYLIST  0xDCA0
#define OFF_HEALTH      0xD8D8
#define OFF_TEAM        0xDBC8
#define OFF_AMMO        0xD99C

bool aimbotEnabled = false;
bool espEnabled = false;
bool ammoEnabled = false;
bool godmodeEnabled = false;

void* CheatLoop(void* arg) {
    while (true) {
        if (libBase != 0) {
            uintptr_t localPlayer = *(uintptr_t *)(libBase + OFF_LOCALPLAYER);
            if (localPlayer) {
                if (ammoEnabled) *(int *)(localPlayer + OFF_AMMO) = 999;
                if (godmodeEnabled) *(int *)(localPlayer + OFF_HEALTH) = 100;
            }
        }
        usleep(100000);
    }
    return NULL;
}

// Essa função é chamada quando você mexe nos botões do menu
extern "C" void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    switch (featNum) {
        case 0: aimbotEnabled = boolean; break;
        case 1: espEnabled = boolean; break;
        case 2: ammoEnabled = boolean; break;
        case 3: godmodeEnabled = boolean; break;
    }
}

// Essa função desenha os botões no menu
extern "C" jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;
    const char *features[] = {
        "Toggle_Aimbot",
        "Toggle_ESP",
        "Toggle_Munição Infinita",
        "Toggle_God Mode"
    };
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray) env->NewObjectArray(Total_Feature, env->FindClass("java/lang/String"), env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++) env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    return (ret);
}

void* MainThread(void* arg) {
    do {
        sleep(1);
        libBase = (uintptr_t)getAbsoluteAddress("libil2cpp.so", 0);
    } while (libBase == 0);

    pthread_t cheatThread;
    pthread_create(&cheatThread, NULL, CheatLoop, NULL);
    return NULL;
}

__attribute__((constructor))
void Constructor() {
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, MainThread, NULL);
}
