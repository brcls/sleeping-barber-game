#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "barber.h"

using namespace std;

#define EMPEH       3
#define DESCANSADO  2
#define CORTANDO    1
#define DESCANSANDO 0

Barber::Barber(){
    state = 3;
    energy = 1000;
}

void Barber::sleep(){
            state = DESCANSANDO;
            std::thread recharge(genereate_energy);
            recharge.join();
}

void Barber::cut_hair(){
    cout << "Entrou no cut_hair()\n";
    if (energy>=200){
                state = CORTANDO;
                energy -=200;
                cout << "sua energia é:" << energy << "\n";
                state = EMPEH;
    }
    else printf("sem energia suficiente\n");
}

void Barber::genereate_energy(){
    while (Barber::state == DESCANSANDO)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (Barber::energy < 1000){
            Barber::energy +=50;
            cout << "sua energia é:" << energy << "\n";
        }
        //cout << energy;
        if (Barber::energy == 1000)
            Barber::state = DESCANSADO;
    }
    cout << "Saiu do sleep\n";
}
