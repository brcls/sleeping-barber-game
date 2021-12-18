class Barber{
    private:
        int state = 0;
        void genereate_energy();
    public:
        Barber();
        int energy;
        void cut_hair();
        void sleep();
};