//Caso a raiz esteja na posição zero, dado um nó de índice i qualquer, os seus filhos terão índices 2i + 1 e 2i + 2.
#include <stdio.h>
#define MAX 50

void imprime(int *arvore)
{
    int i;
    for(i=0 ; i<MAX ; i++)
        printf("%d ", arvore[i]);
}
int pai(int i)
{
    return (i-1)/2;
}
int esq(int i)
{
    return 2*i + 1;
}
int dir(int i)
{
    return 2*i + 2;
}
short inserir(int *arvore, int elem, int i)
{
    if(arvore[i]==-1)   //Arvore vazia
        arvore[i]=elem;
    else
    {
        if(elem < arvore[i])
            inserir(arvore, elem, esq(i));
        else if(elem > arvore[i])
            inserir(arvore, elem, dir(i));
        else        //É igual, então não insere, pois não desejamos elementos repetidos
            return(0);
    }
}
void InOrdem(int *arvore, int i)
{
    if(arvore[i]!=-1)
    {
        InOrdem(arvore, esq(i));
        printf("%d ", arvore[i]);
        InOrdem(arvore, dir(i));
    }
}
short EstaVazia(int *arvore)
{
    if(arvore[0]==-1)
        return(1);
    return(0);
}

int main(void)
{
    int arvore[MAX];
    int i;
    for(i=0 ; i<MAX ; i++)
        arvore[i]=-1;
    inserir(arvore, 10, 0);
    inserir(arvore, 4, 0);
    inserir(arvore, 12, 0);
    inserir(arvore, 2, 0);
    inserir(arvore, 6, 0);
    inserir(arvore, 11, 0);
    inserir(arvore, 20, 0);
    inserir(arvore, 1, 0);
    inserir(arvore, 3, 0);
    inserir(arvore, -7, 0);
    imprime(arvore);
    printf("\n  In-Ordem: ");
    InOrdem(arvore, 0);
    return(0);
}
