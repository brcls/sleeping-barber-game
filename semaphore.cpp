#include "semaphore.h"
#include <bits/stdc++.h>

Semaphore::Semaphore(int start){
    sem = start;
}

void Semaphore::up(){
    sem += 1;
}
void Semaphore::down(){
    if (sem>0){
        sem -= 1;
    }
    else{
        std::cout << "Deu ruim";
        // throw std::invalid_argument( "received negative value" );
    }
}