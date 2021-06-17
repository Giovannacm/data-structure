#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define MAX3 20 //Tamanho da String Nome;

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
    int i;
    struct No NoMolde;
    NoMolde.info.RA = -1;
    NoMolde.next = -1;
    strcpy(NoMolde.info.Nome, "NULL");
    for(i=0 ; i<MAX ; i++)
        fwrite(&NoMolde, sizeof(struct No), 1, tab);
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
short InsereDadosTabela(FILE *tab, FILE *dados)       //Colocando os N dados na tabela (que é um arquivo binário tab)
{
    int i, pos, aux, ant, disp;
    struct Aluno A;
    struct No NoMolde, NoMoldeDisp;
    for(i=0 ; (!feof(dados)) ; i++)
    {
        fseek(dados, i*sizeof(struct Aluno), SEEK_SET);
        fread(&A, sizeof(struct Aluno), 1, dados);
        pos = hash(A.Nome);
        fseek(tab, pos*(sizeof(struct No)),SEEK_SET);
        fread(&NoMolde, sizeof(struct No), 1, tab);
        if(NoMolde.info.RA==-1)                 //Não tem ninguém naquela posição do arquivo
        {
            strcpy(NoMolde.info.Nome, A.Nome);
            NoMolde.info.RA = A.RA;
            fseek(tab, pos*(sizeof(struct No)),SEEK_SET);
            fwrite(&NoMolde, sizeof(struct No), 1, tab);
        }
        else                                    //Insere na area de overflow (que na teoria, ela é ilimitada)
        {
            strcpy(NoMoldeDisp.info.Nome, A.Nome);
            NoMoldeDisp.info.RA = A.RA;
            NoMoldeDisp.next = -1;
            fseek(tab, 0, SEEK_END);            //Reposicionando a variável ponteiro de arquivo na ultima posição do arquivo (a posição disponível)
            disp=ftell(tab)/sizeof(struct No);  //Disp está armazenando a quantidade que elementos que a tabela está tendo
            fwrite(&NoMoldeDisp, sizeof(struct No), 1, tab);
            if(NoMolde.next==-1)                //Não tem nenhum elemento na area de overflow
            {
                NoMolde.next=disp;
                fseek(tab, pos*(sizeof(struct No)),SEEK_SET);       //Atualizando o campo next para a posição disponível da area de overflow
                fwrite(&NoMolde, sizeof(struct No), 1, tab);
            }
            else                                //Já tem elemento na area de overflow
            {
                ant=NoMolde.next;
                fseek(tab, NoMolde.next*(sizeof(struct No)),SEEK_SET);
                fread(&NoMolde, sizeof(struct No), 1, tab);
                while(NoMolde.next!=-1)
                {
                    ant=NoMolde.next;
                    fseek(tab, NoMolde.next*(sizeof(struct No)),SEEK_SET);
                    fread(&NoMolde, sizeof(struct No), 1, tab);
                }
                NoMolde.next=disp;
                fseek(tab, ant*(sizeof(struct No)),SEEK_SET);
                fwrite(&NoMolde, sizeof(struct No), 1, tab);
            }
        }
    }
    return(1);
}
void InsereDadosArquivo(FILE *dados, int N)
{
    int i;
    struct Aluno A;
    for(i=0 ; i<N ; i++)
    {
        printf("\n      Nome: ");
        scanf("%s", &A.Nome);
        printf("\n      Matricula: ");
        scanf("%d", &A.RA);
        fseek(dados, 0, SEEK_END);
        fwrite(&A, sizeof(struct Aluno), 1, dados);
    }
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
}
void ImpressaoArqDados(FILE *d)
{
    int i=0;
    struct Aluno A;
    fseek(d, i*(sizeof(struct Aluno)), SEEK_SET);
    fread(&A, sizeof(struct Aluno), 1, d);
    while(!feof(d))
    {
        printf("\n      Nome: %s   Matricula: %d", A.Nome, A.RA);
        i++;
        fseek(d, i*(sizeof(struct Aluno)), SEEK_SET);
        fread(&A, sizeof(struct Aluno), 1, d);
    }
}

int main(void)
{
    FILE *T1, *D1;
    int Quant;
    char Arq1[MAX];
    //printf("\n      Nome do arquivo a ser criado: ");
    //gets(Arq1);
    if((T1 = fopen( "tab4.bin", "w+b"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    if((D1 = fopen( "dados.bin", "w+b"))==NULL)
    {
        printf("\n      ERRO DE ABERTURA!");
        exit(1);
    }
    InicializaTabela(T1);
    printf("\n      Quantidade de pessoas a serem cadastradas: ");
    scanf("%d", &Quant);
    fseek(D1, 0, SEEK_SET);
    InsereDadosArquivo(D1, Quant);
    fseek(T1, 0, SEEK_SET);
    InsereDadosTabela(T1, D1);
    fseek(T1, 0, SEEK_SET);
    ImpressaoTabelaHash(T1);
    fseek(D1, 0, SEEK_SET);
    ImpressaoArqDados(D1);
    fclose(T1);
    fclose(D1);
}

