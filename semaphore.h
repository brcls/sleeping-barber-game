#ifndef SEMAPHORE_H
#define SEMAPHORE_H
class Semaphore{
    private:
        int sem;
    public:
        Semaphore(int);
        Semaphore();
        void up();
        void down();
};
#endif