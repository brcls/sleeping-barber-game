#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <thread>

#define CORTANDO 1
#define DESCANSANDO 0

class Barber{
    private:
        int state = 0;
        void genereate_energy(){
            while (state = DESCANSANDO)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                energy +=100;
                cout << energy;
            }
        }
    public:
        int energy = 1000;
        void cut_hair(){
            if (energy>=200){
                state = CORTANDO;
                energy -=200;
            }
            else printf("sem energia suficiente");
        }
        void sleep(){
            state = DESCANSANDO;
            std::thread recharge(genereate_energy);
            recharge.join();
        }
};
