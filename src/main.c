
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

/* Meu codigo adapta uma pilha que armazena posicao para fazer os processos em paralelo */
int isPrimo(int n){
    if (n==0) return 0;
    if (n==1) return 0;
    int i, divisores=0;
    for (i=2;i<=n;i++){
        if (n%i==0){
            divisores++;
            //printf("%d divide %d", i, n);
        }
        if (divisores==2) return 0;
    }
    //printf("%d Primo\n", n);
    return 1;
}
int isPai(pid_t filho[]){
    if(filho[0]==0 | filho[1]==0 | filho[2]==0) return 0;
    return 1;
}
int main() {
    pid_t filho[4];
    int i;
    char aux;
    
    /* Definir flags de protecao e visibilidade de memoria */
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;
    
    /* Criar area de memoria compartilhada */
    int *memoria;
    memoria = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
    if ((long int)memoria==-1) printf("Erro de alocacao!\n");
    memoria[14]=0; //Posicao na pilha
    memoria[15]=0; //Numero de primos
    
    //Captar entrada
    while (1){
        scanf("%d", &memoria[memoria[14]]);
        scanf("%c", &aux);
        //printf("%d, %d\n", memoria[memoria[14]], memoria[14]);
        if (aux == '\n') break;
        memoria[14]++;
    }
    //iniciar processos paralelos, contando com o pai sempre serao 4
    for (i=0;i<3;i++){
        filho[i]=fork();
        if(filho[i]==0) break;
    }
    //comeca o programa
    while (1){
        if(memoria[14]==-1){
            if (isPai(filho)){
                for (i=0;i<3;i++){
                    waitpid(filho[i], NULL, 0);
                }
                printf("%d\n", memoria[15]);
                return 0;
            } else {
                exit(0);
            }
        }
        memoria[14]--;
        memoria[15]+=isPrimo(memoria[memoria[14]+1]);
    }
    return 0;
}

