#include <stdio.h>
#define MAX 15

struct reg
{
    int info;
    int next;
};
struct lista
{
    int disp;
    int prim;
    struct reg A[MAX];
};

//--------------------------Exemplos aula lista---------------------------------------
void InicializaLista(struct lista *L)
{
    int i=0;
    L->disp=0;
    L->prim=-1;
    for(i=0 ; i<MAX-1 ; i++)
        L->A[i].next=i+1;
    L->A[i].next= -1;
}
short Vazia(struct lista L)
{
    if(L.prim==-1)
        return(1);
    return(0);
}
void ImprimeLista(struct lista L)
{
    int i=0;
    if(Vazia(L))
        printf("\n   Lista vazia!");
    printf("\n Indice     Info    Next");
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
    {
        printf("\n %6d  -  %4d    %4d", i, L.A[i].info, L.A[i].next);
    }
    printf("\n\n [Prim]=%d        [Disp]=%d\n", L.prim, L.disp);
    return;
}
void InsereElementoOrdenado(struct lista *L, int X)
{
    int PROXIMO=0, ANTERIOR=0, ATUAL=0, aux=0;
    if(L->disp != -1)       //Lista não está cheia
    {
        L->A[L->disp].info = X;
        PROXIMO = L->A[L->disp].next;
        if(L->prim == -1)       //Lista vazia
        {
            L->prim=L->disp;
            L->A[L->disp].next = -1;
        }
        else if(X < L->A[L->prim].info)      //Tem elementos //Inserção no início da lista
        {
            L->A[L->disp].next = L->prim;
            L->prim = L->disp;
        }
        else     //Não será inserido no inicio, sendo assim devemos procurar em qual posição ele ficará
        {
            ANTERIOR=L->prim;
            for(ATUAL=L->A[L->prim].next ; (ATUAL!=-1) && (X > L->A[ATUAL].info) ; ATUAL = L->A[ATUAL].next)
                ANTERIOR=ATUAL;
            L->A[ANTERIOR].next = L->disp;
            L->A[L->disp].next = ATUAL;
        }
        L->disp = PROXIMO;
    }
    else        //Lista está cheia
    {
        aux= L->prim;
        if(X < L->A[aux].info)
            L->A[aux].info = X;
        else
            while(L->A[aux].info < X)
            {
                aux=L->A[aux].next;
            }
        L->A[aux].info = X;
    }
    return;
}
void InsereElementoOrdenado2(struct lista *L, int X)
{
    int PROXIMO=0, ANT=-1, ATUAL=0;
    L->A[L->disp].info=X;
    PROXIMO=L->A[L->disp].next;
    L->A[L->disp].next=-1;
    if(L->prim==-1)
        L->prim=L->disp;
    else
    {
        for(ATUAL=L->prim ; (ATUAL!=-1)&&(L->A[ATUAL].info<X) ; ATUAL=L->A[ATUAL].next)
            ANT=ATUAL;
        if(ANT==-1)     //Primeiro elemento
        {
            L->A[L->disp].next=L->prim;
            L->prim=L->disp;
        }
        else
        {
            if(ATUAL!=-1)   //Elemento do meio
                L->A[L->disp].next=L->A[ANT].next;
            L->A[ANT].next=L->disp;
        }
    }
    L->disp=PROXIMO;
}
int ContaNo(struct lista *L)                                                        //L1
{
    int i=0, cont=0;
    for(i=L->prim ; i!=-1 ; i=L->A[i].next)
        cont++;
    return(cont);
}
void RemoveElemento(struct lista *L, int X)
{
    int ATUAL=0, ANTERIOR=0, TAMANHO=0;
    TAMANHO=ContaNo(L);
    ANTERIOR=-1;

    if(L->prim!=-1)
    {
        if(TAMANHO>1)
        {
            for(ATUAL=L->prim ; (ATUAL!=-1) && (X > L->A[ATUAL].info) ; ATUAL=L->A[ATUAL].next)
                ANTERIOR=ATUAL;
            if(ATUAL!=-1 && X==L->A[ATUAL].info)
            {
                if(ANTERIOR==-1) 					//Remove o primeiro elemento.
                {
                    L->disp = ATUAL;
                    L->prim = L->A[ATUAL].next;
                }
                else if(L->A[ATUAL].next==-1)        //Remove ultimo elemento
                {
                    L->A[ANTERIOR].next = -1;
                }
                else 											//Remove no meio.
                {
                    L->disp = L->A[ANTERIOR].next;
                    L->A[ANTERIOR].next = L->A[ATUAL].next;
                }
            }
        }
        else
        {
            if(L->A[L->prim].info==X) 	//Apenas um elemento.
            {
                L->prim=-1;
                L->A[L->prim].next=L->disp;
                L->disp=L->prim;
            }
        }
        TAMANHO--;
    }
    return;
}
void CriaListaOrdenada(struct lista *L, int N)
{
    int i=0, V=0;
    for(i=0 ; i<N ; i++)
    {
        printf("\n      %d = ", i);
        scanf("%d", &V);
        InsereElementoOrdenado(L, V);
    }
}
short Busca(struct lista L, int V)
{
    int i=0;
    for(i=L.prim ; ((i!=-1)&&(L.A[i].info<=V)) ; i=L.A[i].next)
    {
        if(L.A[i].info == V)
            return(1);
    }
    return(0);
}
void InsereNoFim(int valor, struct lista *L)
{
    int PROXIMO=0, ATUAL=0;
    L->A[L->disp].info=valor;
    PROXIMO=L->A[L->disp].next;
    L->A[L->disp].next=-1;
    if(L->prim==-1) //Lista vazia
        L->prim=L->disp;
    else			//Lista com elementos, então vamos mudar o next do penultimo (já que o ultimo é o valor inserido)
    {
        for(ATUAL=L->prim ; L->A[ATUAL].next!=-1 ; ATUAL=L->A[ATUAL].next);	//O "for" para quando chega no indice do penultino elemento (next=-1)
        L->A[ATUAL].next=L->disp;
    }
    L->disp=PROXIMO;
}

//--------------------------Exercicios lista de exercicios----------------------------
int ContaNoRec(struct lista L, int i, int cont)                                     //L2
{
    cont++;
    if(L.A[i].next!=-1)
        ContaNoRec(L, i+1, cont);
    else
        return(cont);
}
int ContaOcorrenciaX(struct lista L, int X)                                         //L3
{
    int i=0, cont=0;
    for(i=L.prim ; (i!=-1 && L.A[i].info<=X) ; i=L.A[i].next) //Coloquei L.A[i].info<=X pois como a lista está ordenada, havera um numero menor de iterações
        if(L.A[i].info==X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMaiorX(struct lista L, int X)                                    //L4
{
    int i=0, cont=0;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
        if(L.A[i].info>X)
            cont++;
    return(cont);
}
int ContaOcorrenciaMenorX(struct lista L, int X)                                    //L5
{
    int i=0, cont=0;
    for(i=L.prim ; (i!=-1 && L.A[i].info<X) ; i=L.A[i].next) //Coloquei L.A[i].info<=X pois como a lista está ordenada, havera um numero menor de iterações
        cont++;
    return(cont);
}
void ListaParImpar(struct lista L, struct lista *LP, struct lista *LI)              //L6
{
    int i=0;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
    {
        if(((L.A[i].info)%2)==0)
            InsereNoFim(L.A[i].info, LP);
        else
            InsereNoFim(L.A[i].info, LI);
    }
    return;
}
void ListaMaiorMenorX(struct lista L, int X, struct lista *LMa, struct lista *LMe)  //L7
{
    int i=0;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
    {
        if((L.A[i].info)<X)
            InsereNoFim(L.A[i].info, LMe);
        else if((L.A[i].info)>X)
            InsereNoFim(L.A[i].info, LMa);
    }
    return;
}
int TrocaXPorY(struct lista *L, int X, int Y)                                       //L8
{
    int i=0, cont=0;
    for(i=L->prim ; i!=-1 ; i=L->A[i].next)
        if(L->A[i].info==X)
        {
            L->A[i].info=Y;
            cont++;
        }
    return(cont);
}
void RemoveOcorrenciaX(struct lista *L, int X)                                      //L9
{
    int i=0;
    for(i=L->prim ; i!=-1 ; i=L->A[i].next)
        if(L->A[i].info==X)
            RemoveElemento(L, X);
}
int ConteudoPrimeiroNo(struct lista L)                                              //L11
{
    return(L.A[L.prim].info);
}
int ConteudoUltimoNo(struct lista L)                                                //L12
{
    int i=0;
    for(i=L.prim ; L.A[i].next!=-1 ; i=L.A[i].next);
    return(L.A[i].info);
}
struct lista* PontUltimoNo(struct lista L)                                          //L13
{
    struct lista *p=NULL;
    p=&L;
    int i=0;
    for(i=L.prim ; L.A[i].next!=-1 ; i=L.A[i].next);
    return(p+i);
}
short InsereElementoDepoisPos(struct lista *L, int pos, int valor)                  //L14
{
    //iremos mudar o ponteiro de pos para que o valor seja o (pos+1)º elemento da lista
    int PROXIMO=0, ATUAL=0, cont=0, aux=0;
    L->A[L->disp].info=valor;
    PROXIMO=L->A[L->disp].next;
    L->A[L->disp].next=-1;

    for(ATUAL=L->prim ; (ATUAL!=-1)&&(cont!=pos) ; ATUAL=L->A[ATUAL].next) //pos [0, n) (se fosse [1,n] cont é inicializado com 1
        cont++;
    if(ATUAL==-1)           //Posicao náo encontrada - fim de varredra
        return(0);
    if(L->A[ATUAL].next==-1)        //Será inserido como ultimo elemento (posicao n-1)
        L->A[ATUAL].next=L->disp;
    else
    {
        aux=L->A[ATUAL].next;
        L->A[ATUAL].next=L->disp;
        L->A[L->disp].next=aux;
    }
    L->disp=PROXIMO;
    return(1);
}
short InsereElementoAntesPos(struct lista *L, int pos, int valor)                   //L15
{
    //iremos mudar o ponteiro da posicao anteiror de pos para que o valor seja o posº elemento da lista
    int PROXIMO=0, ATUAL=0, cont=0, ANTERIOR=-1;
    L->A[L->disp].info=valor;
    PROXIMO=L->A[L->disp].next;
    L->A[L->disp].next=-1;

    for(ATUAL=L->prim ; (ATUAL!=-1)&&(cont!=pos) ; ATUAL=L->A[ATUAL].next) //pos [0, n) (se fosse [1,n] cont é inicializado com 1
    {
        ANTERIOR=ATUAL;
        cont++;
    }
    if(ATUAL==-1)           //Posicao náo encontrada - fim de varredra
        return(0);
    if(ANTERIOR==-1)        //Será inserido como 1º elemento (posicao 0)
        L->prim=L->disp;
    else
        L->A[ANTERIOR].next=L->disp;
    L->A[L->disp].next=ATUAL;

    L->disp=PROXIMO;
    return(1);
}
short RemoveElementoPos(struct lista *L, int pos)                                   //L16
{
    int ATUAL=0, cont=0, ANTERIOR=-1;
    for(ATUAL=L->prim ; (ATUAL!=-1)&&(cont!=pos) ; ATUAL=L->A[ATUAL].next) //pos [0, n) (se fosse [1,n] cont é inicializado com 1
    {
        ANTERIOR=ATUAL;
        cont++;
    }
    if(ANTERIOR==-1)     //remove o primeiro elemento (pos 0)
    {
        L->prim=L->A[ATUAL].next;
        L->A[ATUAL].next=L->disp;
        L->disp=ATUAL;
    }
    else if(L->A[ATUAL].next==-1)           //remove o ultimo elemento
    {
        L->A[ANTERIOR].next=-1;
        L->disp=ATUAL;
    }
    else
    {
        L->A[ANTERIOR].next=L->A[ATUAL].next;
        L->A[ATUAL].next=L->disp;
        L->disp=ATUAL;
    }
}
void IntercalaListaOrdenada(struct lista *L1, struct lista *L2, struct lista *L3)   //L17
{
    int i=0, j=0;
    i=L1->prim;
    j=L2->prim;
    while((i!=-1)&&(j!=-1))
    {
        if(L1->A[i].info < L2->A[j].info)
        {
            InsereNoFim(L1->A[i].info, L3);
            i=L1->A[i].next;
        }
        else if(L2->A[j].info < L1->A[i].info)
        {
            InsereNoFim(L2->A[j].info, L3);
            j=L2->A[j].next;
        }
        else        //Os elementos são iguais, logo basta colocar só um
        {
            InsereNoFim(L1->A[i].info, L3);
            i=L1->A[i].next;
            j=L2->A[j].next;
        }
    }
    while(i!=-1)
    {
        InsereNoFim(L1->A[i].info, L3);
        i=L1->A[i].next;
    }
    while(j!=-1)
    {
        InsereNoFim(L2->A[j].info, L3);
        j=L2->A[j].next;
    }
}
void IntercalaLista(struct lista *L1, struct lista *L2, struct lista *L3)           //L18
{
    int i=0, j=0, aux=0;
    i=L1->prim;
    j=L2->prim;
    while((i!=-1)&&(j!=-1))
    {
        if(aux%2==0)
        {
            InsereNoFim(L1->A[i].info, L3);
            i=L1->A[i].next;
        }
        else
        {
            InsereNoFim(L2->A[j].info, L3);
            j=L2->A[j].next;
        }
        aux++;
    }
    while(i!=-1)
    {
        InsereNoFim(L1->A[i].info, L3);
        i=L1->A[i].next;
    }
    while(j!=-1)
    {
        InsereNoFim(L2->A[j].info, L3);
        j=L2->A[j].next;
    }
}
void ConcatenaDuasListas(struct lista *L1, struct lista *L2)                        //L19
{
    int i=0;
    for(i=L2->prim ; i!=-1 ; i=L2->A[i].next)
    {
        InsereNoFim(L2->A[i].next, L1);
    }
}
void CopiaLista(struct lista *L1, struct lista *L2)                                 //L20
{
    int i=0;
    for(i=L1->prim ; i!=-1 ; i=L1->A[i].next)
        InsereNoFim(L1->A[i].info, L2);
}
short VerificaListaIdentica(struct lista *L1, struct lista *L2)                     //L21
{
    int i=0, j=0;
    if(ContaNo(L1)!=ContaNo(L2))
        return(0);
    i=L1->prim;
    j=L2->prim;
    while((i!=-1)&&(j!=-1))
    {
        if(L1->A[i].info!=L2->A[j].info)
            return(0);
        i=L1->A[i].next;
        j=L2->A[j].next;
    }
    return(1);
}
int SomaLista(struct lista L)                                                       //L22
{
    int i=0, soma=0;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
        soma+= L.A[i].info;
    return(soma);
}
int SubtracaoLista(struct lista L)                                                  //L23
{
    int i=0, sub=0;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
        sub -= L.A[i].info;
    return(sub);
}
int ProdutoLista(struct lista L)                                                    //L24
{
    int i=0, mult=1;
    for(i=L.prim ; i!=-1 ; i=L.A[i].next)
        mult*= L.A[i].info;
    return(mult);
}
void CriaListaCaracteres(struct lista *L, char *S)                                  //L27
{
    int i=0, PROXIMO=0, ATUAL=0;
    for(i=0 ; *(S+i)!='\0' ; i++)
    {
        if((int)*(S+i)!=44) //Se não for a virgula, coloca na lista (INSERE NO FIM)
        {
            L->A[L->disp].info=*(S+i);
            PROXIMO=L->A[L->disp].next;
            L->A[L->disp].next=-1;
            if(L->prim==-1) //Lista vazia
                L->prim=L->disp;
            else
            {
                for(ATUAL=L->prim ; L->A[ATUAL].next!=-1 ; ATUAL=L->A[ATUAL].next);
                L->A[ATUAL].next=L->disp;
            }
            L->disp=PROXIMO;
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
void InsereElementoNoFim(struct lista *L, int X)
{
    int PROXIMO=0, ANTERIOR=0, ATUAL=0, aux=0;
    if(L->disp != -1)       //Lista não está cheia
    {
        L->A[L->disp].info = X;
        PROXIMO = L->A[L->disp].next;
        if(L->prim == -1)       //Lista vazia
        {
            L->prim=L->disp;
            L->A[L->disp].next = -1;
        }
        else     //Não será inserido no inicio, sendo assim devemos procurar em qual posição ele ficará
        {
            ANTERIOR=L->prim;
            for(ATUAL=L->A[L->prim].next ; ATUAL!=-1 ; ATUAL = L->A[ATUAL].next)
                ANTERIOR=ATUAL;
            L->A[ANTERIOR].next = L->disp;
            L->A[L->disp].next = ATUAL;
        }
        L->disp = PROXIMO;
    }
    return;
}

int main(void)
{
    struct lista L1, L2, LPar, LImpar, *LPtr;
    int TAMANHO=0, REMOVE[3]= {1,3,5};
    int cont=0, X=0, Y=0;
	struct lista *ptr;

    InicializaLista(&L1);
    //printf("\n\n      CRIANDO UMA LISTA ESTATICA ENCADEADA.");
    //printf("\n      DIGITE O TAMANHO: ");
    //scanf("%d", &TAMANHO);
    //CriaListaOrdenada(&L1, TAMANHO);
    //printf("\n\n      LISTA ESTATICA ENCADEADA CRIADA!");
    ImprimeLista(L1);
    InsereElementoNoFim(&L1, 10);
    InsereElementoNoFim(&L1, 5);
    InsereElementoNoFim(&L1, 4);
    InsereElementoNoFim(&L1, 18);
    ImprimeLista(L1);


    //printf("\n\n      NUMERO DE ELEMENTOS: %d", ContaNo(&L1));
    //printf("\n\n      NUMERO DE ELEMENTOS RECURSIVO: %d", ContaNoRec(L1, X, cont));

    /*printf("\n\n      NUMERO DE ELEMENTOS COM VALOR X: ");
    scanf("%d", &X);
    printf("\n      %d ocorre %d vezes!", X, ContaOcorrenciaX(L1, X));
    */

    //LPtr=PontUltimoNo(L1);
    //printf("\n      ULTIMO NO: %d", *LPtr);


    //InicializaLista(&LPar);
    //InicializaLista(&LImpar);
    //ListaParImpar(L1, &LPar, &LImpar);
    //ConcatenaDuasListas(&LPar, &LImpar);
    //ImprimeLista(LPar);


    /*
    printf("\n      Lista PAR = ");
    ImprimeLista(LPar);
    printf("\n      Lista IMPAR = ");
    ImprimeLista(LImpar);
    */

    /*printf("\n      TROCAR X POR Y:");
    printf("\n      VALOR A SER SUBSTITUIDO (X) = ");
    scanf("%d", &X);
    printf("\n      VALOR A SUBSTITUIR (Y) = ");
    scanf("%d", &Y);
    TrocaXPorY(&L1, X, Y);
    ImprimeLista(L1);
    */

    /*
    printf("\n      BUSCAR UM VALOR:");
    printf("\n      X=");
    scanf("%d", &X);
    if(Busca(L1, X)) printf("\n      VALOR ENCONTRADO!");
    else printf("\n     NAO ENCONTREI :( ");
    */

    /*
    printf("\n      SOMA VALORES: %d", SomaLista(L1));
    printf("\n      SUBTRAI VALORES: %d", SubtracaoLista(L1));
    printf("\n      MULTIPLICA VALORES: %d", ProdutoLista(L1));
    */

    /*
    printf("\n      REMOVE OCORRENCIA DE X:");
    printf("\n      X=");
    scanf("%d", &X);
    RemoveOcorrenciaX(&L1, X);
    ImprimeLista(L1);
    */

    /*
    InicializaLista(&L2);
    CopiaLista(&L1, &L2);
    ImprimeLista(L2);
    */

    /*
    if(VerificaListaIdentica(&L1, &L2))
        printf("\n      LISTAS IGUAIS!");
    else printf("\n     LISTAS DIFERENTES!");

    if(VerificaListaIdentica(&LImpar, &LPar))
        printf("\n      LISTAS IGUAIS PAR IMPAR!");
    else printf("\n     LISTAS DIFERENTES PAR IMPAR!");
    */


    /*
    printf("\n      INTERCALANDO!");
    IntercalaListaOrdenada(&LPar, &LImpar, &L2);
    ImprimeLista(L2);
    */

    //printf("\n      CONTEUDO DO PRIMEIRO NO: %d", ConteudoPrimeiroNo(L1));

    //printf("\n      CONTEUDO DO ULTIMO NO: %d", ConteudoUltimoNo(L1));

    /*
    InsereElementoDepoisPos(&L1, 4, 100);
    ImprimeLista(L1);
    */

    //RemoveElementoPos(&L1, 0);
    //RemoveElementoPos(&L1, 3);
    //RemoveElementoPos(&L1, 5);
    //ImprimeLista(L1);

    //RemoveNoPosicaoArray(&L1, REMOVE, 3);
    //ImprimeLista(L1);


    printf("\n\n");
    return(0);
}
