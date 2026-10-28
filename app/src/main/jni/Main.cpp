#include <jni.h>
#include <pthread.h>
extern void MakeMenu();
void *menu_thread(void *) {
    MakeMenu();
    return NULL;
}
__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, menu_thread, NULL);
}
