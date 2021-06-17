/*A função HASH, ou função de espalhamento, mapeia um conjunto de chaves em
uma tabela de tamanho fixo de acordo com um conjunto de valores.
Ou seja, dado um valor existe uma chave respectiva e de modo unidirecional.*/

/*Implementação: Realizada por meio de um vetor de tamanho fixo.
Regras: Cada valor possui uma chave respectiva obtida por uma função.
Aplicação: Utilizada na otimização de algoritmos e armazenamento temporário de dados.
Requisitos para aplicação: Dados podem ser sobrescrevidos.
Vantagem: Pode se armazenar em qualquer ordem e é de fácil implementação.
Desvantagem: A tabela não pode ser muito grande, pois terá grande gasto de memória.
A função Hash tem que possuir um perfeito nível de espalhamento para não ocorrer colisão, caso contrário, muitos dados serão perdidos.*/

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
