#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore{
    private:
        int sem;
    public:
        Semaphore();
        Semaphore(int start);
        void up();
        void down();
        int get_sem();
};

#endif
