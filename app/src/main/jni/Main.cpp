#include <jni.h>
#include <pthread.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"

// Declara a função que cria o menu (que está no Menu.cpp)
extern void MakeMenu();

// Função que roda em segundo plano só pra abrir a tela do menu
void *menu_thread(void *) {
    MakeMenu();
    return NULL;
}

// Construtor que o Android chama quando carrega a lib
__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, menu_thread, NULL);
}
