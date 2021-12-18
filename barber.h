class Barber{
    private:
        static int state;
        static void genereate_energy();
    public:
        Barber();
        static int energy;
        void cut_hair();
        void sleep();
};