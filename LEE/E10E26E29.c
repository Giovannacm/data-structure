/*
#include <stdio.h>                                                                  //EXERCICIO 29
#include <stdlib.h>
struct no
{
    int info;
    struct no *next;
};
struct lista
{
    struct no *ini, *fim;
};
short InsereNoOrdenado(int valor, struct lista *L)
{
    struct no *p=NULL, *q=NULL;
    p=(struct no *)malloc(sizeof(struct no));
    if(p==NULL)
        return(0);
    p->next=NULL;
    p->info=valor;
    if(L->ini==NULL)
        L->ini=L->fim=p;
    else
    {
        if(valor <= L->ini->info)
        {
            p->next=L->ini;
            L->ini=p;
        }
        else if(valor >= L->fim->info)
        {
            L->fim->next=p;
            L->fim=p;
        }
        else
        {
            for(q=L->ini ; (q!=NULL) ; q=q->next)
                if(valor <= q->next->info)
                {
                    p->next=q->next;
                    q->next=p;
                    break;
                }
        }
    }
    return(1);
}
short RemoveElemento(int valor, struct lista *L)
{
    struct no *p=NULL, *ant=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info!=valor) ; p=p->next)
        ant=p;
    if(p==NULL)
        return(0);  //Não achou o elemento - fim de varredura
    if(ant==NULL)           //Retira o primeiro elemento
    {
        L->ini=p->next;
        free(p);
    }
    else if(p==L->fim)      //Retira o ultimo elemento
    {
        L->fim=ant;
        L->fim->next=NULL;
        free(p);
    }
    else
    {
        ant->next=p->next;
        free(p);
    }
    return(1);
}
short BuscaNo(int valor, struct lista *L)
{
    struct no *p=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info<=valor) ; p=p->next) //Se a lista estiver ordenada, o código fica mais eficiente ao colocar p->info<=valor
    {
        if(p->info==valor)
            return(1);
    }
    return(0);
}
void ImprimeLista(struct lista *L, int t)
{
    struct no *p=NULL;
    int i=0;
    for(i=0 ; i<t ; i++)
    {
        printf("\n      Lista[%d]: ", i+1);
        for(p=L[i].ini ; p!=NULL ; p=p->next)
            printf("%d ", p->info);
    }
    printf("\n\n");
}
void CriaListaOrdenada(struct lista *L, int TAMANHO)
{
    int i=0, valor=0;
    for(i=0 ; i<TAMANHO; i++)
    {
        printf("\n  %d = ", i+1);
        scanf("%d", &valor);
        InsereNoOrdenado(valor, L);
    }
}
void InicializaLista(struct lista *L)
{
    L->ini=NULL;
    L->fim=NULL;
}

int main (void)
{
    int qtd, valor, i, j;
    struct no *p=NULL;

    printf("\n      QUANTIDADE DE LISTA:");
    scanf("%d", &qtd);
    struct lista VetList[qtd];
    for(i=0 ; i<qtd ; i++)
        InicializaLista(&VetList[i]);

    for(j=0 ; j<qtd ; j++)
    {
        printf("\n      Cria Lista:");
        scanf("%d %d", &i, &valor);
        CriaListaOrdenada(&VetList[i], valor);
    }
    printf("\n      Insercao: ");
    scanf("%d %d", &i, &valor);
    InsereNoOrdenado(valor, &VetList[i]);
    printf("\n      Remocao: ");
    scanf("%d %d", &i, &valor);
    RemoveElemento(valor, &VetList[i]);
    printf("\n      Busca: ");
    scanf("%d %d", &i, &valor);
    BuscaNo(valor, &VetList[i]);
    printf("\n      Impressao: ");
    ImprimeLista(VetList, qtd);

}
*/
#include <stdio.h>
#include <stdlib.h>
struct no                                                                           //EXERCICIO 10
{
    int info;
    struct no *next;
};
struct lista
{
    struct no *ini, *fim;
    int contIns, contRem;
};
short InsereNoOrdenado(int valor, struct lista *L)
{
    struct no *p=NULL, *q=NULL;
    p=(struct no *)malloc(sizeof(struct no));
    if(p==NULL)
        return(0);
    p->next=NULL;
    p->info=valor;
    if(L->ini==NULL)
        L->ini=L->fim=p;
    else
    {
        if(valor <= L->ini->info)
        {
            p->next=L->ini;
            L->ini=p;
        }
        else if(valor >= L->fim->info)
        {
            L->fim->next=p;
            L->fim=p;
        }
        else
        {
            for(q=L->ini ; (q!=NULL) ; q=q->next)
                if(valor <= q->next->info)
                {
                    p->next=q->next;
                    q->next=p;
                    break;
                }
        }
    }
    L->contIns++;
    return(1);
}
short RemoveElemento(int valor, struct lista *L)
{
    struct no *p=NULL, *ant=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info!=valor) ; p=p->next)
        ant=p;
    if(p==NULL)
        return(0);  //Não achou o elemento - fim de varredura
    if(ant==NULL)           //Retira o primeiro elemento
    {
        L->ini=p->next;
        free(p);
    }
    else if(p==L->fim)      //Retira o ultimo elemento
    {
        L->fim=ant;
        L->fim->next=NULL;
        free(p);
    }
    else
    {
        ant->next=p->next;
        free(p);
    }
    L->contRem++;
    return(1);
}
void CriaListaOrdenada(struct lista *L, int TAMANHO)
{
    int i=0, valor=0;
    for(i=0 ; i<TAMANHO; i++)
    {
        printf("\n  %d = ", i+1);
        scanf("%d", &valor);
        InsereNoOrdenado(valor, L);
    }
    L->contIns=0;
    L->contRem=0;
}
void InicializaLista(struct lista *L)
{
    L->ini=NULL;
    L->fim=NULL;
    L->contIns=0;
    L->contRem=0;
}
int main (void)
{
    struct lista L;
    InicializaLista(&L);
    CriaListaOrdenada(&L, 6);

    InsereNoOrdenado(100, &L);
    InsereNoOrdenado(200, &L);
    RemoveElemento(3, &L);
    RemoveElemento(5, &L);

    printf("\n      Houve %d insercoes", L.contIns);
    printf("\n      Houve %d remocoes", L.contRem);

}

/*
#include <stdio.h>                                                                  //EXERCICIO 26
#include <stdlib.h>
#include <string.h>
void ImprimeNomesOrdenado(char **v, int t)
{
    char Aux[20];
    int i=0, j=0;
    for(i=0 ; i<t ; i++)                //Ordenando
    {
        for(j=0 ; j<t-1 ; j++)
        {
            if(strcmp(*(v+j), *(v+j+1))>0)
            {
                strcpy(Aux, *(v+j));
                strcpy(*(v+j), *(v+j+1));
                strcpy(*(v+j+1), Aux);
                printf("\n aux %s", Aux);
            }
        }
    }
    for(i=0 ; i<t ; i++)                //Imprimindo
        printf("\n  %s", *(v+i));
}
int main (void)
{
    int qtd=0, i=0;
    char **p;

    printf("\n      QUANTIDADE DE NOMES:");
    scanf("%d", &qtd);

    p=(char **)malloc(qtd*sizeof(char *));

    for(i=0 ; i<qtd ; i++)
    {
        *(p+i)=(char *)malloc(sizeof(char));
        printf("\n      NOME %d: ", i+1);
        scanf(" %s", *(p+i));
    }
    printf("\n      IMPRESSAO DOS NOMES: ");
    ImprimeNomesOrdenado(p, qtd);
}
*/
