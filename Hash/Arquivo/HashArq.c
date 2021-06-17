#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

struct reg
{
    char Nome[MAX];
    int Pos;
    int next;
};
struct lista
{
    int disp;
    int prim;
    struct reg A[MAX];
};
void InicializaLista(struct lista *L)
{
    int i=0;
    L->disp=0;
    L->prim=-1;
    for(i=0 ; i<MAX-1 ; i++)
        L->A[i].next=i+1;
    L->A[i].next= -1;
}

struct Pessoa
{
    int Pos;        //Posição no arquivo
    char Nome[MAX], Sexo, Cargo;
    int Matricula, Salario, Setor;
};
void InsereDadosArquivo(FILE *f, int N)     //Colocando os dados no arquivo
{
    int i;
    struct Pessoa P;
    for(i=0 ; i<N ; i++)
    {
        P.Pos=i;
        printf("\n      Matricula: ");
        scanf("%d", &P.Matricula);
        printf("\n      Nome: ");
        scanf("%s", &P.Nome);
        printf("\n      Sexo: ");
        scanf("%c", &P.Sexo);
        printf("\n      Salario: ");
        scanf("%d", &P.Salario);
        printf("\n      Cargo: ");
        scanf("%c", &P.Cargo);
        printf("\n      Setor: ");
        scanf("%d", &P.Setor);
        fwrite(&P, sizeof(struct Pessoa), 1, f);
    }
    return;
}

//                      VERSÃO 1 PARA CRIAR A LISTA - (Insere a pessoa na posicao disponivel)
void InsereElementoOrdenado(struct lista *L, int Pos, char *Nome)       //Insercao ordenada de acordo com o nome da pessoa
{
    int PROXIMO=0, ANTERIOR=0, ATUAL=0, aux=0;
    if(L->disp != -1)       //Lista não está cheia
    {
        L->A[L->disp].Pos = Pos;
        strcpy(L->A[L->disp].Nome, Nome);
        PROXIMO = L->A[L->disp].next;
        if(L->prim == -1)       //Lista vazia
        {
            L->prim=L->disp;
            L->A[L->disp].next = -1;
        }
        else if(strcmp(Nome, L->A[L->prim].Nome)<0)      //Tem elementos //Inserção no início da lista
        {
            L->A[L->disp].next = L->prim;
            L->prim = L->disp;
        }
        else     //Não será inserido no inicio, sendo assim devemos procurar em qual posição ele ficará
        {
            ANTERIOR=L->prim;
            for(ATUAL=L->A[L->prim].next ; (ATUAL!=-1) && (strcmp(Nome, L->A[ATUAL].Nome)>0) ; ATUAL = L->A[ATUAL].next)
                ANTERIOR=ATUAL;
            L->A[ANTERIOR].next = L->disp;
            L->A[L->disp].next = ATUAL; //antes do de cima e só se if(ATUAL!=-1)
        }
        L->disp = PROXIMO;
    }
    else        //Lista está cheia
    {
        aux= L->prim;
        if(strcmp(Nome, L->A[aux].Nome)<0)
        {
            L->A[aux].Pos = Pos;
            strcpy(L->A[aux].Nome, Nome);
        }
        else
            while(strcmp(Nome, L->A[aux].Nome)>0)
            {
                aux=L->A[aux].next;
            }
        strcpy(L->A[aux].Nome, Nome);
    }
    return;
}
void CriaLista(FILE *f, struct lista *l)    //Lendo o arquivo e inserindo na tabela hash
{
    struct Pessoa P;
    fread(&P, sizeof(struct Pessoa), 1, f);
    while(!feof(f))
    {
        InsereElementoOrdenado(l, P.Pos, P.Nome);
        fread(&P, sizeof(struct Pessoa), 1, f);
    }
}
void ImprimeLista(struct lista L)
{
    int i=0;
    printf("\n Indice     Nome     Next     Pos");
    for(i=0 ; i<L.disp ; i++)
    {
        printf("\n %4d  -  %s   %4d   %4d", i, L.A[i].Nome, L.A[i].next, L.A[i].Pos);
    }
    printf("\n\n [Prim]=%d        [Disp]=%d\n", L.prim, L.disp);
    return;
}

//                      VERSÃO 2 PARA CRIAR A LISTA - (Insere a pessoa na posicao retornada pela funcao hash)
void InsereElementoHash(struct lista *L, int Pos, char *Nome, int H)    //Insere a pessoa na posicao H e atualiza os "nexts" / Não faz uso da posicao disponivel
{
    int PROXIMO=0, ANTERIOR=0, ATUAL=0, aux=0;
    L->A[H].Pos=Pos;
    strcpy(L->A[H].Nome, Nome);
    if(L->prim == -1)       //Lista vazia
    {
        L->prim=H;
        L->A[H].next = -1;
    }
    else
    {
        for(ATUAL=L->prim ; (ATUAL!=-1) && (strcmp(Nome, L->A[ATUAL].Nome)>0) ; ATUAL = L->A[ATUAL].next)
            ANTERIOR=ATUAL;
        if(ANTERIOR==0)     //Vai se tornar o primeiro elemento
        {
            L->A[H].next=L->prim;
            L->prim=H;
        }
        else if(ATUAL==-1)  //Vai se tornar o ultimo elemento
        {
            L->A[ANTERIOR].next=H;
            L->A[H].next=-1;
        }
        else                //Elemento no meio
        {
            L->A[ANTERIOR].next=H;
            L->A[H].next=ATUAL;
        }
    }
    return;
}
int hash(char *chave, int TAM)      //Hash que retorna um valor inteiro para uma chave do tipo String - representando o nome da pessoa
{
    int i, soma=0, lim;
    lim = strlen(chave);
    for ( i=0 ; i<lim ; i++)
    {
        soma = soma + (i+1)*((int)chave[i]);
    }
    return (soma%TAM);
}
void CriaListaHash(FILE *f, struct lista *l, int TAM)
{
    int H;
    int c;
    struct Pessoa P;
    fread(&P, sizeof(struct Pessoa), 1, f);
    while(!feof(f))
    {
        H = hash(P.Nome, TAM);
        InsereElementoHash(l, P.Pos, P.Nome, H);
        fread(&P, sizeof(struct Pessoa), 1, f);
    }
}
void ImprimeLista2(struct lista L, int TAM)
{
    int i=0;
    printf("\n Indice     Nome     Next     Pos");
    for(i=0 ; i<TAM ; i++)
    {
        printf("\n %4d  -  %s   %4d   %4d", i, L.A[i].Nome, L.A[i].next, L.A[i].Pos);
    }
    printf("\n\n [Prim]=%d        [Disp]=%d\n", L.prim, L.disp);
    return;
}

int main(void)
{
    FILE *f1;
    int Quant;
    char Arq1[MAX];
    struct lista l1;
    InicializaLista(&l1);
    printf("\n      Nome do arquivo a ser criado: ");
    gets(Arq1);
    if((f1 = fopen( Arq1, "wt+"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    printf("\n      Quantidade de pessoas a serem cadastradas: ");
    scanf("%d", &Quant);
    InsereDadosArquivo(f1, Quant);
    fseek(f1, 0, SEEK_SET);

    CriaLista(f1, &l1);
    ImprimeLista(l1);

    //CriaListaHash(f1, &l1, Quant);
    //ImprimeLista2(l1, Quant);

    fclose(f1);
}
/*void ImpressaoArquivo(FILE *f)
{
    struct Pessoa P;
    fread(&P, sizeof(struct Pessoa), 1, f);
    while(!feof(f))
    {
        fprintf(stdout, "\n      Matricula: %d", P.Matricula);
        fprintf(stdout, "\n      Nome: %s", P.Nome);
        fprintf(stdout, "\n      Sexo: %c", P.Sexo);
        fprintf(stdout, "\n      Salario: %d", P.Salario);
        fprintf(stdout, "\n      Cargo: %c", P.Cargo);
        fprintf(stdout, "\n      Setor: %d", P.Setor);
        fprintf(stdout, "\n");
        fread(&P, sizeof(struct Pessoa), 1, f);
    }
    return;
}*/
/*
        //Fiz esse switch pra deixar o valor de retorno da funcao hash igual o exemplo
        c=(int)(P.Nome[0]);
        switch(c)
        {
            case 65:
                H=2;
                break;
            case 66:
                H=4;
                break;
            case 70:
                H=1;
                break;
            case 71:
                H=3;
                break;
            case 80:
                H=5;
                break;
            case 90:
                H=0;
        }*/
