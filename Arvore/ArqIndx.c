#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct indice
{
    int chave, end;
};
struct dados
{
    int chave, end;
    float saldo;
    char nome[30];
};
void inserirIndice(FILE *arq, int pos, int chave, int end)
{
    struct indice molde;
    molde.chave = chave;
    molde.end = end;
    fseek(arq, pos*sizeof(struct indice), SEEK_SET);
    fwrite(&molde, sizeof(struct indice), 1, arq);
}
void inserirDados(FILE *arq, int pos, int chave, int end, char *nome, float saldo)
{
    struct dados molde;
    molde.chave = chave;
    molde.end = end;
    molde.saldo = saldo;
    strcpy(molde.nome, nome);
    fseek(arq, pos*sizeof(struct dados), SEEK_SET);
    fwrite(&molde, sizeof(struct dados), 1, arq);
}
struct dados* buscaBinaria(FILE *ind, FILE *dados, int chave, int ini, int fim)     //Recursiva
{
    struct indice atual;
    struct dados molde, *ptrDado;
    if(fim<ini)
        return(NULL);
    int meio = (ini+fim)/2;
    fseek(ind, sizeof(struct indice)*meio, SEEK_SET); //3 pois (caracter)+(\n)+(voltalinha)
    fread(&atual, sizeof(struct indice), 1, ind);
    //printf("\n %d", atual.chave);
    if(chave<atual.chave)
        buscaBinaria(ind, dados, chave, ini, meio-1);
    else if(chave>atual.chave)
        buscaBinaria(ind, dados, chave, meio+1, fim);
    else
    {
        fseek(dados, sizeof(struct dados)*(atual.end), SEEK_SET);
        fread(&molde, sizeof(struct dados), 1, dados);
        ptrDado = &molde;
        return(ptrDado);
    }
}

int main (void)
{
    int i;
    struct indice auxI;
    struct dados auxD, *dadoBusca;
    FILE *Indice, *Dados;
    if((Indice = fopen("indice.bin", "wb+"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    if((Dados = fopen("dados.bin", "wb+"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }

    //----Criando a tabela de indice----
    inserirIndice(Indice, 0, 45128, 006);
    inserirIndice(Indice, 1, 70918, 001);
    inserirIndice(Indice, 2, 121267, 002);
    inserirIndice(Indice, 3, 160252, 005);
    inserirIndice(Indice, 4, 166702, 003);
    inserirIndice(Indice, 5, 378845, 004);
    inserirIndice(Indice, 6, 379452, 000);

    //----Criando a tabela de dados----
    inserirDados(Dados, 0, 000, 379452, "Mary Dodd", 1432.45);
    inserirDados(Dados, 1, 001, 70918, "Sarah Trapp", 100.22);
    inserirDados(Dados, 2, 002, 121267, "Bryan Devaux", 11.45);
    inserirDados(Dados, 3, 003, 166702, "Harry Eagle", 14321.00);
    inserirDados(Dados, 4, 004, 378845, "John Carver", 7234.01);
    inserirDados(Dados, 5, 005, 160252, "Tuan Ngo", 15121.10);
    inserirDados(Dados, 6, 006, 45128, "Shouli Feldman", 87922.05);

    //----Impressão dos indices----
    printf("\n  Tabela de indices");
    for(i=0 ; i<7 ; i++)
    {
        fseek(Indice, sizeof(struct indice)*i, SEEK_SET);
        fread(&auxI, sizeof(struct indice), 1, Indice);
        printf("\n  %d  %d  ", auxI.chave, auxI.end);
    }

    //----Impressão dos dados----
    printf("\n\n  Tabela de dados");
    for(i=0 ; i<7 ; i++)
    {
        fseek(Dados, sizeof(struct dados)*i, SEEK_SET);
        fread(&auxD, sizeof(struct dados), 1, Dados);
        printf("\n  %d  %d  %s  %.2f", auxD.chave, auxD.end, auxD.nome, auxD.saldo);
    }

    //----Busca----
    dadoBusca = buscaBinaria(Indice, Dados, 70918, 0, 6);
    if(dadoBusca!=NULL)
        printf("\n\n  [Busca]   %d  %d  %s  %.2f  \n", dadoBusca->chave, dadoBusca->end, dadoBusca->nome, dadoBusca->saldo);
    else
        printf("\n\n  [Busca]   Dado nao encontrado!");

    fclose(Indice);
    fclose(Dados);
    return(0);
}
