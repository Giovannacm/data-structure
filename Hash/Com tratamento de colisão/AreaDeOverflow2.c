//Tratamento de colisão utilisando lista encadeada estatica + area de overflow (area de overflow está na própria lista)
//Teoricamente, a partir da posicao 10 de Vet, irá começar a area de overflow
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 15
#define Overf 10

struct No
{
    int info;
    int next;
};
struct tabelaHash
{
    int disp;
    struct No Vet[MAX];
};
void inicializaTabela(struct tabelaHash *T)
{
    int i;
    for(i=0 ; i<Overf ; i++)
    {
        T->Vet[i].info = -1;
        T->Vet[i].next = -1;
    }
    for(i=Overf ; i<MAX-1 ; i++)
    {
        T->Vet[i].next = i+1;
    }
    T->Vet[i].next = -1;
    T->disp=Overf;
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
    return (soma%Overf);
}
short inserir(struct tabelaHash *T, int elemento)
{
    int pos, p, ant, aux;
    pos = hash(elemento);
    if(T->disp==-1)     //Overflow cheio
        return(0);
    if(T->Vet[pos].next == -1 && T->Vet[pos].info == -1)
        T->Vet[pos].info = elemento;
    else    //Insere na atria de overflow
    {
        if(T->Vet[pos].next==-1)    //Não tem nenhum elemento na area de overflow
        {
            T->Vet[pos].next = T->disp;
        }
        else                        //Já tem elemento na area de overflow
        {
            for(p=T->Vet[pos].next ; T->Vet[p].next!=-1 ; p=T->Vet[p].next);
            T->Vet[p].next = T->disp;
        }
        aux=T->Vet[T->disp].next;
        T->Vet[T->disp].info = elemento;
        T->Vet[T->disp].next = -1;
        T->disp=aux;
    }
    return(1);
}
short remover(struct tabelaHash *T, int elemento)
{
    int pos, p, ant=-1, ind;
    pos = hash(elemento);
    if(T->Vet[pos].next== -1 && T->Vet[pos].info==-1)
        return(0);
    if(T->Vet[pos].info == elemento)  //Remove o elemento do Vet
    {
        if(T->Vet[pos].next==-1)
            T->Vet[pos].info=-1;
        else
        {
            ind = T->Vet[pos].next;
            T->Vet[pos].info = T->Vet[ind].info;
            T->Vet[pos].next = T->Vet[ind].next;
            T->Vet[ind].next = T->disp;
            T->disp=ind;
        }
    }
    else                            //Remove o elemento do Overf
    {
        for(p=T->Vet[pos].next ; p!=-1 && T->Vet[p].info != elemento ; p=T->Vet[p].next);
            ant = p;
        if(p==-1)
            return(0);
        if(ant==-1)
            T->Vet[pos].next=T->Vet[p].next;
        else
            T->Vet[ant].next= T->Vet[p].next;
        T->Vet[p].next = T->disp;
        T->disp=p;
    }
    return(1);
}
short buscar(struct tabelaHash *T, int elemento)
{
    int pos, p;
    pos = hash(elemento);
    if(T->Vet[p].next== -1 && T->Vet[p].info==-1)
        return(0);
    if(T->Vet[p].info == elemento)
        return(1);
    for(p=T->Vet[pos].next ; p!=-1 ; p=T->Vet[p].next);
    {
        if(T->Vet[p].info == elemento)
            return 1;
    }
    return 0;
}
void imprimir(struct tabelaHash T)
{
    int i;
    for(i=0 ; i<MAX ; i++)
    {
        printf("\n Vet[%d]: ", i);
        printf("%d          %d", T.Vet[i].info, T.Vet[i].next);
    }
    printf("\n\n [Disp]: %d", T.disp);
}
int main (void)
{
    struct tabelaHash T1;
    inicializaTabela(&T1);
    inserir(&T1, 82);
    inserir(&T1, 8);
    inserir(&T1, 44);
    inserir(&T1, 25);

    remover(&T1, 82);
    remover(&T1, 44);

    /*inserir(&T1, 901);
    inserir(&T1, 5);
    inserir(&T1, 441);
    inserir(&T1, 24);
    inserir(&T1, 24);
    inserir(&T1, 96);
    inserir(&T1, 7);
    inserir(&T1, 8);
    inserir(&T1, 30);
    inserir(&T1, 45);*/
    //if(buscar(&T1, 1))
    //    printf("\n Encontrei");
    //else
    //    printf("\n Nao encontrei");
    imprimir(T1);
}

