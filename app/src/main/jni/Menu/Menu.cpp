#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Menu.h"

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

void MakeMenu() {
    AddCategory("COUNTER FLAME MOBILE");
    AddToggle("Aimbot", &aimbotEnabled);
    AddToggle("ESP", &espEnabled);
    AddToggle("Munição Infinita", &ammoEnabled);
    AddToggle("God Mode", &godmodeEnabled);
}

void* MainThread(void* arg) {
    do {
        sleep(1);
        // Puxa a biblioteca direto, sem depender do Utils.hpp
        void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
        if (handle) {
            libBase = (uintptr_t)handle;
        }
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
