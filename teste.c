#define NUM_THREADS 10

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fila[3];
int inicio;
int final;
int tamanho_fila;
int contador;
pthread_mutex_t mtx;
void imprime();
void *produtor();
void *consumidor();
void mutexManual(); //falta implementar

void *printhello(void *threadid)
{
   int tid;

   tid = (int)(long int)threadid;
   printf("Hello World! It's me, thread #%d!\n", tid);

   pthread_exit(0);
}

void *consumidor(void *threadid)
{

   pthread_mutex_lock(&mtx);
   imprime();

   //estado inicial
   //[1,1,1]
   //i=0
   //f=2
   if (contador == 0)
   {
      printf("nao tem oque ser consumido\n");
   }else{

       if (fila[inicio] == 1)
   {
      fila[inicio] = 0;

      if(inicio!=final){

      if (inicio + 1 < tamanho_fila)
      {
         inicio++;
      }
      else
      {
         if(fila[0]==1){
           inicio = 0; 
         }
         
      }


      }

      

      printf("\nconsumiu");
      contador--;
   }
  
   }
   
   pthread_mutex_unlock(&mtx);
   //estado inicial
   //[0,1,1]
   //i=1
   //f=2
}

void *produtor(void *threadid)
{
    pthread_mutex_lock(&mtx);
   imprime();

   //não pode produzir
   if (contador == tamanho_fila)
   {
      printf("produtor iria sobrescrever\n");
   }else{

//quando a fila está vazia
   //da para add aonde estou no inicio da fila
   if (fila[inicio] == 0)
   {
      fila[final] = 1;
      printf("\nProduziu");
      contador++;
   }
   else
   {

      if ((final + 1) < tamanho_fila)
      {
         //vejo se não preciso apontar o F para o inicio novamente

         final++;

         if (final == inicio)
         {
            printf("TO SOBREESCREVENDO !!!!!!!!!!!!");
            final--;
            
         }else{

          fila[final] = 1;
         printf("\nProduziu");
         contador++;  
         }

         
      }
      else
      {
         //volto pro inicio do vetor
         final = 0;

         if (final == inicio)
         {
            printf("TO SOBREESCREVENDO !!!!!!!!!!!!");
            final = tamanho_fila - 1;
            
         }else{
         fila[final] = 1;
         printf("\nProduziu");
         contador++;   
         }

         
      }
   }

   }

pthread_mutex_unlock(&mtx);
   

   /*
//estado inicial
//[1,0,0]
//i=0
//f=0

if(inicio==final && fila[inicio]==1){
   final++;
   fila[final]=1;
   printf("\nProduziu");
   return;
}

//estado inicial
//[1,1,0]
//i=0
//f=1

//apontar para a proxima posição

if((final+1) < tamanho_fila){
   //vejo se não preciso apontar o F para o inicio novamente
   final++;
   fila[final]=1;
   printf("\nProduziu");
   return;
}
//estado inicial
//[1,1,1]
//i=0
//f=2
*/
}

void imprime()
{
   //imprime a fila, com o seus itens

   printf("\nFila-> ");

   for (int i = 0; i < tamanho_fila; i++)
   {
      printf(": %d :", fila[i]);
   }
   printf("\n");

   printf("inicio %d   Final  %d   Contador  %d\n", inicio, final, contador);
}

int main(int argc, char **argv)
{
   //valores inciais das variaveis
   inicio = 0;
   final = 0;
   tamanho_fila = 3;
   contador = 0;

/*
   consumidor();
   produtor();
   consumidor();
   produtor();
   consumidor();
   consumidor();
   produtor();
   consumidor();
   produtor();
   consumidor();
   produtor();
   consumidor();
   produtor();
   consumidor();
   produtor();
   consumidor();*/
   
	pthread_t threads[NUM_THREADS];
	long int rc, t;

  pthread_mutex_init(&mtx, NULL);

	for (t = 0; t < NUM_THREADS; t++) {
      if(t%2==0){
         printf("Criando Produtor !!!!\n");
         rc = pthread_create(&threads[t], NULL, produtor, (void *)t);
		   if (rc) {
			printf("ERROR code is %d\n", (int)rc);
			return -1;
		   }
      }else{

        printf("Criando Consumidor !!!!\n");
         rc = pthread_create(&threads[t], NULL, consumidor, (void *)t);
		   if (rc) {
			printf("ERROR code is %d\n",(int) rc);
			return -1;
		   }


      }
	}
	
	pthread_exit(0);
   
}