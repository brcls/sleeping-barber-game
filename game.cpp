#include <bits/stdc++.h>
#include <thread>
#include "semaphore.h"
#include <mutex>
#include <cstdlib>
#include <chrono>

# define CHAIRS 5 /*quantidade de cadeiras para clientes*/

//std::mutex mtx;

unsigned int score = 0;

Semaphore customers (0);/*clientes esperando pelo serviço*/
Semaphore barbers  (0);  /* barbeiros esperando*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

/*
void cut_hair(){
}
*/

void barber (void)
{
    customers.down();
    mutex.down();
    waiting -=1;
    barbers.up();
    mutex.up();
    //cut_hair( );
    std::this_thread::sleep_for(std::chrono::seconds(3));
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
    //cout << nCustomer;
}

void regenerate_energy() {
    std::this_thread::sleep_for(std::chrono::seconds(1);
    energy += 100;
}

void get_input() {
    char input;
    cin >> input;
    if (input == 'z')
        barber();
    if (input == 'x')
        regenerate_energy();
}

int main(){
    //std::thread game(game_logic);
    //game.join();    

    int executing = 1;
    while (true) {
        std::thread generation(generate_customer);
        std::thread get_input(get_input);
        //std::thread (get_input);
        generation.join();
        get_input.join();
    }
}

