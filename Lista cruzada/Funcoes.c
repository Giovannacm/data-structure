#include <stdio.h>
#include <stdlib.h>
#define MAXL 20
#define MAXC 20
struct no
{
    int valor, linha, coluna;
    struct no *baixo, *direira;
};
struct listacruz
{
    int nlinha, ncoluna;
    struct no *VL[MAXL], *VC[MAXC];
};
void inicializa(struct listacruz *L)
{
    int i, j;
    for(i=0 ; i<L->nlinha ; i++)
        L->VL[i]=NULL;
    for(j=0 ; j<L->ncoluna ; j++)
        L->VC[j]=NULL;
    return;
}
void imprime(struct listacruz L)
{
    int i, j;
    struct no *aux;
    for(i=0 ; i<L.nlinha ; i++)
    {
        aux=L.VL[i];
        printf("\n");
        for(j=0 ; j<L.ncoluna ; j++)
        {
            if(aux==NULL)
                printf(" 0");
            else
            {
                if(aux->coluna==j)
                {
                    printf(" %d", aux->valor);
                    aux=aux->direira;
                }
                else
                    printf(" 0");
            }

        }
    }
}
void inserir(struct listacruz *L, struct no N)
{
    struct no *aux, *p, *antl=NULL, *antc=NULL;
    aux=(struct no *)malloc(sizeof(struct no));
    aux->valor=N.valor;
    aux->linha=N.linha;
    aux->coluna=N.coluna;
    aux->baixo=NULL;
    aux->direira=NULL;
    if(L->VL[N.linha]==NULL)
    {
        L->VL[N.linha]=aux;
    }
    else
    {
        for(p=L->VL[N.linha] ; p!=NULL && p->coluna<N.coluna ; p=p->direira)
            antl=p;
        if(antl==NULL)      //Inserir no inicio
        {
            aux->direira=p;
            L->VL[N.linha]=aux;
        }
        else if(p==NULL)    //Insere no fim
            antl->direira=aux;
        else                //Insere no meio
        {
            aux->direira=p;
            antl->direira=aux;
        }
    }
    if(L->VC[N.coluna]==NULL)
    {
        L->VC[N.coluna]=aux;
    }
    else
    {
        for(p=L->VC[N.coluna] ; p!=NULL && p->linha<N.linha ; p=p->baixo)
            antc=p;
        if(antc==NULL)      //Insere no inicio
        {
            aux->baixo=p;
            L->VC[N.coluna]=aux;
        }
        else if(p==NULL)    //Insere no fim
            antc->baixo=aux;
        else                //Insere no meio
        {
            aux->baixo=p;
            antc->baixo=aux;
        }
    }
}
short remover(struct listacruz *L, struct no N)
{
    struct no *pl, *pc, *antl=NULL, *antc=NULL;
    for(pl=L->VL[N.linha] ; pl!=NULL && pl->coluna<N.coluna ; pl=pl->direira)
        antl=pl;
    if(pl==NULL)        //Não achou o elemento
        return(0);
    for(pc=L->VC[N.coluna] ; pc!=NULL && pc->linha<N.linha ; pc=pc->baixo)      //Ou pc!=pl
        antc=pc;
    if(antl==NULL)      //Remove do inicio
        L->VL[N.linha]=pl->direira;
    else                //Remove no meio
        antl->direira=pl->direira;
    if(antc==NULL)      //Remove do inicio
    {
        L->VC[N.coluna]=pc->baixo;
        free(pc);
    }
    else                //Remove no meio
    {
        antc->baixo=pc->baixo;
        free(pc);
    }
}
short somaKlinhaI(struct listacruz *L, int i, int k)
{
    struct no *p, N;
    for(p=L->VL[i]; p!=NULL ; p=p->direira)
    {
        p->valor+=k;
        if(p->valor==0)
        {
            N.linha=p->linha;
            N.coluna=p->coluna;
            remover(L, N);
        }
    }
}
void leitura(struct listacruz *L, int qtd)
{
    int i;
    struct no N1;
    for(i=0 ; i<qtd ; i++)
    {
        printf("\n      Linha: ");
        scanf("%d", &N1.linha);
        printf("\n      Coluna: ");
        scanf("%d", &N1.coluna);
        printf("\n      Valor: ");
        scanf("%d", &N1.valor);
        inserir(L, N1);
    }
}
void somaMatrizes(struct listacruz *L1, struct listacruz *L2, struct listacruz *L)
{
    struct no *aux1, *aux2, N;
    int i;
    if(L1->nlinha != L2->nlinha)
        return;
    if(L1->ncoluna != L2->ncoluna)
        return;
    for(i=0 ; i<L1->nlinha ; i++)       //Ou L2->nlinha, já que possuem tamanhos iguais
    {
        aux1=L1->VL[i];
        aux2=L2->VL[i];
        while(aux1!=NULL && aux2!=NULL)
        {
            if(aux1->coluna<aux2->coluna)
            {
                N.linha=aux1->linha;
                N.coluna=aux1->coluna;
                N.valor=aux1->valor;
                inserir(L, N);
                aux1=aux1->direira;
            }
            else if(aux2->coluna<aux1->coluna)
            {
                N.linha=aux2->linha;
                N.coluna=aux2->coluna;
                N.valor=aux2->valor;
                inserir(L, N);
                aux2=aux2->direira;
            }
            else
            {
                N.valor=aux1->valor + aux2->valor;
                if(N.valor!=0)                          //Se o valor for nulo, nem adiciona
                {
                    N.linha=aux1->linha;                //Poderia sr aux2;
                    N.coluna=aux1->coluna;
                    inserir(L, N);
                }
                aux1=aux1->direira;
                aux2=aux2->direira;
            }
        }
        while(aux1!=NULL)
        {
            N.linha=aux1->linha;
            N.coluna=aux1->coluna;
            N.valor=aux1->valor;
            inserir(L, N);
            aux1=aux1->direira;
        }
        while(aux2!=NULL)
        {
            N.linha=aux2->linha;
            N.coluna=aux2->coluna;
            N.valor=aux2->valor;
            inserir(L, N);
            aux2=aux2->direira;
        }
    }
}
short matrizIdentidade(struct listacruz *L)     //matriz quadrada em que todos os elementos da diagonal principal são iguais a 1 e os demais são nulos;
{
    int i;
    struct no *p;
    if(L->ncoluna != L->nlinha)
        return(0);          //Não é quadrada
    for(i=0 ; i<L->nlinha ; i++)
    {
        for(p=L->VL[i] ; p!= NULL ; p=p->direira)
        {
            if(p->valor!=1)
                return(0);
            if(p->linha != p->coluna)
                return(0);
        }
    }
    return(1);
}
void multiplicaMatrizes(struct listacruz *L1, struct listacruz *L2, struct listacruz *L)
{
    int i, j, valor=0;
    struct no *aux1, *aux2, N;
    if(L1->ncoluna != L2->nlinha)
        return;
    if(L1->nlinha != L2->ncoluna)
        return;
    for(i=0 ; i<L1->nlinha ; i++)    //L1->nlinha e L2->ncoluna
    {
        aux1=L1->VL[i];
        if(aux1==NULL)
            continue;
        for(j=0 ; j<L2->ncoluna ; j++)
        {
            aux1=L1->VL[i];
            aux2=L2->VC[j];
            if(aux2==NULL)
                continue;
            N.valor=0;
            N.linha=i;
            N.coluna=j;
            while(aux1!=NULL && aux2!=NULL)
            {
                if(aux1->coluna<aux2->linha)
                    aux1=aux1->direira;
                else if(aux2->linha<aux1->coluna)
                    aux2=aux2->baixo;
                else
                {
                    N.valor+=aux1->valor*aux2->valor;
                    aux1=aux1->direira;
                    aux2=aux2->baixo;
                }
            }
            if(N.valor!=0)
            {
                inserir(L, N);
            }
        }
    }
}
void converteMatriz(struct listacruz L, int *M)
{
    int i, j, k;
    struct no *aux;
    for(i=0 ; i<L.nlinha ; i++)
    {
        aux=L.VL[i];
        for(j=0 ; j<L.ncoluna ; j++)
        {
            if(aux==NULL)
                *(M + i*L.ncoluna + j)=0;
            else
            {
                if(aux->coluna==j)
                {
                    *(M + i*L.ncoluna + j)=aux->valor;
                    aux=aux->direira;
                }
                else
                {
                    *(M + i*L.ncoluna + j)=0;
                }
            }

        }
    }
}
void Impressao_Valores_Matriz(const int *A)
{
    int i, j;
    for  (i=0; i<4; i++)
    {
        printf("\n");
        for  (j=0; j<5; j++)
            printf(" %4d ", *(A + i*5 + j) );
    }
    return;
}

int main (void)
{
    int M[4][5];
    int quant, i, linha, k;
    struct listacruz L1, L2, LSoma, LMult;
    struct no N1;
    printf("\n      Numero de linhas: ");
    scanf("%d", &L1.nlinha);
    printf("\n      Numero de coluna: ");
    scanf("%d", &L1.ncoluna);
    inicializa(&L1);
    printf("\n      Quantidade de elementos a serem inseridos: ");
    scanf("%d", &quant);
    leitura(&L1, quant);
    imprime(L1);

    converteMatriz(L1, &M[0][0]);
    Impressao_Valores_Matriz(&M[0][0]);

    //printf("\n      Numero de linhas: ");
    //scanf("%d", &L2.nlinha);
    //printf("\n      Numero de coluna: ");
    //scanf("%d", &L2.ncoluna);
    //inicializa(&L2);
    //printf("\n      Quantidade de elementos a serem inseridos: ");
    //scanf("%d", &quant);
    //leitura(&L2, quant);
    //imprime(L2);

    /*printf("\n      Quantidade de elementos a serem removidos: ");
    scanf("%d", &quant);
    for(i = 0 ; i<quant ; i++)
    {
        printf("\n      Linha: ");
        scanf("%d", &N1.linha);
        printf("\n      Coluna:");
        scanf("%d", &N1.coluna);
        if(!remover(&L1, N1))
            printf("\n      ERRO :(");
    }
    imprime(L1);*/

    /*printf("\n      Somar os elementos da linha i a uma constante k: ");
    printf("\n      Valor K: ");
    scanf("%d", &k);
    printf("\n      Linha I: ");
    scanf("%d", &linha);
    somaKlinhaI(&L1, linha, k);
    imprime(L1);*/

    /*printf("\n      Somar duas matrizes: ");
    printf("\n      Numero de linhas: ");
    scanf("%d", &LSoma.nlinha);
    printf("\n      Numero de coluna: ");
    scanf("%d", &LSoma.ncoluna);
    inicializa(&LSoma);
    somaMatrizes(&L1, &L2, &LSoma);
    imprime(LSoma);*/

    /*printf("\n      Matriz identidade: ");
    if(matrizIdentidade(&L1))
        printf("\n      L1 eh uma matriz identidade!!!");
    else
        printf("\n      L1 NAO eh uma matriz identidade!!!");*/

    //printf("\n      Multiplicar duas matrizes: ");
    //LMult.nlinha=L1.nlinha;
    //LMult.ncoluna=L2.ncoluna;
    //inicializa(&LMult);
    //multiplicaMatrizes(&L1, &L2, &LMult);
    //imprime(LMult);

}
