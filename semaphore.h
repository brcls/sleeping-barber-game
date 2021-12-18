
class Semaphore{
    private:
        int sem;
    public:
        Semaphore(int);
        void up();
        void down();
};
