#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include "barber.h"
#include "semaphore.h"

int Barber::energy = 1000;
int Barber::state=3;

#define CHAIRS 3 /*quantidade de cadeiras para clientes*/

std::mutex mtx;

unsigned int score = 0;
int executing = 1;

Barber barber;
Semaphore customers (0);/*clientes esperando pelo serviço*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

void cutting (void)
{
    customers.down();
    mtx.lock();
    waiting -= 1;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barber.cut_hair();
    mtx.unlock();
    std::cout << "Cabelo do cliente cortado\n";
    score += 500;
}

void customer (void)
{
    mtx.lock();
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      customers.up();
      mtx.unlock();
      std::cout << " Um novo cliente chega\n";
    }
    else
    {
       executing = 0;
    }
}

void generate_customer() {
    std::cout << "Entrou na thread de gerar\n";
    int max = 5, cTime;
    srand(time(0));
    cTime = (rand() % max) + 5;
    std::this_thread::sleep_for(std::chrono::seconds(cTime));
    customer();
}

void get_input(int *exec) {
    std::cout << "Entrou na thread de pegar input\n";
    char input;
    std::cin >> input;
    if (input == 'z' && *exec == 1){
        cutting();
    }
    if (input == 'x' && *exec == 1){
        barber.sleep();
    }
    if (*exec == 1)
        get_input(exec);
}

void drenar_energia(int *exec) {
    std::cout << barber.state << std::endl;
    if (*exec == 0)
        return;
    while (barber.state == 3) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (barber.energy > 0){
            barber.energy -=50;
            std::cout << "sua energia é:" << barber.energy << "\n";
        } else {
            barber.state = 1;
            *exec = 0;
        }
    }
    while (barber.state != 3 && *exec == 1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    drenar_energia(exec);
}

int main(){
    std::thread leitura(get_input, &executing);
    std::thread drenar(drenar_energia, &executing);
    while (executing) {
        std::thread generation(generate_customer);
        generation.join();
    }
    drenar.join();
    leitura.join();
    std::cout << "GAME OVER!\n";
    return 0;
}
