#include "semaphore.h"
#include <bits/stdc++.h>

class Semaphore{
    private:
        int sem;
    public:
        Semaphore(int start){
            sem = start;
        }
        Semaphore(){
            sem=0;
        }
        void up(){
            sem += 1;
        }
        void down(){
            if (sem>0){
                sem -= 1;
            }
            else{
                throw std::invalid_argument( "received negative value" );
            }
        }
};
Semaphore::Semaphore(int start){
    sem = start;
}
Semaphore::Semaphore(){
    sem=0;
}