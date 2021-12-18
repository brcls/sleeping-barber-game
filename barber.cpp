#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

#define CORTANDO 1
#define DESCANSANDO 0

class Barber{
    private:
        static int state;
        static void genereate_energy(){
            while (state == DESCANSANDO)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                energy +=100;
                cout << energy;
            }
        }
    public:
        Barber();
        static int energy;
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
Barber::Barber(){
    state = 0;
    energy = 1000;
}