#include <stdio.h>
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

//--------------------------Exemplos aula lista---------------------------------------
void InicializaLista(struct lista *L)
{
    L->ini=NULL;
    L->fim=NULL;
}
short InsereNoInicio(int valor, struct lista *L)
{
    struct no *p=NULL;
    p=(struct no *)malloc(sizeof(struct no));
    if(p==NULL)
        return(0);
    p->next=NULL;
    p->info=valor;
    if(L->ini==NULL) //Lista vazia
        L->fim=p;
    else
        p->next=L->ini;
    L->ini=p;
    return(1);
}
short ListaVazia(struct lista *L)
{
    if(L->ini==NULL)
        return(1);
    return(0);
}
void ImprimeLista(struct lista *L)
{
    struct no *p=NULL;
    if(ListaVazia(L))
        printf("\n        Lista vazia!");
    else
    {
        for(p=L->ini ; p!=NULL ; p=p->next)
            printf("%d ", p->info);
    }
    printf("\n\n");
}
struct no* BuscaNo(int valor, struct lista *L)
{
    struct no *p=NULL;
    for(p=L->ini ; (p!=NULL)&&(p->info<=valor) ; p=p->next) //Se a lista estiver ordenada, o código fica mais eficiente ao colocar p->info<=valor
    {
        if(p->info==valor)
            return(p);
    }
    return(NULL);
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
struct no* getPriElem(struct lista *L)
{
    return(L->ini);
}
struct no* getUltElem(struct lista *L)
{
    return(L->fim);
    /*for (p=L->ini; p != NULL; p=p->next) Se não soubessemos quem era o L->fim
     if (p->next == NULL ) return(p);*/
}
int Tamanho(struct lista *L)
{
    struct no *p=NULL;
    int t=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        t++;
    return(t);
}
short InsereNoPosicao(int valor, int pos, struct lista *L)
{
    struct no *p=NULL, *ant=NULL, *q=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(cont<pos) ; p=p->next)
    {
        ant=p;
        cont++;
    }
    if(p==NULL)
        return(0);      //Lista nao tem essa posição - pos>Tamanho(Lista)
    q=(struct no *)malloc(sizeof(struct no));
    if(q==NULL)
        return(0);
    q->info=valor;
    q->next=NULL;
    if(ant==NULL)               //Primeiro elemento, logo não será possivel utilizar o ponteiro ant=NULL
    {
        q->next=p;
        L->ini=q;
    }
    else
    {
        q->next=p;
        ant->next=q;
    }
}
short RemovePrimeroElemento(struct lista *L)
{
    struct no *p=NULL;
    if(L->ini==NULL)
        return(0); //Lista vazia
    p=L->ini;
    L->ini=p->next;
    free(p);
    return(1);
}
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
short VerificaListaOrdenada(struct lista *L)
{
    struct no *p=NULL;
    p=L->ini;
    for( ; p!=NULL ; p=p->next)
    {
        if(p->next->info < p->info)       //Essa situação é quando a lista está em ordem crescente, quando for em ordem decrescente é ">"
            return(0);
    }
    return(1);
}
short InsereNoFim(int valor, struct lista *L)
{
    struct no *p=NULL;
    p=(struct no *)malloc(sizeof(struct no));
    if(p==NULL)
        return(0);
    p->next=NULL;
    p->info=valor;
    if(L->ini==NULL) //Lista vazia
        L->ini=p;
    else
        L->fim->next=p;
    L->fim=p;
    return(1);
}
void CopiaLista(struct lista *L1, struct lista *L2)                                 //L20
{
    struct no *p=NULL;
    for(p=L1->ini ; p!=NULL ; p=p->next)
        InsereNoFim(p->info, L2);
}
void InverteLista2(struct lista *L1, struct lista *L2)
{
    struct no *p=NULL;
    for(p=L1->ini ; p!=NULL ; p=p->next)
        InsereNoInicio(p->info, L2);
}
void CopiaSemRepeticao(struct lista *L, struct lista *LCop)
{
    struct no *p=NULL, *v=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(LCop->fim==NULL)     //Se for o primeiro elemento da lista, apenas coloca
            InsereNoFim(p->info, LCop);
        else if(p->info!=LCop->fim->info)   //Se já existir elementos na lista, eu vejo se é igual ao ultimo inserido
            InsereNoFim(p->info, LCop);
    }
}
void InverteElementos(struct no *p, struct no *ant)
{
    if(p->next!=NULL)
        InverteElementos(p->next, p);
    p->next=ant;
}
void InverteLista(struct lista *L1)
{
    struct no *aux=NULL;
    InverteElementos(L1->ini, NULL); //Invertendo os ponteiros de forma recursiva
    aux=L1->ini;                     //Trocando os ponteiros do inicio e do fim
    L1->ini=L1->fim;
    L1->fim=aux;
}
struct lista* ConcatenaDuasListas2(struct lista *L1, struct lista *L2)
{
    struct no *p=NULL;
    p=L1->fim;
    p->next=L2->ini; //L1 e L2 estão concatenadas em L1
    return(L1);
}
void OrdenaLista(struct lista *L)
{
    struct no *p=NULL, *q=NULL;
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
short ExcluiListaInteira(struct lista *L)
{
    struct no *p=NULL;
    while(L->ini!=NULL)
    {
        p=L->ini;
        L->ini=p->next;
        free(p);
    }
    return(1);
}

//--------------------------Exercicios lista de exercicios----------------------------
int ContaNoRec(struct no *L, int cont)                                              //L2
{
    cont++;
    if(L->next!=NULL)
        ContaNoRec(L->next, cont);
    else
        return(cont);
}
int ContaOcorrenciaX(struct lista *L, int X)                                        //L3
{
    struct no *p=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL && p->info<=X) ; p=p->next) //Coloquei L.A[i].info<=X pois como a lista está ordenada, havera um numero menor de iterações
        if(p->info==X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMaiorX(struct lista *L, int X)                                   //L4
{
    struct no *p=NULL;
    int cont=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        if(p->info>X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMenorX(struct lista *L, int X)                                   //L5
{
    struct no *p=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(p->info<X) ; p=p->next) //Coloquei L.A[i].info<X pois como a lista está ordenada, havera um numero menor de iterações
        cont++;
    return(cont);
}
void ListaParImpar(struct lista *L, struct lista *LP, struct lista *LI)             //L6
{
    struct no *p=NULL;
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
    struct no *p=NULL;
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
    struct no *p=NULL;
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
    struct no *p=NULL;
    for(p=L->ini ; p!=NULL ; p=p->next)
    {
        if(p->info==X)
        {
            RemoveElemento(X, L);
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
struct no* PontUltimoNo(struct lista *L)                                            //L13
{
    struct no *p=NULL;
    p=L->fim;
    return(p);
}
short InsereElementoDepoisPos(struct lista *L, int pos, int valor)                  //L14
{
    struct no *p=NULL, *q=NULL;
    int cont=0;
    q=(struct no *)malloc(sizeof(struct no));
    q->info=valor;
    q->next=NULL;
    for(p=L->ini ; (p!=NULL)&&(cont!=pos) ; p=p->next) //pos [0,n)
        cont++;
    if(p==NULL)
        return(0);      //Fim de varredura
    if(p->next==NULL)           //Depois do ultimo elemento
    {
        p->next=q;
        L->fim=q;
    }
    else
    {
        q->next=p->next;
        p->next=q;
    }
    return(1);
}
short InsereElementoAntesPos(struct lista *L, int pos, int valor)                   //L15
{
    struct no *p=NULL, *q=NULL, *ant=NULL;
    int cont=0;
    q=(struct no *)malloc(sizeof(struct no));
    q->info=valor;
    q->next=NULL;
    for(p=L->ini ; (p!=NULL)&&(cont!=pos) ; p=p->next) //pos [0,n)
    {
        ant=p;
        cont++;
    }
    if(p==NULL)                 //Fim de varredura
        return(0);
    if(ant==NULL)           //Antes do primeiro elemento
    {
        q->next=p;
        L->ini=q;
    }
    else
    {
        q->next=p;
        ant->next=q;
    }
    return(1);
}
short RemoveElementoPos(struct lista *L, int pos)                                   //L16
{
    struct no *p=NULL, *ant=NULL;
    int cont=0;
    for(p=L->ini ; (p!=NULL)&&(cont<pos) ; p=p->next)
    {
        ant=p;
        cont++;
    }
    if(p==NULL)
        return(0);
    if(ant==NULL)       //Remove a primeira posição
    {
        L->ini=p->next;
        free(p);
    }
    else if(p==L->fim)
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
}
void IntercalaListaOrdenada(struct lista *L1, struct lista *L2, struct lista *L3)   //L17
{
    struct no *p=NULL, *q=NULL;
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
    struct no *p=NULL, *q=NULL;
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
short VerificaListaIdentica(struct lista *L1, struct lista *L2)                     //L21
{
    struct no *p=NULL, *q=NULL;
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
    struct no *p=NULL;
    int soma=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        soma+= p->info;
    return(soma);
}
int SubtracaoLista(struct lista *L)                                                 //L23
{
    struct no *p=NULL;
    int sub=0;
    for(p=L->ini ; p!=NULL ; p=p->next)
        sub-= p->info;
    return(sub);
}
int ProdutoLista(struct lista *L)                                                   //L24
{
    struct no *p=NULL;
    int mult=1;
    for(p=L->ini ; p!=NULL ; p=p->next)
        mult*= p->info;
    return(mult);
}
void CriaListaCaracteres(struct lista *L, char *S)                                  //L27
{
    int i=0;
    struct no *p=NULL;
    for(i=0 ; *(S+i)!='\0' ; i++)
    {
        if((int)*(S+i)!=44) //Se não for a virgula, coloca na lista
        {
            p=(struct no *)malloc(sizeof(struct no));
            p->info=*(S+i);
            p->next=NULL;
            if(L->ini==NULL)
                L->ini=p;
            else
                L->fim->next=p;
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
    struct no *LPtr;
    int Elementos=0, X=0;

    InicializaLista(&L1);
    printf("\n      Numero de elementos: ");
    scanf("%d", &Elementos);
    CriaListaOrdenada(&L1, Elementos);
    ImprimeLista(&L1);

    /*
    printf("\n\n      NUMERO DE ELEMENTOS COM VALOR X: ");
    scanf("%d", &X);
    printf("\n      %d ocorre %d vezes!", X, ContaOcorrenciaX(&L1, X));
    */

    //printf("\n\n      NUMERO DE ELEMENTOS RECURSIVO: %d", ContaNoRec(L1.ini, 0));

    /*
    InicializaLista(&LPar);
    InicializaLista(&LImpar);
    ListaMaiorMenorX(&L1, 5, &LPar, &LImpar);
    ImprimeLista(&LPar);
    ImprimeLista(&LImpar);
    */

    /*
    LPtr=PontUltimoNo(&L1);
    printf("\n      ULTIMO NO: %d", *LPtr);
    */

    //InsereElementoDepoisPos(&L1, 0, 100);
    //InsereElementoDepoisPos(&L1, 3, 100);
    //InsereElementoDepoisPos(&L1, 5, 100);
    //ImprimeLista(&L1);

    //CopiaSemRepeticao(&L1, &L);
    //ImprimeLista(&L);

}
