/*A fun��o HASH, ou fun��o de espalhamento, mapeia um conjunto de chaves em
uma tabela de tamanho fixo de acordo com um conjunto de valores.
Ou seja, dado um valor existe uma chave respectiva e de modo unidirecional.*/

/*Implementa��o: Realizada por meio de um vetor de tamanho fixo.
Regras: Cada valor possui uma chave respectiva obtida por uma fun��o.
Aplica��o: Utilizada na otimiza��o de algoritmos e armazenamento tempor�rio de dados.
Requisitos para aplica��o: Dados podem ser sobrescrevidos.
Vantagem: Pode se armazenar em qualquer ordem e � de f�cil implementa��o.
Desvantagem: A tabela n�o pode ser muito grande, pois ter� grande gasto de mem�ria.
A fun��o Hash tem que possuir um perfeito n�vel de espalhamento para n�o ocorrer colis�o, caso contr�rio, muitos dados ser�o perdidos.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10

struct tabelaHash
{
    int Vet[MAX];
};
void inicializaTabela(struct tabelaHash *T)
{
    int i;
    for(i=0 ; i<MAX ; i++)
    {
        T->Vet[i] = -1;
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
short inserir(struct tabelaHash *T, int chave)
{
    int pos;
    pos = hash(chave);
    T->Vet[pos] = chave;
}
short remover(struct tabelaHash *T, int chave)
{
    int pos;
    pos = hash(chave);
    if(T->Vet[pos]!=-1 && T->Vet[pos]==chave)
    {
        T->Vet[pos]=-1;
        return(1);
    }
    return(0);
}
short buscar(struct tabelaHash *T, int chave)
{
    int pos;
    pos = hash(chave);
    if(T->Vet[pos]!=-1 && T->Vet[pos]==chave)
        return(1);
    return(0);
}

int main(void)
{

}
