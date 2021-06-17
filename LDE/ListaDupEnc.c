#include <stdio.h>
#include <stdlib.h>

struct nod
{
    int info;
    struct nod *next, *last;
};
struct lista
{
    struct nod *ini, *fim;
};

//--------------------------Exemplos aula lista---------------------------------------
void InicializaLista(struct lista *L)
{
    L->ini=NULL;
    L->fim=NULL;
}
short InsereNoInicio(int valor, struct lista *L)
{
    struct nod *p=NULL;
    p=(struct nod *)malloc(sizeof(struct nod));
    if(p==NULL)
        return(0);
    p->info=valor;
    p->last=p->next=NULL;
    if(L->ini==NULL)
        L->fim=p;
    else
    {
        p->next=L->ini;
        L->ini->last=p;
    }
    L->ini=p;
}
short ListaVazia(struct lista *L)
{
    if(L->ini==NULL)
        return(1);
    return(0);
}
void ImprimeLista(struct lista *L)
{
    struct nod *p=NULL;
    if(ListaVazia(L))
        printf("\n        Lista vazia!");
    else
    {
        for(p=L->ini ; p!=NULL ; p=p->next)
            printf("%d ", p->info);
    }
    printf("\n\n");
}
struct nod* BuscaNo(int valor, struct lista *L)
{
    struct nod *p=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info<=valor) ; p=p->next) //Se a lista estiver ordenada, o código fica mais eficiente ao colocar p->info<=valor
    {
        if(p->info==valor)
            return(p);
    }
    return(NULL);
}
short RemoveElemento(int valor, struct lista *L)
{
    struct nod *p=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info!=valor) ; p=p->next);
    if(p==NULL)
        return(0);
    if(p->last==NULL)
    {
        L->ini=p->next;
        L->ini->last=NULL;
        free(p);
    }
    else if(p->next==NULL)
    {
        L->fim=p->last;
        L->fim->next=NULL;
        free(p);
    }
    else
    {
        p->last->next=p->next;
        p->next->last=p->last;
        free(p);
    }
    return(1);
}
int Tamanho(struct lista *L)                                                        //L1
{
    struct nod *p=NULL;
    int t=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        t++;
    return(t);
}
short InsereNoPosicao(int valor, int pos, struct lista *L)
{
    struct nod *p=NULL, *q=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(cont<pos) ; p=p->next)
        cont++;
    if(p==NULL)
        return(0);      //Lista nao tem essa posição - pos>Tamanho(Lista)
    q=(struct nod *)malloc(sizeof(struct nod));
    if(q==NULL)
        return(0);
    q->info=valor;
    q->next=q->last=NULL;
    if(p->last==NULL)               //Primeiro elemento, logo não será possivel utilizar o ponteiro ant=NULL
    {
        q->next=p;
        p->last=q;
        L->ini=q;
    }
    else
    {
        q->next=p;
        q->last=p->last;
        p->last->next=q;
        p->last=p;
    }
}
short RemovePrimeroElemento(struct lista *L)
{
    struct nod *p=NULL;
    if(L->ini==NULL)
        return(0); //Lista vazia
    p=L->ini;
    L->ini=p->next;
    L->ini->last=NULL;
    free(p);
    return(1);
}
short InsereNoOrdenado(int valor, struct lista *L)
{
    struct nod *p=NULL, *q=NULL;
    p=(struct nod *)malloc(sizeof(struct nod));
    if(p==NULL)
        return(0);
    p->info=valor;
    p->last=p->next=NULL;
    if(L->ini==NULL)
        L->ini=L->fim=p;
    else
    {
        if(valor<=L->ini->info)
        {
            p->next=L->ini;
            L->ini->last=p;
            L->ini=p;
        }
        else if(valor>=L->fim->info)
        {
            p->last=L->fim;
            L->fim->next=p;
            L->fim=p;
        }
        else
        {
            for(q=L->ini ; q!=NULL ; q=q->next)
                if(valor <= q->next->info)
                {
                    p->next=q->next;
                    p->last=q;
                    q->next->last=p;
                    q->next=p;
                    break;
                }
        }
    }
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
}
short InsereNoFim(int valor, struct lista *L)
{
    struct nod *p=NULL;
    p=(struct nod *)malloc(sizeof(struct nod));
    if(p==NULL)
        return(0);
    p->info=valor;
    p->last=p->next=NULL;
    if(L->ini==NULL)
        L->ini=p;
    else
    {
        p->last=L->fim;
        L->fim->next=p;
    }
    L->fim=p;
}
void InverteLista(struct lista *L)
{
    struct nod *p=NULL, *q=NULL;
    int aux=0, cont=0, T=0;
    T=Tamanho(L)/2;
    p=L->ini;
    q=L->fim;
    while(cont<T)
    {
        aux=p->info;
        p->info=q->info;
        q->info=aux;

        p=p->next;
        q=q->last;

        cont++;
    }
}
void InverteLista3(struct lista *L)
{
    struct nod *p=NULL, *q=NULL;
    int aux=0;
    p=L->ini;
    q=L->fim;
    while(p<q)
    {
        aux=p->info;
        p->info=q->info;
        q->info=aux;

        p=p->next;
        q=q->last;
    }
}
void InverteLista2(struct lista *L1, struct lista *L2)
{
    struct nod *p=NULL;
    for(p=L1->ini ; p!=NULL ; p=p->next)
        InsereNoInicio(p->info, L2);
}
short ExcluiListaInteira(struct lista *L)
{
    struct nod *p=NULL;
    while(L->ini!=NULL)
    {
        p=L->ini;
        L->ini=p->next;
        L->ini->last=NULL;
        free(p);
    }
    return(1);
}
void OrdenaLista(struct lista *L)
{
    struct nod *p=NULL, *q=NULL;
    int continua=1, aux=0;
    while(continua)
    {
        continua=0;
        p=L->ini;
        q=p->next;
        while(q!=NULL)
        {
            if(q->info < p->info)
            {
                aux=p->info;
                p->info=q->info;
                q->info=aux;
                continua=1;
            }
            p=q;
            q=q->next;
        }
    }
}
void CopiaSemRepeticao(struct lista *L, struct lista *LCop)
{
    struct nod *p=NULL, *v=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(LCop->fim==NULL)     //Se for o primeiro elemento da lista, apenas coloca
            InsereNoFim(p->info, LCop);
        else if(p->info!=LCop->fim->info)   //Se já existir elementos na lista, eu vejo se é igual ao ultimo inserido
            InsereNoFim(p->info, LCop);
    }
}

//--------------------------Exercicios lista de exercicios----------------------------
int ContaNoRec(struct nod *L, int cont)                                             //L2
{
    cont++;
    if(L->next!=NULL)
        ContaNoRec(L->next, cont);
    else
        return(cont);
}
int ContaOcorrenciaX(struct lista *L, int X)                                        //L3
{
    struct nod *p=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL && p->info<=X) ; p=p->next) //Coloquei L.A[i].info<=X pois como a lista está ordenada, havera um numero menor de iterações
        if(p->info==X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMaiorX(struct lista *L, int X)                                   //L4
{
    struct nod *p=NULL;
    int cont=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        if(p->info>X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMenorX(struct lista *L, int X)                                   //L5
{
    struct nod *p=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(p->info<X) ; p=p->next) //Coloquei L.A[i].info<X pois como a lista está ordenada, havera um numero menor de iterações
        cont++;
    return(cont);
}
void ListaParImpar(struct lista *L, struct lista *LP, struct lista *LI)             //L6
{
    struct nod *p=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(p->info%2==0)
            InsereNoFim(p->info, LP);
        else
            InsereNoFim(p->info, LI);
    }
}
void ListaMaiorMenorX(struct lista *L, int X, struct lista *LMa, struct lista *LMe) //L7
{
    struct nod *p=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(p->info<X)
            InsereNoFim(p->info, LMe);
        else if (p->info>X)
            InsereNoFim(p->info, LMa);
    }
}
int TrocaXPorY(struct lista *L, int X, int Y)                                       //L8
{
    struct nod *p=NULL;
    int cont=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(p->info==X)
        {
            p->info=Y;
            cont++;
        }
    }
    return(cont);
}
void RemoveOcorrenciaX(struct lista *L, int X)                                      //L9
{
    struct nod *p=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(p->info==X)
        {
            p->last->next=p->next;
            p->next->last=p->last;
            free(p);
        }
    }
}
int ConteudoPrimeiroNo(struct lista *L)                                             //L11
{
    return(L->ini->info);
}
int ConteudoUltimoNo(struct lista *L)                                               //L12
{
    return(L->fim->info);
}
struct nod* PontUltimoNo(struct lista *L)                                           //L13
{
    struct nod *p=NULL;
    p=L->fim;
    return(p);
}
short InsereElementoDepoisPos(struct lista *L, int pos, int valor)                  //L14
{
    struct nod *p=NULL, *q=NULL;
    int cont=0;
    q=(struct nod *)malloc(sizeof(struct nod));
    q->info=valor;
    q->next=q->last=NULL;
    for(p=L->ini ; (p!=NULL)&&(cont!=pos) ; p=p->next) //pos [0,n)
        cont++;
    if(p==NULL)
        return(0);      //Fim de varredura
    if(p->next==NULL)           //Depois do ultimo elemento
    {
        q->last=p;
        p->next=q;
        L->fim=q;
    }
    else
    {
        q->next=p->next;
        q->last=p;
        p->next->last=q;
        p->next=q;
    }
    return(1);
}
short InsereElementoAntesPos(struct lista *L, int pos, int valor)                   //L15
{
    struct nod *p=NULL, *q=NULL;
    int cont=0;
    q=(struct nod *)malloc(sizeof(struct nod));
    q->info=valor;
    q->next=q->last=NULL;
    for(p=L->ini ; (p!=NULL)&&(cont!=pos) ; p=p->next) //pos [0,n)
        cont++;
    if(p==NULL)                 //Fim de varredura
        return(0);
    if(p->last==NULL)           //Antes do primeiro elemento
    {
        q->next=p;
        p->last=q;
        L->ini=q;
    }
    else
    {
        q->next=p;
        q->last=p->last;
        p->last->next=q;
        p->last=q;
    }
    return(1);
}
short RemoveElementoPos(struct lista *L, int pos)                                   //L16
{
    struct nod *p=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(cont!=pos) ; p=p->next)
        cont++;
    if(p==NULL)
        return(0);
    if(p->last==NULL)       //Remove a primeira posição
    {
        L->ini=p->next;
        L->ini->last=NULL;
        free(p);
    }
    else if(p->next==NULL)
    {
        L->fim=p->last;
        L->fim->next=NULL;
        free(p);
    }
    else
    {
        p->last->next=p->next;
        p->next->last=p->last;
        free(p);
    }
    return(1);
}
void IntercalaListaOrdenada(struct lista *L1, struct lista *L2, struct lista *L3)   //L17
{
    struct nod *p=NULL, *q=NULL;
    p=L1->ini;
    q=L2->ini;
    while((p!=NULL)&&(q!=NULL))
    {
        if(p->info < q->info)
        {
            InsereNoFim(p->info, L3);
            p=p->next;
        }
        else if(q->info < p->info)
        {
            InsereNoFim(q->info, L3);
            q=q->next;
        }
        else        //Os elementos são iguais, logo basta colocar só um
        {
            InsereNoFim(p->info, L3);
            p=p->next;
            q=q->next;
        }
    }
    while(p!=NULL)
    {
        InsereNoFim(p->info, L3);
        p=p->next;
    }
    while(q!=NULL)
    {
        InsereNoFim(q->info, L3);
        q=q->next;
    }
}
void IntercalaLista(struct lista *L1, struct lista *L2, struct lista *L3)           //L18
{
    struct nod *p=NULL, *q=NULL;
    int aux=0;
    p=L1->ini;
    q=L2->ini;
    while((p!=NULL)&&(q!=NULL))
    {
        if(aux%2==0)
        {
            InsereNoFim(p->info, L3);
            p=p->next;
        }
        else
        {
            InsereNoFim(q->info, L3);
            q=q->next;
        }
        aux++;
    }
    while(p!=NULL)
    {
        InsereNoFim(p->info, L3);
        p=p->next;
    }
    while(q!=NULL)
    {
        InsereNoFim(q->info, L3);
        q=q->next;
    }
}
void ConcatenaDuasListas(struct lista *L1, struct lista *L2)                        //L19
{
    L1->fim->next=L2->ini; //L1 e L2 estão concatenadas em L1
    L1->fim=L2->fim;
}
void CopiaLista(struct lista *L1, struct lista *L2)                                 //L20
{
    struct nod *p=NULL, *q=NULL;
    for(p=L1->ini ; p!=NULL ; p=p->next)
    {
        q=(struct nod *)malloc(sizeof(struct nod));
        q->info=p->info;
        q->next=q->last=NULL;
        if(L2->ini==NULL)
            L2->ini=q;
        else
        {
            q->last=L2->fim;
            L2->fim->next=q;
        }
        L2->fim=q;
    }
}
short VerificaListaIdentica(struct lista *L1, struct lista *L2)                     //L21
{
    struct nod *p=NULL, *q=NULL;
    if(Tamanho(L1)!=Tamanho((L2)))
        return(0);
    p=L1->ini;
    q=L2->ini;
    while((p!=NULL)&&(q!=NULL))
    {
        if(p->info!=q->info)
            return(0);
        p=p->next;
        q=q->next;
    }
    return(1);
}
int SomaLista(struct lista *L)                                                      //L22
{
    struct nod *p=NULL;
    int soma=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        soma+= p->info;
    return(soma);
}
int SubtracaoLista(struct lista *L)                                                 //L23
{
    struct nod *p=NULL;
    int sub=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        sub-= p->info;
    return(sub);
}
int ProdutoLista(struct lista *L)                                                   //L24
{
    struct nod *p=NULL;
    int mult=1;
    for(p=L->ini ; p!=NULL ; p=p->next)
        mult*= p->info;
    return(mult);
}
void CriaListaCaracteres(struct lista *L, char *S)                                  //L27
{
    int i=0;
    struct nod *p=NULL;
    for(i=0 ; *(S+i)!='\0' ; i++)
    {
        if((int)*(S+i)!=44) //Se não for a virgula, coloca na lista
        {
            p=(struct nod *)malloc(sizeof(struct nod));
            p->info=*(S+i);
            p->last=p->next=NULL;
            if(L->ini==NULL)
                L->ini=p;
            else
            {
                p->last=L->fim;
                L->fim->next=p;
            }
            L->fim=p;
        }
    }
}
void RemoveNoPosicaoArray(struct lista *L, int *L2, int D)                          //L28
{
    int i=0, pos=0;
    for(i=0 ; i<D ; i++)
    {
        pos=*(L2+i);
        RemoveElementoPos(L, pos-1-i); //Tira 1 para deixar a posição entre [0, n)
    }
}


int main(void)
{
    struct lista L, L1, LConc, LInvert, LAux, LPar, LImpar;
    struct nod *LPtr;
    int Elementos=0, X[3]= {1,3,5};

    InicializaLista(&L1);
    printf("\n      Numero de elementos: ");
    scanf("%d", &Elementos);
    CriaListaOrdenada(&L1, Elementos);
    ImprimeLista(&L1);

    /*
    printf("\n\n      NUMERO DE ELEMENTOS COM VALOR X: ");
    scanf("%d", &X);
    printf("\n      %d ocorre %d vezes!", X, ContaOcorrenciaX(&L1, X));

    printf("\n\n      NUMERO DE ELEMENTOS RECURSIVO: %d\n", ContaNoRec(L1.ini, 0));

    InicializaLista(&LPar);
    InicializaLista(&LImpar);
    ListaMaiorMenorX(&L1, 5, &LPar, &LImpar);
    ImprimeLista(&LPar);
    ImprimeLista(&LImpar);

    LPtr=PontUltimoNo(&L1);
    printf("\n      ULTIMO NO: %d", *LPtr);
    */

    //InsereElementoAntesPos(&L1, 0, 100);
    //InsereElementoAntesPos(&L1, 3, 100);
    //InsereElementoAntesPos(&L1, 5, 100);
    //ImprimeLista(&L1);

    //CopiaSemRepeticao(&L1, &L);
    //ImprimeLista(&L);

}
