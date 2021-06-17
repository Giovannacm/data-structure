#include <stdio.h>
#include <stdlib.h>

struct No
{
    int info;
    struct No *next;
};
struct Pilha
{
    struct No *topo;
};
void InicializaPilha(struct Pilha *P)
{
    P->topo=NULL;
}
short Insere(struct Pilha *P, int C)
{
    struct No *a=NULL;
    a=(struct No *)malloc(sizeof(struct No));
    if(a==NULL)
        return(0);
    a->info=C;
    a->next=P->topo;
    P->topo=a;
    return(1);
}
short Remove(struct Pilha *P)
{
    struct No *a=NULL;
    if(P->topo==NULL)       //Pilha vazia
        return(0);
    a=P->topo;
    P->topo=a->next;
    free(a);
    return(1);
}
void Imprime(struct Pilha P)
{
    struct No *a=NULL;
    printf("\n Topo -> ");
    for(a=P.topo ; a!=NULL ; a=a->next)
        printf("%d  ", a->info);
}
void Calculadora(struct Pilha *P, char *S)
{
    int i=0, Vint=0, N1=0, N2=0;
    for(i=0 ; *(S+i)!='\0' ; i++)
    {
        if((int)*(S+i)<=57 && (int)*(S+i)>=48)
        {
            Vint=(int)*(S+i)-48;
            Insere(P, Vint);
        }
        else
        {
            N2=P->topo->info;
            Remove(P);
            N1=P->topo->info;
            Remove(P);
            switch(*(S+i))
            {
            case '+':
                Insere(P, N1+N2);
                break;
            case '-':
                Insere(P, N1-N2);
                break;
            case '*':
                Insere(P, N1*N2);
                break;
            case '/':
                Insere(P, N1/N2);
                break;
            }
        }
    }
}
int main (void)
{
    struct Pilha Calc;
    char Exp[10];
    InicializaPilha(&Calc);

    printf("\n      Digite a expressao: ");
    scanf("%s", Exp);
    Calculadora(&Calc, Exp);
    printf("\n      [RESULTADO]: %d\n", Calc.topo->info);

    return(0);
}
