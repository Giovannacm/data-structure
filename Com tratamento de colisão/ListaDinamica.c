//Tratamento de colisão utilisando lista encadeada dinamica
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10

struct No
{
    int info;
    struct No *next;
};
struct tabelaHash
{
    struct No *Vet[MAX];
};
void inicializaTabela(struct tabelaHash *T)
{
    int i;
    for(i=0 ; i<MAX ; i++)
    {
        T->Vet[i] = NULL;
    }
}
int hash(int chave)
{
    int i, soma=0, lim;
    char s[10];
    itoa(chave, s, 10);
    lim = strlen(s);
    for ( i=0 ; i<lim ; i++)
    {
        soma = soma + (i+1)*s[i];
    }
    return (soma%MAX);
}
short inserir(struct tabelaHash *T, int elemento)
{
    int pos;
    struct No *p=NULL, *ant=NULL, *novo=NULL;
    pos = hash(elemento);
    novo = (struct No *)malloc(sizeof(struct No));
    if(novo==NULL)
        return 0;
    novo->info = elemento;
    novo->next = NULL;
    if(T->Vet[pos] == NULL)
        T->Vet[pos] = novo;
    else
    {
        for(p = T->Vet[pos] ; p!=NULL && p->info<elemento; p=p->next)
            ant = p;
        if(ant==NULL)   //Insere no inicio
        {
            novo->next=p;
            T->Vet[pos]=novo;
        }
        else if(p==NULL)//Insere no fim
            ant->next=novo;
        else    //Insere no meio
        {
            ant->next=novo;
            novo->next=p;
        }
    }
    return(1);
}
short remover(struct tabelaHash *T, int elemento)
{
    int pos;
    struct No *p=NULL, *ant=NULL;
    pos = hash(elemento);
    for(p=T->Vet[pos] ; p!=NULL && p->info<elemento ; p=p->next)
        ant = p;
    if(p!=NULL && p->info==elemento)    //Elemento encontrado
    {
        if(ant==NULL)   //Remove do inicio
        {
            T->Vet[pos]=p->next;
        }
        else
        {
            ant->next=p->next;
        }
        free(p);
        return 1;
    }
    return 0;
}
short buscar(struct tabelaHash *T, int elemento)
{
    int pos;
    struct No *p=NULL;
    pos = hash(elemento);
    for(p=T->Vet[pos] ; p!=NULL && p->info<=elemento ; p=p->next)
    {
        if(p->info == elemento)
            return 1;
    }
    return 0;
}
void imprimir(struct tabelaHash T)
{
    struct No *p;
    int i;
    for(i=0 ; i<MAX ; i++)
    {
        printf("\n Vet[%d]: ", i);
        if(T.Vet[i]==NULL)
            printf("NULL");
        else
        {
            for(p=T.Vet[i] ; p!=NULL ; p=p->next)
            {
                printf("%d ", p->info);
            }
        }
    }
}
int main (void)
{
    struct tabelaHash T1;
    inicializaTabela(&T1);
    inserir(&T1, 901);
    inserir(&T1, 5);
    inserir(&T1, 441);
    inserir(&T1, 24);
    inserir(&T1, 24);
    inserir(&T1, 96);
    inserir(&T1, 7);
    inserir(&T1, 8);
    inserir(&T1, 30);
    inserir(&T1, 45);
    if(buscar(&T1, 1))
        printf("\n Encontrei");
    else
        printf("\n Nao encontrei");
    imprimir(T1);
}
