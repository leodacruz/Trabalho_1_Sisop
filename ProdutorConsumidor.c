#define NUM_THREADS 10
#define TAMANHO_FILA 5

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int inicioFila;
int finalFila;
int contadorFila;
int fila[TAMANHO_FILA];
pthread_mutex_t mtx;

void *produtor();
void *consumidor();

void *consumidor(void *threadid)
{
   int confirmaConsumo=1;                 //Confirmar que consumiu o recurso
   int tid = (int)(long int)threadid;     //ID da Thread pro Print

   
   while(confirmaConsumo){                //fica num loop até a thread consumir o recurso

      while(!contadorFila>0);                 //Parar o Consumidor

   pthread_mutex_lock(&mtx);              //Inicio da seção critica
  
   if (fila[inicioFila] == 1)                 //ve se da para consumir o recurso da fila
   {
      fila[inicioFila] = 0;                   //ato do consumo

      if(inicioFila!=finalFila){                  //posicionar o inicio da fila

         if (inicioFila + 1 < TAMANHO_FILA)
         {
            inicioFila++;
         }
         else
         {
            if(fila[0]==1){
            inicioFila = 0; 
            }
         }
      }

      contadorFila--;                         // controle de recursos
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

      while(contadorFila==TAMANHO_FILA);         //Parar o Consumidor

   pthread_mutex_lock(&mtx);                 //Inicio da seção critica

   
   if(contadorFila==TAMANHO_FILA){              //Controle para threads que sairam do loop ao mesmo tempo
      printf("-> [Produtor %d Não produziu o conteudo!]\n",tid);
   }else{
      
      if (fila[inicioFila] == 0)                //Fila vazia
      {
         fila[finalFila] = 1;                   //Ato de produzir recurso
         contadorFila++;                        //Controle de Recurso    
         confirmaproducao=0;                      
         printf("Produtor ID: %d produziu o recurso\n",tid);
      }
         else                               //Fila está com algum recurso
         {
            if ((finalFila + 1) < TAMANHO_FILA)  //Se o finalFila da fila não esta na ultima pos vetor
            {
               finalFila++;                      //ponteiro do finalFila move para o prox posicao vetor
               fila[finalFila] = 1;              //ato de produzir
               contadorFila++;                   //Controle de  Recurso
               confirmaproducao=0;
               printf("Produtor ID: %d produziu o recurso\n",tid);  
            }
               else
               {
                  finalFila = 0;                    //volto pro inicio do vetor
                  fila[finalFila] = 1;              //ato de produzir
                  contadorFila++;                   //Controle de  Recurso
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
 
   inicioFila = 0;
   finalFila = 0;
   contadorFila = 0;

	pthread_t threads[NUM_THREADS];
	long int rc, t;

   pthread_mutex_init(&mtx, NULL);
   
   //Criação daas Threads
	for (t = 0; t < NUM_THREADS; t++) {
      if(t<(NUM_THREADS/2)){
        printf("Criando Consumidor ID: %d\n",(int)t);
        rc = pthread_create(&threads[t], NULL, consumidor, (void *)t);
      }else{
         printf("Criando Produtor ID: %d\n",(int)t);
         rc = pthread_create(&threads[t], NULL, produtor, (void *)t);
      }

      if (rc) {
			printf("ERROR code is %d\n",(int) rc);
			return -1;
		}
	}

	pthread_exit(0);
}
