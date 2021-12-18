#include <bits/stdc++.h>
#include <thread>
#include "semaphore.h"

# define CHAIRS 5 /*quantidade de cadeiras para clientes*/

Semaphore customers (0);/*clientes esperando pelo serviço*/
Semaphore barbers  (1);  /* barbeiros esperando*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/


void cut_hair(){
}

void barber (void)
{
    while (true)
    {
         customers.down();
         mutex.down();;
         waiting -=1;
         barbers.up();
         mutex.up();
         cut_hair( );
    }
}

void customer (void)
{
   mutex.down();
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      customers.up();
      mutex.up();
      barbers.down();
      get_haircut( ); 
    }
    else
           
       {mutex.up();}
}

std::thread barbeiro(barber);
std::thread cliente(customer);

int main(){
    Semaphore a(1)
}