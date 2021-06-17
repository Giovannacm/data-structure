#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 20

struct Pessoa
{
    short Op, Cliente;
    struct Pessoa *next;
};
struct Fila
{
    struct Pessoa *ini, *fim;
    int cont;                   //Contador que conta o número de pessoas na fila
};
void Inicializa(struct Fila **C)
{
    int i=0;
    for(i=0 ; i<4 ; i++)
    {
        C[i] = (struct Fila *)malloc(sizeof (struct Fila));     //Alocando espaço de memoria para a struct Lista para cada caixa
        C[i]->ini = NULL;                                       //Inicializando os campos ini e fim do caixa
        C[i]->fim = NULL;
        C[i]->cont=0;
    }
}
void Imprime(struct Fila **C)
{
    int i=0;
    struct Pessoa *p=NULL;
    for(i=0 ; i<4 ; i++)
    {
        printf("\n\n      [CAIXA %d]", i+1);
        if(C[i]->ini==NULL)
            printf(" CAIXA VAZIO!");
        else
        {
            for(p=C[i]->ini ; p!=NULL ; p=p->next)
            {
                if(p->Cliente)
                    printf(" C-");
                else
                    printf(" NC-");
                if(p->Op)
                    printf("S ");
                else
                    printf("D ");
            }
        }
    }
}
short InserePessoa(struct Fila **C, int X, struct Pessoa PX)
{
    struct Pessoa *p=NULL;
    p=(struct Pessoa *)malloc(sizeof(struct Pessoa));
    if(p==NULL)
        return(0);
    p->Cliente = PX.Cliente;
    p->Op = PX.Op;
    p->next = NULL;
    if(C[X]->ini == NULL)
        C[X]->ini = p;
    else
        C[X]->fim->next = p;
    C[X]->fim = p;
    C[X]->cont++;
    return(1);
}
short Remove(struct Fila **C, int X)
{
    struct Pessoa *p=NULL;
    if(C[X]->ini==NULL)        //Fila vazia
        return(0);
    p=C[X]->ini;
    C[X]->ini=p->next;
    C[X]->cont--;
    free(p);
    return(1);
}
int main(void)
{
    struct Fila * Caixa[4];             //Vetor de ponteiros para a struct Fila -> só irá apontar para ini ou fim (já que são elementos da estrutura Fila)
    struct Pessoa P;                    //Struct Pessoa para receber os valores de rand() e ser passada como parametro na funcao inserir
    int TempoMAX=1000, Tempo=0, C, i=0, Menor, Pos=0;     //Variaveis auxiliares
    Inicializa(Caixa);                  //Inicializando o vetor de ponteiros para a Struct Lista
    Imprime(Caixa);
    srand(time(NULL));                  //Funcao auxiliar para a funcao rand()
    while(Tempo<TempoMAX)
    {
        if(Tempo%5==0)
        {
            P.Cliente=rand()%2;
            P.Op=rand()%2;
            C=rand()%4;
            //----------------------------Evita caixa vazio e outros caixas ocupados------------------------
            /*
            if(Caixa[C]->ini!=NULL)     //Se no caixa dado por rand() já está ocupado
            {
                for(i=0 ; i<4 ; i++)    //Busco por um caixa vazio
                {
                    if(i==C)
                        continue;
                    if(Caixa[i]->ini==NULL)
                    {
                        InserePessoa(Caixa, i, P);  //E insere no caixa vazio
                        break;
                    }
                }
                if(i==4)                //Não achou, então insere onde era para inserir
                    InserePessoa(Caixa, C, P);
            }
            else
                InserePessoa(Caixa, C, P);
            */
            //----------------------------------------------------------------------------------------------

            //-----------------Evita numero de clientes distribuidos de formas desproporcionais-------------

            if(Caixa[C]->ini!=NULL)     //Se no caixa dado por rand() já está ocupado
            {
                for(i=0 ; i<4 ; i++)    //Busco por um caixa vazio
                {
                    if(i==C)
                        continue;
                    if(Caixa[i]->ini==NULL)
                    {
                        InserePessoa(Caixa, i, P);  //E insere no caixa vazio
                        break;
                    }
                }
                if(i==4)                //Não achou, então insere onde era para inserir
                {
                    Pos=C;
                    Menor=Caixa[C]->cont;
                    for(i=0 ; i<4 ; i++)    //Busco por um caixa vazio
                    {
                        if(i==C)
                            continue;
                        if(Caixa[i]->cont<Menor)
                        {
                            Menor=Caixa[i]->cont;
                            Pos=i;
                        }
                    }
                    InserePessoa(Caixa, Pos, P);
                }
            }
            else
                InserePessoa(Caixa, C, P);
            //----------------------------------------------------------------------------------------------

            system("cls");
            Imprime(Caixa);
            //printf("\n      C= %d", C+1);
            //printf("\n      Pos=%d", Pos+1);
            printf("\n\n");
            //system("pause");
        }

        if(Tempo%6==0)
        {
            C=rand()%4;
            if(Caixa[C]->ini!=NULL)
            {
                Remove(Caixa, C);
                system("cls");
                Imprime(Caixa);
                printf("\n\n");
                //system("pause");
            }
        }
        Tempo++;
    }
    return(0);
}
/*
void PreencheFicha(struct Pessoa *Pessoa)
{
    printf("\n      [NOME]: ");
    //scanf("%s", Pessoa->Nome);
    printf("\n      [OPERACAO] [1]Saque  [2]Deposito: ");
    scanf("%d", &(Pessoa->Op));
    printf("\n      [CLIENTE] [0]Nao  [1]Sim: ");
    scanf("%d", &(Pessoa->Cliente));
}
    Caixa[0] = clientes para saque
    Caixa[1] = não clientes para saque
    Caixa[2] = clientes para deposito
    Caixa[3] = não clientes para deposito
*/
