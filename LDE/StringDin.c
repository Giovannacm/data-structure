#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct NoS
{
    char info;
    struct NoS *next;
};
struct StrDim
{
    struct NoS *ini, *fim;
    //int Tam;      //Para armazenar o tamanho da lista
};

void CriaStringVazia(struct StrDim *Str)
{
    Str->fim = Str->ini = NULL;
}
void ReiniciaString(struct StrDim *Str)
{
    struct NoS *c=NULL;
    while(Str->ini!=NULL)
    {
        c=Str->ini;
        Str->ini=c->next;
        //Str->ini->Last = NULL;                    //Se fosse duplamente encadeada
        free(c);
    }
}
short StringVazia(struct StrDim Str)
{
    if(Str.ini==NULL)
        return(1);
    return(0);
}
void ImprimeString(struct StrDim Str)
{
    struct NoS *c=NULL;
    if(StringVazia(Str))
        printf("\n      String vazia!");
    else
    {
        for(c=Str.ini ; c!=NULL ; c=c->next)
            printf("%c ", c->info);
    }
}
int TamanhoString(struct StrDim *Str)
{
    int t=0;
    struct NoS *c=NULL;
    for(c=Str->ini ; c!=NULL ; c=c->next)
        t++;
    return(t);
}
short InsereCaracOrd(struct StrDim *Str, char Ch)
{
    struct NoS *p=NULL, *c=NULL;
    c=(struct NoS *)malloc(sizeof(struct NoS));
    if(c==NULL)
        return(0);
    c->info=Ch;
    c->next=NULL;
    if(Str->ini==NULL)
        Str->ini=Str->fim=c;
    else
    {
        if(Ch < Str->ini->info)        //ch<valor do inicio, então insere no inicio
        {
            c->next=Str->ini;
            Str->ini=c;
        }
        else if(Ch > Str->fim->info)   //ch>valor do fim, então insere no fim
        {
            Str->fim->next=c;
            Str->fim=c;
        }
        else                                   //ch será inserido no meio, logo vamo achar a posição
        {
            for(p=Str->ini ; p!=NULL ; p=p->next)
            {
                if((Ch <= p->next->info))//ch menor ou igual o valor no proximo nó
                {
                    c->next=p->next;
                    p->next=c;
                    break;
                }
            }
        }
    }
    return(1);
}
short InsereCaracFim(struct StrDim *Str, char Ch)
{
    struct NoS *c=NULL;
    c=(struct NoS *)malloc(sizeof(struct NoS));
    if(c==NULL)
        return(0);
    c->info=Ch;
    c->next=NULL;
    if(Str->ini==NULL)
        Str->ini=c;
    else
        Str->fim->next=c;
    Str->fim=c;
    return(1);
}
short InsereCaracIni(struct StrDim *Str, char Ch)
{
    struct NoS *c=NULL;
    c=(struct NoS *)malloc(sizeof(struct NoS));
    if(c==NULL)
        return(0);
    c->info=Ch;
    c->next=NULL;
    if(Str->ini==NULL)
        Str->fim=c;
    else
        c->next=Str->ini;
    Str->ini=c;
    return(1);
}
void CopiaString(struct StrDim *S1, struct StrDim *S2)
{
    struct NoS *c=NULL;
    for(c=S1->ini ; c!=NULL ; c=c->next)
        InsereCaracFim(S2, c->info);
}
void ConcatenaString(struct StrDim *S1, struct StrDim *S2, struct StrDim *S3)
{
    CopiaString(S1, S3);    //S1 e S2 em S3
    CopiaString(S2, S3);
}
void ConcatenaString2(struct StrDim *S1, struct StrDim *S2)
{
    S1->fim->next=S2->ini;
    S1->fim=S2->fim;
}
short RemoveStrPos(struct StrDim *Str, int qtd, int start)
{
    int cont=0;
    struct NoS *p=NULL, *q=NULL;
    for(p=Str->ini ; (p!=NULL)&&(cont!=start) ; p=p->next)  //Já sai com p na posição certa e ant na posicão anterior
        cont++;
    if(p==NULL) //Não encontrou a posicao (start>tamanho(Str))
        return(0);
    for(cont=0 ; (p!=NULL)&&(cont!=qtd) ; cont++)
    {
        q=p->next;
        if(q==NULL)     //Estamos em uma posicao que nao tem a quantidade (qtd) de nós na frente;
            return(0);
        p->next=q->next;
        free(q);
    }
    return(1);
}
short InsereStrPos(struct StrDim *Str, struct StrDim *SubS, int start)
{
    int cont=0;
    struct NoS *p=NULL;
    for(p=Str->ini ; (p!=NULL)&&(cont!=start) ; p=p->next)  //Já sai com p na posição certa e ant na posicão anterior
        cont++;
    if(p==NULL)
        return(0);
    if(p==Str->fim) //Quando a inserção é no fim, usa-se a funcao de concatenação
    {
        ConcatenaString2(Str, SubS);
        return(1);
    }
    SubS->fim->next=p->next;
    p->next=SubS->ini;
    return(1);
}
short VerificaStringMenor(struct StrDim *S1, struct StrDim *S2)
{
    struct NoS *p=NULL, *q=NULL;
    p=S1->ini;
    q=S2->ini;
    while((p->info==q->info)&&(p!=NULL)&&(q!=NULL))  //caracter
    {
        p=p->next;
        q=q->next;
    }
    if(p->info < q->info)
        return(1);
    return(0);
}
short VerificaStringIgual(struct StrDim *S1, struct StrDim *S2)
{
    struct NoS *p=NULL, *q=NULL;
    if(TamanhoString(S1)!=TamanhoString(S2))
        return(0);
    p=S1->ini;
    q=S2->ini;
    while((p->info==q->info)&&(p->next!=NULL))
    {
        p=p->next;
        q=q->next;
    }
    if((p->info==q->info))
        return(1);
    return(0);
}
short Matching(struct StrDim *S1, struct StrDim *Padrao)
{
    struct NoS *p=NULL, *q=NULL;
    int TS1=0, TPad=0, MAX=0, it=0, cont=1;
    q=Padrao->ini;
    p=S1->ini;

    TS1=TamanhoString(S1);
    TPad=TamanhoString(Padrao);
    MAX=TS1-TPad;

    for(it=0 ; it<=MAX ; it++)
    {
        while((p->info==q->info)&&(cont<TPad))
        {
            cont++;            p=p->next;
            q=q->next;
        }
        if(cont==TPad)
            return(it);
        p=p->next;
    }
    return(-1);     //Não achou a string padrao em S1
}
void CriaStringOrd(struct StrDim *S1, int T)
{
    int i=0;
    char Ch;
    for(i=0 ; i<T ; i++)
    {
        printf("\n      %d: ", i);
        scanf(" %c", &Ch);
        InsereCaracOrd(S1, Ch);
    }
}

int main (void)
{
    struct StrDim S1, S2, SAux, SConc;
    int Tam, i, qtd, pos, loc;
    char C;
    CriaStringVazia(&S1);
    //CriaStringVazia(&S2);
    //CriaStringVazia(&SConc);
    //CriaStringVazia(&S3);
    //CriaStringVazia(&SAux);

    printf("\n      Numero de elementos de S1: ");
    scanf("%d", &Tam);
    CriaStringOrd(&S1, Tam);
    ImprimeString(S1);

    //printf("\n      Numero de elementos de S2: ");
    //scanf("%d", &Tam);
    //CriaStringOrd(&S2, Tam);
    //ImprimeString(S2);

    //ConcatenaString(&S1, &S2, &SConc);
    //printf("\n      Concatenacao: ");
    //ImprimeString(SConc);

    //CopiaString(&S1, &SAux);
    //printf("\n      Copia: ");
    //ImprimeString(SAux);

    //printf("\n      Remove elemento start:");
    //printf("\n      Quantidade: ");
    //scanf(" %d", &qtd);
    //printf("\n      Posicao: ");
    //scanf(" %d", &pos);
    //RemoveStrPos(&S1, qtd, pos);
    //ImprimeString(S1);

    //printf("\n      Insere elemento start:");
    //printf("\n      Posicao: ");
    //scanf(" %d", &pos);
    //InsereStrPos(&S1, &S2, pos);
    //ImprimeString(S1);

    //printf("\n      Verifica string igual:");
    //if(VerificaStringIgual(&S1, &S2))
    //    printf("\n      Sao iguais!!!");
    //else
    //    printf("\n      Sao diferentes!!!");

    //printf("\n      Verifica string menor:");
    //if(VerificaStringMenor(&S1, &S2))
    //    printf("\n      S1 menor!!!");
    //else
    //    printf("\n      S2 menor!!!");

    //printf("\n      Matching strings:");
    //loc=Matching(&S1, &S2);
    //if(loc==-1)
    //    printf("\n      Padrao nao encontrado!!!");
    //else
    //    printf("\n      O padrao aparece na posicao [0,n): %d", loc);

}
