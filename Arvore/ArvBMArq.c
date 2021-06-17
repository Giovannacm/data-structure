#include <stdio.h>
#include <stdlib.h>
#define n 3
#define t 2

int raiz;
struct noB
{
    int nroChavesArm, indice[n+1]; //Se por folha, indice[0] ser� usado para a lista encadeada das folhas
    char chave[n];
    short folha;
};
void inserirNoArquivo(FILE *arv, int indice, int nroChavesArm, short folha, char a, char b, char c, int d, int e, int f, int g)
{
    int i;
    struct noB molde;
    molde.nroChavesArm = nroChavesArm;
    molde.folha = folha;
    molde.chave[0] = a;
    molde.chave[1] = b;
    molde.chave[2] = c;
    molde.indice[0] = d;
    molde.indice[1] = e;
    molde.indice[2] = f;
    molde.indice[3] = g;
    fseek(arv, indice*sizeof(struct noB), SEEK_SET);
    fwrite(&molde, sizeof(struct noB), 1, arv);
}

/*Dessa forma quando buscamos uma chave k, percorremos a �rvore de cima para baixo carregando as p�ginas internas e selecionando a p�gina
apontada pelo ponteiro correspondente ao intervalo no qual pertence k e caso uma c�pia de k esteja numa p�gina interna devemos carregar a
p�gina � direita de k. Encontrado uma p�gina folha o algoritmo deve buscar k nesta e responder se ela se encontra ou n�o.*/

short buscaArvBM(FILE *arv, struct noB no, char elem)
{
    int i;
    struct noB molde;
    if(!no.folha)           //Se n�o � folha, basta percorrer os indices at� encontrar um n� folha
    {
        for(i=0 ; i<no.nroChavesArm && elem>=no.chave[i] ; i++);
        printf("\n 1  [%c] [%d]", no.chave[i], no.indice[i]);
        fseek(arv, no.indice[i]*sizeof(struct noB), SEEK_SET);
        fread(&molde, sizeof(struct noB), 1, arv);
        return(buscaArvBM(arv, molde, elem));
    }
    else                    //Se � folha, basta percorrer o vetor de chaves e ver se elem est� l�
    {
        for(i=0 ; i<no.nroChavesArm && elem>no.chave[i] ; i++);
        printf("\n 2  [%c] [%d]", no.chave[i], no.indice[i]);
        if(i<no.nroChavesArm && elem==no.chave[i])
            return(1);
        else
            return(0);
    }
}

int main(void)
{
    FILE *arvB;
    struct noB aux;
    int i, j;
    if((arvB = fopen("ArvBM.bin", "wb+"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    //----Gravando em um arquivo bin�rio
    inserirNoArquivo(arvB, 0, 2, 0, 'K', 'Q', ' ', 1, 2, 3, -1);
    inserirNoArquivo(arvB, 1, 2, 0, 'C', 'F', ' ', 4, 5, 6, -1);
    inserirNoArquivo(arvB, 2, 1, 0, 'M', ' ', ' ', 7, 8, -1, -1);
    inserirNoArquivo(arvB, 3, 3, 0, 'T', 'W', 'Y', 9, 10, 11, 12);
    inserirNoArquivo(arvB, 4, 2, 1, 'A', 'B', ' ', 5, -1, -1, -1);
    inserirNoArquivo(arvB, 5, 3, 1, 'C', 'D', 'E', 6, -1, -1, -1);
    inserirNoArquivo(arvB, 6, 2, 1, 'F', 'H', ' ', 7, -1, -1, -1);
    inserirNoArquivo(arvB, 7, 2, 1, 'K', 'L', ' ', 8, -1, -1, -1);
    inserirNoArquivo(arvB, 8, 3, 1, 'M', 'N', 'P', 9, -1, -1, -1);
    inserirNoArquivo(arvB, 9, 3, 1, 'Q', 'R', 'S', 10, -1, -1, -1);
    inserirNoArquivo(arvB, 10, 2, 1, 'T', 'V', ' ', 11, -1, -1, -1);
    inserirNoArquivo(arvB, 11, 2, 1, 'W', 'X', ' ', 12, -1, -1, -1);
    inserirNoArquivo(arvB, 12, 2, 1, 'Y', 'Z', ' ', -1, -1, -1, -1);

    //----Colocando n� raiz na mem�ria----
    fseek(arvB, 0*sizeof(struct noB), SEEK_SET);
    fread(&aux, sizeof(struct noB), 1, arvB);

    //----Fazendo a busca
    if(buscaArvBM(arvB, aux, 'G'))
        printf("\n  ENCONTREI!");
    else
        printf("\n  NAO ENCONTREI!");

    //----Impress�o----
    for(i=0 ; i<=11 ; i++)
    {
        fseek(arvB, sizeof(struct noB)*i, SEEK_SET);
        fread(&aux, sizeof(struct noB), 1, arvB);
        printf("\n  %d  %d  ", aux.nroChavesArm, aux.folha);
        for(j=0 ; j<n ; j++)
        {
            printf("%c  ", aux.chave[j]);
        }
        for(j=0 ; j<n+1 ; j++)
        {
            printf("%d  ", aux.indice[j]);
        }
    }
    fclose(arvB);
    return(0);
}
