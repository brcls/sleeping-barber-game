#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include "barber.h"
#include "semaphore.h"

# define CHAIRS 5 /*quantidade de cadeiras para clientes*/

//std::mutex mtx;

unsigned int score = 0;
int executing = 1;

Barber barber ();
Semaphore customers (0);/*clientes esperando pelo serviço*/
//Semaphore barbers  (0);  /* barbeiros esperando*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

/*
void cut_hair(){
}
*/

void cutting (void)
{
    customers.down();
    mutex.down();
    waiting -= 1;
    //barbers.up();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    barber.cut_hair();
    mutex.up();
    //cut_hair( );
    cout << "Cabelo do cliente cortado\n";
    score += 500;
}

void customer (void)
{
   mutex.down();
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      customers.up();
      mutex.up();
      cout << "Um novo cliente chega\n";
      //barbers.down();
      //get_haircut( ); 
    }
    else
    {
       mutex.up();
       executing = 0;
    }
}


/*
void game_logic() {

    std::thread barbeiro(barber);
    std::thread cliente(customer);

    barbeiro.join();
    cliente.join();
}
*/

void generate_customer() {
    int max = 15, cTime;
    srand(time(0));
    cTime = (rand() % max) + 5;
    std::this_thread::sleep_for(std::chrono::seconds(cTime));
    customer();
}

/*
void regenerate_energy() {
    std::this_thread::sleep_for(std::chrono::seconds(1);
    energy += 100;
}
*/

void get_input() {
    char input;
    cin >> input;
    if (input == 'z')
        cutting();
    if (input == 'x')
        barber.sleep();
}

int main(){
    std::thread get_input(get_input);
    while (executing) {
        std::thread generation(generate_customer);
        generation.join();
    }
    get_input.join();
    return 0;
}

