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

void *produtor();
void *consumidor();
void mutexManual(); //falta implementar


void mutexManual(){

}



void *consumidor(void *threadid)
{
   int confirmaConsumo=1;                 //Confirmar que consumiu o recurso
   int tid = (int)(long int)threadid;     //ID da Thread pro Print

   
   while(confirmaConsumo){                //fica num loop até a thread consumir o recurso

      while(!contador>0);                 //Parar o Consumidor

   pthread_mutex_lock(&mtx);              //Inicio da seção critica
  
   if (fila[inicio] == 1)                 //ve se da para consumir o recurso da fila
   {
      fila[inicio] = 0;                   //ato do consumo

      if(inicio!=final){                  //posicionar o inicio da fila

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

      contador--;                         // controle de recursos
      confirmaConsumo=0;                  // sair do primeiro loop
      
      printf("Consumidor ID: %d Consumiu o recurso\n",tid);
   }
   
   //quando entra, mas não da para consumir
   if(confirmaConsumo==1){
      printf("-> [Consumidor ID: %d Não Consumiu o recurso!]\n",tid);  
   }
   
   pthread_mutex_unlock(&mtx);
   }
   

}

void *produtor(void *threadid)
{
   int confirmaproducao=1;                   //Confirmar que produziu o recurso
   int tid = (int)(long int)threadid;        //ID da Thread pro Print
   


   while(confirmaproducao){                  //fica num loop até o thread produzir o recurso

      while(contador==tamanho_fila);         //Parar o Consumidor

   pthread_mutex_lock(&mtx);                 //Inicio da seção critica

   
   if(contador==tamanho_fila){              //Controle para threads que sairam do loop ao mesmo tempo
      printf("-> [Produtor %d Não produziu o conteudo!]\n",tid);
   }else{
      
      if (fila[inicio] == 0)                //Fila vazia
      {
         fila[final] = 1;                   //Ato de produzir recurso
         contador++;                        //Controle de Recurso    
         confirmaproducao=0;                      
         printf("Produtor ID: %d produziu o recurso\n",tid);
      }
         else                               //Fila está com algum recurso
         {
            if ((final + 1) < tamanho_fila)  //Se o final da fila não esta na ultima pos vetor
            {
               final++;                      //ponteiro do final move para o prox posicao vetor
               fila[final] = 1;              //ato de produzir
               contador++;                   //Controle de  Recurso
               confirmaproducao=0;
               printf("Produtor ID: %d produziu o recurso\n",tid);  
            }
               else
               {
                  final = 0;                    //volto pro inicio do vetor
                  fila[final] = 1;              //ato de produzir
                  contador++;                   //Controle de  Recurso
                  confirmaproducao=0; 
                  printf("Produtor ID: %d produziu o recurso\n",tid);  
               }
         }
   }

   pthread_mutex_unlock(&mtx);

   } 
}

int main(int argc, char **argv)
{
 
   inicio = 0;
   final = 0;
   tamanho_fila = 3;
   contador = 0;

	pthread_t threads[NUM_THREADS];
	long int rc, t;

   pthread_mutex_init(&mtx, NULL);
   
   //Criação daas Threads
	for (t = 0; t < NUM_THREADS; t++) {
      if(t<5){
         printf("Criando Produtor ID: %d\n",t);
         rc = pthread_create(&threads[t], NULL, produtor, (void *)t);
      }else{
         printf("Criando Consumidor ID: %d\n",t);
         rc = pthread_create(&threads[t], NULL, consumidor, (void *)t);
      }

      if (rc) {
			printf("ERROR code is %d\n",(int) rc);
			return -1;
		}
	}


	pthread_exit(0);
   
}
