#include "semaphore.h"
#include <bits/stdc++.h>

/*Funcao: Semaphore
Descricao: controi um semaforo inicia em start
@param int start: valor inicial do semaforo
*/
Semaphore::Semaphore(int start){
    sem = start;
}

/*Funcao: up 
Descricao: aumenta o valor do semaforo
*/
void Semaphore::up(){
    sem += 1;
}

/*Funcao: down()
Descricao: diminui o valor do semaforo não o deixando assumir valores negativos
*/
void Semaphore::down(){
    if (sem>0){
        sem -= 1;
    }
    else{
        std::cout << "Deu ruim\n";
        // throw std::invalid_argument( "received negative value" );
    }
}
/*Funcao: get_sem
Descricao: retorna valor atual do semaforo
*/
int Semaphore::get_sem(){
    return sem;
}
