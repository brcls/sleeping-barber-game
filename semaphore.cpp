#include <semaphore.h>

class Semaphore{
    private:
        int sem;
    public:
        Semaphore();
        Semaphore(int);
        void up(){
            sem += 1;
        }
        void down(){
            if (sem > 0) 
                sem -= 1;
            else
                throw std::invalid_argument("received negative value\n");
        }
};

Semaphore::Semaphore(int start){
    sem = start;
}

Semaphore::Semaphore(){
    sem=0;
}
