#include <stdio.h>
#include <stdlib.h>
#define MAX 50
//Implementação de arvore binaria de busca em arquivo

struct no
{
    int info;
    int dir, esq;
};
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
short Inserir(FILE *arvore, int elem, int i)
{
    struct no NoMolde;
    fseek(arvore, sizeof(struct no)*i, SEEK_SET);
    if(fread(&NoMolde, sizeof(struct no), 1, arvore)==0)   //Arvore vazia
    {
        NoMolde.info = elem;
        NoMolde.esq = -1;
        NoMolde.dir = -1;
        fseek(arvore, sizeof(struct no)*i, SEEK_SET);
        fwrite(&NoMolde, sizeof(struct no), 1, arvore);
    }
    else
    {
        if(elem < NoMolde.info)
        {
            Inserir(arvore, elem, esq(i));
            NoMolde.esq = esq(i);
        }
        else if(elem > NoMolde.info)
        {
            Inserir(arvore, elem, dir(i));
            NoMolde.dir = dir(i);
        }
        else
            return(0);
        //Atualizando o valor de esq e dir após a insercao
        fseek(arvore, sizeof(struct no)*i, SEEK_SET);   //Volta na raiz
        fwrite(&NoMolde, sizeof(struct no), 1, arvore); //Escreve os novos valores
    }
}
void InOrdem(FILE *arvore, int i)
{
    struct no NoMolde;
    fseek(arvore, sizeof(struct no)*i, SEEK_SET);
    if(fread(&NoMolde, sizeof(struct no), 1, arvore)!=0)
    {
        InOrdem(arvore, NoMolde.esq);
        printf(" %d", NoMolde.info);
        InOrdem(arvore, NoMolde.dir);
    }
}
void ImpressaoArquivo(FILE *arq)
{
    int i=0;
    struct no NoMolde;
    fseek(arq, i*(sizeof(struct no)), SEEK_SET);
    fread(&NoMolde, sizeof(struct no), 1, arq);
    while(!feof(arq))
    {
        printf("\n     %d      info: %d   esq: %d   dir: %d", i, NoMolde.info, NoMolde.esq, NoMolde.dir);
        i++;
        fseek(arq, i*(sizeof(struct no)), SEEK_SET);
        fread(&NoMolde, sizeof(struct no), 1, arq);
    }
}
int main(void)
{
    FILE *arv;
    if((arv = fopen("ArvArq.bin", "w+b"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    Inserir(arv, 10, 0);
    Inserir(arv, 4, 0);
    Inserir(arv, 12, 0);
    Inserir(arv, 2, 0);
    Inserir(arv, 6, 0);
    Inserir(arv, 11, 0);
    Inserir(arv, 20, 0);
    Inserir(arv, 1, 0);
    Inserir(arv, 3, 0);
    Inserir(arv, -7, 0);
    InOrdem(arv, 0);
    ImpressaoArquivo(arv);
    fclose(arv);
}
