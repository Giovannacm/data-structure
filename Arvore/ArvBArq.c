/*Árvore B (B-tree) de ordem m é uma árvore com as seguintes propriedades:
- Cada nó contém no máximo (m−1) chaves;
- A raiz contém no mínimo 2 chaves e cada um dos demais nós contém no mínimo m/2 chaves;
- Cada nó que não seja uma folha tem um filho para cada uma de suas chaves;
- Todos os caminhos da raiz até uma folha têm o mesmo comprimento (ou seja, a árvore é perfeitamente balanceada);
- Aplicações: árvores B são a estrutura subjacente a muitos sistemas de arquivos e bancos de dados;
São árvores de pesquisa balanceadas projetadas para funcionar bem em discos magnéticos ou outros dispositivos de armazenamento secundário.
– voltado para arquivos volumosos
– proporciona rápido acesso	aos	dados;
O número de acessos ao disco exigidos para a maioria das operações em uma árvore B é proporcional a sua	altura

Propriedades:
1 - Todo nó x tem os seguintes campos:
    -   n[x] = o número de chaves armazenadas no nó x
    -   as próprias n[x] chaves, armazenadas em ordem crescente, de modo que chave1[x]<=chave2[x]<=...<=chaven[x]
    -   folha[x], um valor booleano que é true se x é folha, e false se x é um nó interno
2 - Cada nó interno x também contém n[x]+1 ponteiros para seus filhos.
3 - Seja t (t>=2) o grau mínimo da árvore B, então:
    -   todo nó diferente da raiz deve ter pelo menos t-1 chaves, sendo assim possui pelo menos t filhos. Se a árvore é não vazia, a raiz deve ter pelo menos uma chave
    -   todo nó pode ter no máximo 2t-1 chaves, sendo assim um nó interno pode ter no máximo 2t filhos.
Grau mínimo t
    n = 2t – 1 chaves
    n + 1 = 2t filhos
Ordem m
    n = m – 1 chaves
    n + 1 = m filhos
O número de acesso ao disco exigido pela maioria das operações em uma árvore B é proporcional à altura da árvore.
No pior caso, sua altura pode ser definida como a<=log de ((n+1)/2) na base t, onde n é o número de nós e t, o grau minimo.
*/
#include <stdio.h>
#include <stdlib.h>
#define n 3
#define t 2

int raiz;
struct noB
{
    int nroChavesArm, indice[n+1];
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
short buscaArvB(FILE *arv, struct noB no, char elem)
{
    int i;
    struct noB molde;
    for(i=0 ; i<no.nroChavesArm && elem>no.chave[i] ; i++);
    printf("\n  [%c] ", no.chave[i]);
    if(i<no.nroChavesArm && elem==no.chave[i])
        return(1);
    if(no.folha)
        return(0);
    else
    {
        fseek(arv, no.indice[i]*sizeof(struct noB), SEEK_SET);
        fread(&molde, sizeof(struct noB), 1, arv);
        return(buscaArvB(arv, molde, elem));
    }
}

int main(void)
{
    FILE *arvB;
    struct noB aux;
    int i, j;
    if((arvB = fopen("ArvB.bin", "wb+"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    //----Gravando em um arquivo binário
    inserirNoArquivo(arvB, 0, 2, 0, 'K', 'Q', ' ', 1, 2, 3, -1);
    inserirNoArquivo(arvB, 1, 2, 0, 'B', 'F', ' ', 4, 5, 6, -1);
    inserirNoArquivo(arvB, 2, 1, 0, 'M', ' ', ' ', 7, 8, -1, -1);
    inserirNoArquivo(arvB, 3, 2, 0, 'T', 'W', ' ', 9, 10, 11, -1);
    inserirNoArquivo(arvB, 4, 1, 1, 'A', ' ', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 5, 3, 1, 'C', 'D', 'E', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 6, 1, 1, 'H', ' ', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 7, 1, 1, 'L', ' ', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 8, 2, 1, 'N', 'P', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 9, 2, 1, 'R', 'S', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 10, 1, 1, 'V', ' ', ' ', -1, -1, -1, -1);
    inserirNoArquivo(arvB, 11, 3, 1, 'X', 'Y', 'Z', -1, -1, -1, -1);

    //----Colocando nó raiz na memória----
    fseek(arvB, 0*sizeof(struct noB), SEEK_SET);
    fread(&aux, sizeof(struct noB), 1, arvB);

    //----Fazendo a busca
    if(buscaArvB(arvB, aux, 'D'))
        printf("\n  ENCONTREI!");
    else
        printf("\n  NAO ENCONTREI!");

    //----Impressão----
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
/*
void criaArvBVazia(FILE *arv, int indice)
{
    struct noB molde;
    molde.folha = 1;
    molde.nroChavesArm = 0;
    fseek(arv, indice*sizeof(struct noB), SEEK_SET);
    fwrite(&molde, sizeof(struct noB), 1, arv);
    raiz = indice;
}
void inserirArvB()
{

}
void split()
{

}
void inserirNaoCheioArvB(FILE *arv, struct noB no, char elem)
{
    int i;
    struct noB aux;
    if(no.folha)
    {
        for(i=no.nroChavesArm ; i>=1 && elem<no.chave[i] ; i--)
            no.chave[i+1] = no.chave[i];
        no.chave[i+1] = elem;
        no.nroChavesArm++;
        //disk-write(no)
    }
    else
    {
        for(i=no.nroChavesArm ; i>=1 && elem<no.chave[i] ; i--);
        i++;
        fseek(arv, no.indice[i]*sizeof(struct noB), SEEK_SET);
        fread(&aux, sizeof(struct noB), 1, arv);
        if(aux.nroChavesArm==2*t-1)
        {

            if(elem>no.chave[i])
                i++;
        }
        inserirNaoCheioArvB(arv, aux, elem);
    }
}
*/
