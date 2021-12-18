
class Semaphore{
    private:
        int sem;
    public:
        Semaphore();
        Semaphore(int);
        void up();
        void down();
};
