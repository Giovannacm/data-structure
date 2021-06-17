#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define MAX2 5
#define MAX3 20 //Tamanho da String Nome;

int Disp;
struct Aluno
{
    int RA;
    char Nome[20];
};
struct No
{
    int next;
    struct Aluno info;
};
void InicializaTabela(FILE *tab)
{
    int i, LIM;
    LIM=MAX+MAX2;
    struct No NoMolde;
    for(i=0 ; i<MAX ; i++)
    {
        NoMolde.info.RA = -1;
        NoMolde.next = -1;
        strcpy(NoMolde.info.Nome, "NULL");
        fwrite(&NoMolde, sizeof(struct No), 1, tab);
    }
    for(i=MAX ; i<LIM-1 ; i++)
    {
        NoMolde.next=i+1;
        strcpy(NoMolde.info.Nome, "NULL");
        fwrite(&NoMolde, sizeof(struct No), 1, tab);
    }
    NoMolde.next=-1;
    fwrite(&NoMolde, sizeof(struct No), 1, tab);
    Disp=MAX;
}
int hash(char *chave)
{
    int i, soma=0, lim;
    lim = strlen(chave);
    for( i=0 ; i<lim ; i++)
    {
        soma = soma + (i+1)*((int)chave[i]);
    }
    return (soma%MAX);
    //MAX SER PRIMO
}
short InsereDadosTabela(FILE *tab, int N)     //Colocando os N dados na tabela (que é um arquivo binário tab)
{
    int i, pos, aux, ant;
    struct Aluno A;
    struct No NoMolde, NoMoldeOver, NoMoldeDisp;
    for(i=0 ; i<N ; i++)
    {
        printf("\n      Nome: ");
        scanf("%s", &A.Nome);
        printf("\n      Matricula: ");
        scanf("%d", &A.RA);
        pos = hash(A.Nome);
        fseek(tab, pos*(sizeof(struct No)),SEEK_SET);
        fread(&NoMolde, sizeof(struct No), 1, tab);
        if(Disp==-1)
        {
            printf("\n      TABELA CHEIA!!");
            return(0);
        }
        if(NoMolde.next==-1 && NoMolde.info.RA==-1) //Não tem ninguém naquela posição do arquivo
        {
            strcpy(NoMolde.info.Nome, A.Nome);
            NoMolde.info.RA = A.RA;
            fseek(tab, pos*(sizeof(struct No)),SEEK_SET);
            fwrite(&NoMolde, sizeof(struct No), 1, tab);
        }
        else    //Insere na area de overflow
        {
            if(NoMolde.next==-1)    //Não tem nenhum elemento na area de overflow
            {
                NoMolde.next=Disp;
                fseek(tab, pos*(sizeof(struct No)),SEEK_SET);       //Atualizando o campo next para a posição disponível da area de overflow
                fwrite(&NoMolde, sizeof(struct No), 1, tab);
            }
            else                    //Já tem elemento na area de overflow
            {
                ant=NoMolde.next;
                fseek(tab, NoMolde.next*(sizeof(struct No)),SEEK_SET);
                fread(&NoMoldeOver, sizeof(struct No), 1, tab);
                while(NoMoldeOver.next!=-1)
                {
                    ant=NoMoldeOver.next;
                    fseek(tab, NoMoldeOver.next*(sizeof(struct No)),SEEK_SET);
                    fread(&NoMoldeOver, sizeof(struct No), 1, tab);
                }
                NoMoldeOver.next=Disp;
                fseek(tab, ant*(sizeof(struct No)),SEEK_SET);
                fwrite(&NoMoldeOver, sizeof(struct No), 1, tab);
            }
            fseek(tab, Disp*(sizeof(struct No)),SEEK_SET);
            fread(&NoMoldeDisp, sizeof(struct No), 1, tab);
            aux=NoMoldeDisp.next;
            strcpy(NoMoldeDisp.info.Nome, A.Nome);
            NoMoldeDisp.info.RA = A.RA;
            NoMoldeDisp.next = -1;
            fseek(tab, Disp*(sizeof(struct No)),SEEK_SET);
            fwrite(&NoMoldeDisp, sizeof(struct No), 1, tab);
            Disp=aux;
            //Poderia usar o ftell -> area de overflow ilimitada (não inicializa a área de overflow)
            //Usar seek_end
        }
    }
    return(1);
}
void ImpressaoTabelaHash(FILE *t)
{
    int i=0;
    struct No NoMolde;
    fseek(t, i*(sizeof(struct No)), SEEK_SET);
    fread(&NoMolde, sizeof(struct No), 1, t);
    while(!feof(t))
    {
        printf("\n     %2d      Nome: %s   Matricula: %d   Next:%d", i, NoMolde.info.Nome, NoMolde.info.RA, NoMolde.next);
        i++;
        fseek(t, i*(sizeof(struct No)), SEEK_SET);
        fread(&NoMolde, sizeof(struct No), 1, t);
    }
    printf("\n\n     [Disp]: %d\n", Disp);
}

int main(void)
{
    FILE *T1;
    int Quant;
    char Arq1[MAX];
    //printf("\n      Nome do arquivo a ser criado: ");
    //gets(Arq1);
    if((T1 = fopen( "dados2.bin", "w+b"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    fseek(T1, 0, SEEK_SET);
    InicializaTabela(T1);
    printf("\n      Quantidade de pessoas a serem cadastradas: ");
    scanf("%d", &Quant);
    fseek(T1, 0, SEEK_SET);
    InsereDadosTabela(T1, Quant);
    fseek(T1, 0, SEEK_SET);
    ImpressaoTabelaHash(T1);
    fclose(T1);
}
