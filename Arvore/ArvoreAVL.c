#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct no
{
    int info;
    struct no *esq, *dir;
};
/*  0 : equilibrada;
    menos 1 : sub-árvore esquerda tem mais um nível que a da direita;
    1 : sub-árvore direita tem mais um nível que a da esquerda.*/
int Altura(struct no *arvore)
{
    /*É a distância entre um nó e o seu descendente mais afastado. Mas precisamente, a altura de v é o número de passos do mais
    longo caminho que leva de v até uma folha. Os nós folha sempre têm altura igual a 0;
    A altura de uma árvore T é dada pela altura da raiz da árvore*/
    if (arvore == NULL)
        return -1;
    int he = Altura(arvore->esq);
    int hd = Altura(arvore->dir);
    if (he > hd)
        return he+1;
    return hd+1;
}
void RotacionaDir(struct no **arvore)
{
    struct no *aux;
    aux = (*arvore)->esq;
    (*arvore)->esq = aux->dir;
    aux->dir = *arvore;
    *arvore = aux;
}
void RotacionaEsq(struct no **arvore)
{
    struct no *aux;
    aux = (*arvore)->dir;
    (*arvore)->dir = aux->esq;
    aux->esq = *arvore;
    *arvore = aux;
}
int FatorBalanceamento(struct no *NO)
{
    return(Altura(NO->dir)-Altura(NO->esq));
}
short InserirRec(struct no **arvore, int elem)
{
    int FB;
    if(*arvore==NULL)   //Arvore vazia
    {
        struct no *novo;
        novo = (struct no *)malloc(sizeof(struct no));
        if(novo==NULL)
            return(0);
        novo->info = elem;
        novo->dir = NULL;
        novo->esq = NULL;
        *arvore = novo;
    }
    else
    {
        if(elem < (*arvore)->info)
            InserirRec(&(*arvore)->esq, elem);
        else if(elem > (*arvore)->info)
            InserirRec(&(*arvore)->dir, elem);
        else        //É igual, então não insere, pois não desejamos elementos repetidos
            return(0);
        //      BALANCEAMENTO
        FB = FatorBalanceamento(*arvore);
        if(FB>=2)   //A arvore esta desbalanceada a direita, ou seja, a altura da direita é maior
        {
            if(FatorBalanceamento((*arvore)->dir)>0)  //Mesmo sinal
                RotacionaEsq(&(*arvore));
            else
            {
                RotacionaDir(&((*arvore)->dir));
                RotacionaEsq(&(*arvore));
            }
        }
        else if(FB<=-2)    //A arvore esta desbalanceada a esquerda, ou seja, a altura da esquerda é maior
        {
            if(FatorBalanceamento((*arvore)->esq)>0)
            {
                RotacionaEsq(&((*arvore)->esq));
                RotacionaDir(&(*arvore));
            }
            else        //Mesmo sinal
                RotacionaDir(&(*arvore));
        }
    }
//Se houver um nó desbalanceado (abs(FB)>1), devemos analisar os dois casos possiveis
//Caso 1: se um nó tem fator de balanceamento com o MESMO SINAL que o fator de balanceamento do seu filho:
//          Solução: rotação simples sobre o nó de FB=2 (-2).
//          Rotações são feitas à esquerda quando FB positivo e à direita quando FB negativo.
//Caso 2: se um nó tem fator de balanceamento com o SINAL OPOSTO que o fator de balanceamento do seu filho:
//          Solução: duas rotações
//          1. primeiro roda-se o nó com FB=1 (-1) na direção apropriada
//          2. depois roda-se o nó que tinha FB=-2 (2) na direção oposta
}
struct no* Minimo(struct no *arvore)
{
    struct no *aux=NULL;
    for(aux=arvore ; aux->esq!=NULL ; aux=aux->esq);
    return(aux);
}
short RemoverRec(struct no **arvore, int elem)
{
    int FB;
    if(*arvore==NULL)   //Arvore vazia, não encontrou
        return(0);
    else
    {
        if(elem < (*arvore)->info)
            RemoverRec(&(*arvore)->esq, elem);
        else if(elem > (*arvore)->info)
            RemoverRec(&(*arvore)->dir, elem);
        //      BALANCEAMENTO
        FB = FatorBalanceamento(*arvore);
        if(FB>=2)   //A arvore esta desbalanceada a direita, ou seja, a altura da direita é maior
        {
            if(FatorBalanceamento((*arvore)->dir)>0)  //Mesmo sinal
                RotacionaEsq(&(*arvore));
            else
            {
                RotacionaDir(&((*arvore)->dir));
                RotacionaEsq(&(*arvore));
            }
        }
        else if(FB<=-2)    //A arvore esta desbalanceada a esquerda, ou seja, a altura da esquerda é maior
        {
            if(FatorBalanceamento((*arvore)->esq)>0)
            {
                RotacionaEsq(&((*arvore)->esq));
                RotacionaDir(&(*arvore));
            }
            else        //Mesmo sinal
                RotacionaDir(&(*arvore));
        }
        if((*arvore)->info==elem)            //Encontrou o elemento, vamos analisar os casos
        {
            struct no *aux=NULL;
            if((*arvore)->esq!=NULL && (*arvore)->dir!=NULL) //Caso 3: o nó possui duas sub-árvores -> O nó contendo o menor valor da sub-árvore direita (ou o maior valor da sub-árvore esquerda) pode “ocupar” o lugar;
            {
                aux = Minimo((*arvore)->dir);                   //Ou Maximo((*arvore)->esq);
                (*arvore)->info=aux->info;
                RemoverRec(&(*arvore)->dir, (*arvore)->info);   //Ou RemoverRec(&(*arvore)->esq...
            }
            else        //Ou não tem sub-arvore, ou só tem uma (deve-se analisar qual lado)
            {
                aux = *arvore;
                if((*arvore)->esq==NULL)        //Caso 2: o nó possui uma sub-árvore (esq./dir.) -> O nó-raiz da sub-árvore (esq./dir.) “ocupa” o lugar do nó retirado;
                    *arvore = (*arvore)->dir;
                else if((*arvore)->dir==NULL)
                    *arvore = (*arvore)->esq;
                free(aux);                      //Caso 1: o nó é folha -> O nó pode ser retirado sem problema;
            }
        }
    }
}
void PreOrdem(struct no *arvore)
{
    if(arvore!=NULL)
    {
        printf("%d ", arvore->info);
        PreOrdem(arvore->esq);
        PreOrdem(arvore->dir);
    }
}

int main(void)
{
    struct no *raiz=NULL;
    InserirRec(&raiz, 10);
    InserirRec(&raiz, 15);
    InserirRec(&raiz, 2);
    InserirRec(&raiz, 20);
    InserirRec(&raiz, 12);
    InserirRec(&raiz, 14);
    InserirRec(&raiz, 3);
    InserirRec(&raiz, 6);
    InserirRec(&raiz, 1);
    InserirRec(&raiz, 23);
    printf("\n  Pre-Ordem: ");
    PreOrdem(raiz);
    RemoverRec(&raiz, 2);
    RemoverRec(&raiz, 20);
    RemoverRec(&raiz, 14);
    RemoverRec(&raiz, 15);
    printf("\n  Pre-Ordem: ");
    PreOrdem(raiz);
    return(0);
}
