#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "barber.h"
#include <mutex>

std::mutex mtex;
using namespace std;

#define EMPEH       2
#define CORTANDO    1
#define DESCANSANDO 0

/*Funcao: Barber()
Descricao: contrutor do barbeiro
*/
Barber::Barber(){
    state = EMPEH;
    energy = 1000;
}

/*Funcao: sleep
Descricao: coloca o barbeiro para dormir e inicia a execução da thread responsável por fazer ele recuperar energia
a funçao de recuperação de energia ocorre numa thread separado para que possa ser interrompida por qualquer
outro novo comando que seja passado para o barbeiro
*/
void Barber::sleep(){
            state = DESCANSANDO;
            std::thread recharge(genereate_energy);
            recharge.detach();
}

/*Funcao: cut_hair
Descricao: caso o barbeiro tenha energia para cortar o cabelo ele corta caso contrario
a função apenas informa o jogador que a energia é insuficiente e o cabelo não é cortado
*/
void Barber::cut_hair(){
    // cout << "Entrou no cut_hair()\n";
    if (energy>=200){
                state = CORTANDO;
                mtex.lock();
                energy -=200;
                // cout << "sua energia é:" << energy << "\n";
                
                mtex.unlock();
    }
    else printf("sem energia suficiente\n");
}

/*Funcao: generate_energy()
Descricao: regenera a energia do barbeiro quando ele está dormindo e garante que a energia não passará de 1000
*/
void Barber::genereate_energy(){
    while (Barber::state == DESCANSANDO)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (Barber::energy < 1000 && Barber::state==DESCANSANDO){
            Barber::energy +=50;
            // cout << "sua energia é:" << energy << "\n";
        }
        //cout << energy;
        if (Barber::energy == 1000){
            mtex.lock();
            Barber::state = EMPEH;
            mtex.unlock();
            return;
        }
    }
    cout << "Saiu do sleep\n";
}
