#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Menu.h"
#include "Includes/Utils.hpp" // Usando o Utils nativo do LGL para achar a lib

// SEUS OFFSETS DO COUNTER FLAME
uintptr_t libBase = 0;
#define OFF_LOCALPLAYER 0xD7E8
#define OFF_ENTITYLIST  0xDCA0
#define OFF_HEALTH      0xD8D8
#define OFF_TEAM        0xDBC8
#define OFF_AMMO        0xD99C

// Variáveis que os botões do menu vão ligar/desligar
bool aimbotEnabled = false;
bool espEnabled = false;
bool ammoEnabled = false;
bool godmodeEnabled = false;

// Loop que roda em segundo plano pra alterar a memória
void* CheatLoop(void* arg) {
    while (true) {
        if (libBase != 0) {
            // Lê o LocalPlayer usando o offset
            uintptr_t localPlayer = *(uintptr_t *)(libBase + OFF_LOCALPLAYER);
            
            if (localPlayer) {
                // Munição Infinita
                if (ammoEnabled) *(int *)(localPlayer + OFF_AMMO) = 999;
                
                // God Mode (Vida Infinita)
                if (godmodeEnabled) *(int *)(localPlayer + OFF_HEALTH) = 100;

                // ESP / Aimbot (Leitura da lista de inimigos)
                if (aimbotEnabled || espEnabled) {
                    uintptr_t entityList = *(uintptr_t *)(libBase + OFF_ENTITYLIST);
                    int myTeam = *(int *)(localPlayer + OFF_TEAM);
                    
                    // O jogo costuma ter até 10 players
                    for (int i = 0; i < 10; i++) {
                        uintptr_t enemy = *(uintptr_t *)(entityList + (i * 0x8)); 
                        if (enemy && enemy != localPlayer) {
                            int enemyHealth = *(int *)(enemy + OFF_HEALTH);
                            int enemyTeam = *(int *)(enemy + OFF_TEAM);
                            
                            // Se o inimigo estiver vivo e for do time adversário
                            if (enemyHealth > 0 && enemyHealth < 150 && enemyTeam != myTeam) {
                                // Aqui entraria a lógica visual do ESP e a puxada de mira
                            }
                        }
                    }
                }
            }
        }
        usleep(100000); // Roda a cada 100ms (não pesa no celular)
    }
    return NULL;
}

// Criação do Menu visual
void MakeMenu() {
    AddCategory("COUNTER FLAME MOBILE");
    AddToggle("Aimbot (Mira)", &aimbotEnabled);
    AddToggle("ESP (Caixas)", &espEnabled);
    AddToggle("Munição Infinita", &ammoEnabled);
    AddToggle("God Mode (Vida Infinita)", &godmodeEnabled);
}

// Thread de inicialização (Roda quando o APK abre)
void* MainThread(void* arg) {
    // Espera a biblioteca do jogo (libil2cpp.so) ser carregada na memória
    do {
        sleep(1);
        // Usa a função getAbsoluteAddress do Utils.hpp do LGL
        libBase = (uintptr_t)getAbsoluteAddress("libil2cpp.so", 0);
    } while (libBase == 0);

    // Inicia o loop do cheat
    pthread_t cheatThread;
    pthread_create(&cheatThread, NULL, CheatLoop, NULL);

    return NULL;
}

// O "Construtor" que o Android chama automaticamente
__attribute__((constructor))
void Constructor() {
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, MainThread, NULL);
}
