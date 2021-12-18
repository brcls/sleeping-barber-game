# define CHAIRS 5 /*quantidade de cadeiras para clientes*/
typedef int semaphore;
semaphore customers = 0;/*clientes esperando pelo serviço*/
semaphore barbers = 0;  /* barbeiros esperando*/
semaphore mutex = 1; /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

void barber (void)
{
       while (TRUE)
      {
           down(&custumers):
           down(&mutex);
           waiting = waiting – 1;
           up(&barbers);
           up(&mutex);
           cut_hair( );
      }
}
void customer (void)
{
   down(&mutex);
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      up(&customers);
      up(&mutex);
      down(&barbers);
      get_haircut( ); 
    }
    else
           
       {up(&mutex);}
}