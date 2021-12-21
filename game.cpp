#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include "barber.h"
#include "semaphore.h"

//Estados do barbeiro
#define EMPEH       2
#define CORTANDO    1
#define DESCANSANDO 0

//estados  do programa
#define EXECUTANDO 1
#define PARADO 0

//configuração inicial do barbeiro
int Barber::energy = 1000;
int Barber::state=EMPEH;

#define CHAIRS 3 /*quantidade de cadeiras para clientes*/

std::mutex mtx; // mutex que controla as regioes crirticas

unsigned int score = 0;
int executing = EXECUTANDO;

Barber barber; // classe que controla o barbeiro
Semaphore customers (0);/*clientes esperando pelo serviço*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

/*Funcao: cutting
Descricao: faz toda a regulação de semaforos e mutex na hora decortar o cabelo
*/
void cutting ()
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

/*Funcao: customer
Descricao: faz toda a regulação de semaforos e mutex na hora de adicionar um novo cliete
*/
void customer ()
{
    mtx.lock();
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      customers.up();
      mtx.unlock();
      std::cout << "Um novo cliente chega\n";
    }
    else
    {
        executing = PARADO;
        barber.state=EMPEH;
    }
}

/*Funcao: generate_customer
Descricao: adiciona um novo cliente a cada 5-10s, esse valor diminui chegando a um cliente a cada 0-5s na pontuacao maxima
*/
void generate_customer() {
    // std::cout << "Entrou na thread de gerar\n";
    int dificuldade = score/5000;
    if (dificuldade >=5)
        dificuldade = 5;
    int max = 5, cTime;
    srand(time(0));
    cTime = (rand() % max) + (5-dificuldade);
    std::this_thread::sleep_for(std::chrono::seconds(cTime));
    customer();
}

/*Funcao: get_input
Descricao: fica rodando em uma thread propria detectando qualquer input que o player digite
Parâmetros: int* exec, ponteiro de inteiro que define se o jogo exta ou não sendo executado
*/
void get_input(int *exec) {
    // std::cout << "Entrou na thread de pegar input\n";
    char input;
    std::cin >> input;
    if(*exec == EXECUTANDO){
        if (input == 'z'){
            if (customers.get_sem() > 0){
                cutting();
                std::cout << "sua pontuacao é: " << score << '\n';
            }
            else{
                std::cout << "não havia nenhum cliente na loja";
                barber.state = EMPEH;
            }
        }
        else if (input == 'x'){
            barber.sleep();
        }
        else if (input == 'q'){
            *exec = PARADO;
        }
    }
    else
        *exec = PARADO;
    if (*exec == EXECUTANDO)
        get_input(exec);
}

/*Funcao: drenar_energia
Descricao: função que fica em uma thread executando constantemente drenando energia se o barbeiro esta de pé e nao fazendo nada se ele está em outro estado
*/
void drenar_energia(int *exec) {
    std::cout << barber.state << std::endl;
    if (*exec == PARADO)
        return;
    while (barber.state == EMPEH && *exec ==EXECUTANDO) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (barber.energy > 0){
            barber.energy -=50;
            std::cout << "sua energia é:" << barber.energy << "\n";
        } else {
            *exec = PARADO;
            return;
        }
    }
    while (barber.state != EMPEH && *exec == EXECUTANDO) {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    drenar_energia(exec);
}

// função responasavel por toda execução do codigo
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
