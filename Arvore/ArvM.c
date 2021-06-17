#include <stdio.h>
#include <stdlib.h>

struct no
{
    char info;
    struct no *filho, *irmao;
};

void percorreArvM(struct no arv)
{
    int i;
    printf("");
    //tem direita

        //
    //tem esquerda
}
void inserirDados(struct no arv, char info, struct no *filho, struct no *irmao)
{
    struct no molde;
    molde.info = info;
    molde.filho = filho;
    molde.irmao = irmao;
    arv = molde;
}

int main(void)
{
    struct no arv[16];
    inserirDados(arv[0], 'A', NULL, NULL);
    inserirDados(arv[1], 'B', NULL, NULL);
    inserirDados(arv[2], 'C', NULL, NULL);
    inserirDados(arv[3], 'D', NULL, NULL);
    inserirDados(arv[4], 'E', NULL, NULL);
    inserirDados(arv[5], 'F', NULL, NULL);
    inserirDados(arv[6], 'G', NULL, NULL);
    inserirDados(arv[7], 'H', NULL, NULL);
    inserirDados(arv[8], 'I', NULL, NULL);
    inserirDados(arv[9], 'J', NULL, NULL);
    inserirDados(arv[10], 'K', NULL, NULL);
    inserirDados(arv[11], 'L', NULL, NULL);
    inserirDados(arv[12], 'M', NULL, NULL);
    inserirDados(arv[13], 'N', NULL, NULL);
    inserirDados(arv[14], 'O', NULL, NULL);
    inserirDados(arv[15], 'P', NULL, NULL);
    arv[0].filho = &arv[1];
    arv[1].filho = &arv[5];
    arv[1].irmao = &arv[2];
    arv[2].filho = &arv[8];
    arv[2].irmao = &arv[3];
    arv[3].irmao = &arv[4];
    arv[4].filho = &arv[9];
    arv[5].irmao = &arv[6];
    arv[6].filho = &arv[12];
    arv[6].irmao = &arv[7];
    arv[9].irmao = &arv[10];
    arv[10].irmao = &arv[11];
    arv[12].filho = &arv[15];
    arv[12].irmao = &arv[13];
    arv[13].irmao = &arv[14];
    percorreArvM(arv);
    return(0);
}
