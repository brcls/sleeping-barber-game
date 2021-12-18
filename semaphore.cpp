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
            sem -= 1;
        }
};
Semaphore::Semaphore(int start){
    sem = start;
}
Semaphore::Semaphore(){
    sem=0;
}