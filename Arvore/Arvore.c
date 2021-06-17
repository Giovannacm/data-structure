#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 30 //Para a pilha

struct no
{
    int info;
    struct no *esq, *dir;
};
struct arvore
{
    struct no *raiz;
};
void PreOrdem(struct no *arvore)
{
    if(arvore!=NULL)
    {
        printf("%d ", arvore->info);
        PreOrdem(arvore->esq);
        PreOrdem(arvore->dir);
    }
}
void InOrdem(struct no *arvore)
{
    if(arvore!=NULL)
    {
        InOrdem(arvore->esq);
        printf("%d ", arvore->info);
        InOrdem(arvore->dir);
    }
}
void PosOrdem(struct no *arvore)
{
    if(arvore!=NULL)
    {
        PosOrdem(arvore->esq);
        PosOrdem(arvore->dir);
        printf("%d ", arvore->info);
    }
}
void PreOrdemIt(struct no *arvore)      //Função não recursiva para percorrer uma árvore binária PreOrdem (uso de pilha)
{
    struct Pilha
    {
        int topo;
        struct no* item[MAX];
    } p;
    struct no *aux;
    p.topo=-1;
    aux=arvore;
    while(1)                //A pilha irá manter o caminho da raiz para o nó n, com a raiz na parte inferior e o nó n na parte superior da pilha
    {
        if(aux!=NULL)    //percorre os nós da esquerda o maximo possivel, salvando ponteios para nos passados
        {
            printf("%d ", aux->info);   //visita a raiz
            p.topo++;           //push
            p.item[p.topo]=aux; //push
            aux=aux->esq;
        }
        else            //Exploração do caminho na pilha agora está completa
        {
            if(p.topo==-1)
                return;
            aux=p.item[p.topo]; //pop
            p.topo--;           //pop
            aux=aux->dir;       //percorre subarvore dir
        }
    }
}
void InOrdemIt(struct no *arvore)       //Função não recursiva para percorrer uma árvore binária InOrdem (uso de pilha)
{
    struct Pilha
    {
        int topo;
        struct no* item[MAX];
    } p;
    struct no *aux;
    p.topo=-1;
    aux=arvore;
    while(1)
    {
        if(aux!=NULL)    //percorre os nós da esquerda o maximo possivel, salvando ponteios para nos passados
        {
            p.topo++;           //push
            p.item[p.topo]=aux; //push
            aux=aux->esq;
        }
        else      //verifica termino
        {
            //neste ponto a subarvore esq esta vazia
            if(p.topo==-1)
                return;
            aux=p.item[p.topo]; //pop
            p.topo--;           //pop
            printf("%d ", aux->info);   //visita a raiz
            aux=aux->dir;       //percorre subarvore dir
        }
    }
}
void PosOrdemIt(struct no *arvore)      //Função não recursiva para percorrer uma árvore binária PosOrdem (uso de pilha)    NÃO CONSEGUI!
{

}
void Destruir(struct no **arvore)
{
    if (*arvore!=NULL)
    {
        Destruir(&(*arvore)->esq);
        Destruir(&(*arvore)->dir);
        free(*arvore);
    }
    *arvore=NULL;
}
short Inserir(struct arvore *arv, int elem)
{
    struct no *aux=NULL, *novo=NULL;
    aux = arv->raiz;
    novo = (struct no *)malloc(sizeof(struct no));
    if(novo==NULL)
        return(0);
    novo->info=elem;
    novo->dir=NULL;
    novo->esq=NULL;
    if(aux==NULL)   //Insere na raiz
    {
        arv->raiz=novo;
        return(1);
    }
    while(aux!=NULL)
    {
        if(elem < aux->info)
        {
            if(aux->esq==NULL)  //Insere na esquerda do nó
            {
                aux->esq=novo;
                return(1);
            }
            aux=aux->esq;       //Percorre
        }
        else
        {
            if(aux->dir==NULL)  //Insere na direita do nó
            {
                aux->dir=novo;
                return(1);
            }
            aux=aux->dir;       //Percorre
        }
    }
}
short InserirRec(struct no **arvore, int elem)
{
    if(*arvore==NULL)   //Arvore vazia
    {
        struct no *novo;
        novo = (struct no *)malloc(sizeof(struct no));
        if(novo==NULL)
            return(0);
        novo->info = elem;
        novo->dir = NULL;
        novo->esq = NULL;
        *arvore = novo;
    }
    else
    {
        if(elem < (*arvore)->info)
            InserirRec(&(*arvore)->esq, elem);
        else if(elem > (*arvore)->info)
            InserirRec(&(*arvore)->dir, elem);
        else        //É igual, então não insere, pois não desejamos elementos repetidos
            return(0);
    }
}
struct no * Busca(struct no **arvore, int elem)
{
    struct no *aux=*arvore;
    while (aux != NULL)
    {
        if (aux->info == elem)
            return aux;
        else
        {
            if (elem > aux->info)
                aux = aux->dir;
            else
                aux = aux->esq;
        }
    }
    return aux;
}
short BuscarRec(struct no *arvore, int elem)
{
    if(arvore==NULL)   //Arvore vazia, não encontrou
        return(0);
    else
    {
        if(elem < arvore->info)
            BuscarRec(arvore->esq, elem);
        else if(elem > arvore->info)
            BuscarRec(arvore->dir, elem);
        else            //É igual
            return(1);
    }
}
struct no* Minimo(struct no *arvore)
{
    struct no *aux=NULL;
    for(aux=arvore ; aux->esq!=NULL ; aux=aux->esq);
    return(aux);
}
short RemoverRec(struct no **arvore, int elem)
{
    if(*arvore==NULL)   //Arvore vazia, não encontrou
        return(0);
    else
    {
        if(elem < (*arvore)->info)
            RemoverRec(&(*arvore)->esq, elem);
        else if(elem > (*arvore)->info)
            RemoverRec(&(*arvore)->dir, elem);
        else            //Encontrou o elemento, vamos analisar os casos
        {
            struct no *aux=NULL;
            if((*arvore)->esq!=NULL && (*arvore)->dir!=NULL) //Caso 3: o nó possui duas sub-árvores -> O nó contendo o menor valor da sub-árvore direita (ou o maior valor da sub-árvore esquerda) pode “ocupar” o lugar;
            {
                aux = Minimo((*arvore)->dir);                   //Ou Maximo((*arvore)->esq);
                (*arvore)->info=aux->info;
                RemoverRec(&(*arvore)->dir, (*arvore)->info);   //Ou RemoverRec(&(*arvore)->esq...
            }
            else        //Ou não tem sub-arvore, ou só tem uma (deve-se analisar qual lado)
            {
                aux = *arvore;
                if((*arvore)->esq==NULL)        //Caso 2: o nó possui uma sub-árvore (esq./dir.) -> O nó-raiz da sub-árvore (esq./dir.) “ocupa” o lugar do nó retirado;
                    *arvore = (*arvore)->dir;
                else
                    *arvore = (*arvore)->esq;
                free(aux);                      //Caso 1: o nó é folha -> O nó pode ser retirado sem problema;
            }
        }
    }
}
short RemoverRec2(struct no **arvore, int elem)     //->CADILAG (não remove o menor elemento)
{
    if(*arvore==NULL)   //Arvore vazia, não encontrou
        return(0);
    else
    {
        if(elem < (*arvore)->info)
            RemoverRec(&(*arvore)->esq, elem);
        else if(elem > (*arvore)->info)
            RemoverRec(&(*arvore)->dir, elem);
        else            //Encontrou o elemento, vamos analisar os casos
        {
            struct no *aux=*arvore;
            if((*arvore)->dir==NULL)
                *arvore = (*arvore)->esq;
            else if((*arvore)->esq==NULL)
                *arvore = (*arvore)->dir;
            else
            {
                struct no *min=(*arvore)->dir;
                while(min->esq!=NULL)
                    min=min->esq;
                (*arvore)->info=min->info;
                min=min->dir;
                aux=min;
            }
            free(aux);
        }
    }
}
int Nivel(struct no *arvore, int elem)              //NÃO DA CERTO QUANDO O ELEMENTO NÃO EXISTE
{
    //O nível de um nó T é definido como: O nível de um nó raiz é 0; O nível de um nó não raiz é dado por (nível de seu nó PAI + 1).
    if(arvore==NULL)   //Arvore vazia, não encontrou
        return(-1);
    else
    {
        if(elem < arvore->info)
            return(Nivel(arvore->esq, elem)+1);
        else if(elem > arvore->info)
            return(Nivel(arvore->dir, elem)+1);
        else          //É igual
            return(0);
    }
}
int Grau(struct no *arvore, int elem)
{
    //O grau de um nó T de uma árvore é igual ao número de filhos do nó T. E o grau da arvore é o grau maximo entre os graus de seus nós
    if(arvore==NULL)   //Arvore vazia, não encontrou
        return(-1);
    else
    {
        if(elem < arvore->info)
            Grau(arvore->esq, elem);
        else if(elem > arvore->info)
            Grau(arvore->dir, elem);
        else            //É igual
        {
            if(arvore->esq==NULL && arvore->dir==NULL)
                return 0;       //Nenhum filho
            if(arvore->esq != NULL && arvore->dir != NULL)
                return 2;       //Dois filhos
            return 1;           //Um filho
        }
    }
}
int Altura(struct no *arvore)
{   /*É a distância entre um nó e o seu descendente mais afastado. Mas precisamente, a altura de v é o número de passos do mais
    longo caminho que leva de v até uma folha. Os nós folha sempre têm altura igual a 0;
    A altura de uma árvore T é dada pela altura da raiz da árvore*/
    if (arvore == NULL)
        return -1;
    int he = Altura(arvore->esq);
    int hd = Altura(arvore->dir);
    if (he > hd)
        return he+1;
    return hd+1;
}
short Balanceada(struct no *arvore)
{
    //Uma árvore balanceada é aquela onde para cada nó, as alturas de suas duas sub-árvores diferem de, no máximo, 1.
    //Fator de Balanceamento (FB) de um nó é a altura da subárvore direita do nó menos a altura da subárvore esquerda do nó
    short aux = 1;  //Variável auxiliar para ver se as sub-arvores são balanceadas, se não for, "para" as chamadas recursivas
    if(arvore!=NULL)
    {
        aux = Balanceada(arvore->esq);
        if(aux)
            aux = Balanceada(arvore->dir);
        if(aux)
        {
            int he = Altura(arvore->esq);
            int hd = Altura(arvore->dir);
            if(abs(he-hd)<=1)
                aux=1;
            else
                aux=0;
        }
    }
    return(aux);
}
int ContaNo(struct no *arvore)
{
    if (arvore == NULL)
        return 0;
    int ne = ContaNo(arvore->esq);
    int nd = ContaNo(arvore->dir);
    return (ne+nd+ 1);
}
short PerfBalanceada(struct no *arvore)
{
    //O número de nós de suas sub-árvores esquerda e direita difere em, no máximo, 1.
    short aux = 1;  //Variável auxiliar para ver se as sub-arvores são perfeitamente balanceadas, se não for, "para" as chamadas recursivas
    if(arvore!=NULL)
    {
        aux = PerfBalanceada(arvore->esq);
        if(aux)
            aux = PerfBalanceada(arvore->dir);
        if(aux)
        {
            int he = ContaNo(arvore->esq);
            int hd = ContaNo(arvore->dir);
            if(abs(he-hd)<=1)
                aux=1;
            else
                aux=0;
        }
    }
    return(aux);
}
short EstritBin(struct no *arvore)
{
    //Se todo nó que não é folha numa árvore binária tiver sub-árvores esquerda e direita não vazias.
    if(arvore->dir==NULL && arvore->esq==NULL)   //Não tem sub-arvores esq e dir
        return 1;
    if(arvore->dir!=NULL && arvore->esq!=NULL)   //Tem sub-arvores esq e dir
        return(EstritBin(arvore->esq) && EstritBin(arvore->dir));
    return 0;
}


int contaPais(struct no *arvore)
{   //Internet
    //Função recursiva que conta o número de nós em uma árvore binária que possuem pelo menos um filho.
    if (arvore == NULL || (arvore->esq == NULL && arvore->dir == NULL))
        return 0;
    return (1 + contaPais(arvore->esq) + contaPais(arvore->dir));
}
void imprimeMaior(struct no *arvore, int valor)
{   //Exercicio 9 - Lista de exercicio   (ARVORE BINARIA) Iimprimir somente os valores que sejam maiores que o parâmetro valor.
    if(arvore!=NULL)
    {
        imprimeMaior(arvore->esq, valor);
        if(arvore->info>valor)
        {
            printf("%d ", arvore->info);
        }
        imprimeMaior(arvore->dir, valor);
    }
}
void imprimeMaiorABB(struct no *arvore, int valor)
{   //Exercicio 9 - Lista de exercicio   (ARVORE BINARIA DE BUSCA) Iimprimir somente os valores que sejam maiores que o parâmetro valor.
    if(arvore!=NULL)
    {
        if(arvore->info>valor)
        {
            imprimeMaiorABB(arvore->esq, valor);
            printf("%d ", arvore->info);
        }
        imprimeMaiorABB(arvore->dir, valor);
    }
}
void PorNivel(struct no *arvore)
{   //Exercicio 11 - Lista de exercicio   ATENÇÃO: a fila foi implementada de forma estática, porém fiz alguns ajustes para não precisar fazer funções
    struct fila
    {
        struct no* item[MAX];
        int cont, ini;
    }f;
    struct no *aux=NULL;
    f.cont=0;
    f.ini=0;
    f.item[f.cont]=arvore;
    f.cont++;
    while(f.cont!=f.ini)
    {
        aux = f.item[f.ini];
        f.ini++;
        printf("%d ", aux->info);
        if(aux->esq!=NULL)
        {
            f.item[f.cont]=aux->esq;
            f.cont++;
        }
        if(aux->dir!=NULL)
        {
            f.item[f.cont]=aux->dir;
            f.cont++;
        }
    }
}
void novaArvoreEspelhada(struct no *arvore, struct no **arvEsp)
{   //Exercicio 14 - Lista de exercicio   Retorna a imagem espelhada em uma nova árvore
    if(arvore!=NULL)
    {
        struct no *aux = arvore;
        InserirRec(arvEsp, arvore->info);
        novaArvoreEspelhada(arvore->esq, &((*arvEsp)->dir));
        novaArvoreEspelhada(arvore->dir, &((*arvEsp)->esq));
    }
}
void arvoreEspelhada(struct no **arvore)
{   //Exercicio 14 - Lista de exercicio   Modifica a mesma árvore e retorna como sua própria imagem espelhada
    if(*arvore!=NULL)
    {
        struct no *aux = (*arvore)->esq;
        (*arvore)->esq = (*arvore)->dir;
        (*arvore)->dir = aux;
        arvoreEspelhada(&((*arvore)->esq));
        arvoreEspelhada(&((*arvore)->dir));
    }
}
short arvoresSemelhantes(struct no *arv1, struct no *arv2)
{   //Exercicio 23 - Lista de exercicios  Implemente uma função para determinar se duas árvores binárias são semelhantes.
    return(arv1==NULL&&arv2==NULL||
           (arv1!=NULL&&arv2!=NULL&&(arv1->info==arv2->info)&&arvoresSemelhantes(arv1->esq, arv2->esq)&&arvoresSemelhantes(arv1->dir, arv2->dir)));
}
void ordemDecresc(struct no *arvore)
{   //Exercicio 26 - Lista de exercicio   Apresente todos os elementos de uma árvore em ordem decrescente
    if(arvore!=NULL)
    {
        ordemDecresc(arvore->dir);
        printf("%d ", arvore->info);
        ordemDecresc(arvore->esq);
    }
}
int contaNumPares(struct no *arvore)
{   //Exercicio 27 - Lista de exercicio   Retorne a quantidade de números pares
    int cont = 0;
    if(arvore!=NULL)
    {
        if(arvore->info%2==0)
            cont++;
        cont += contaNumPares(arvore->esq);
        cont += contaNumPares(arvore->dir);
    }
    return(cont);
}
void imprimeIntervalo(struct no *arvore, int a, int b)
{   //Exercicio 32 - Lista de exercicio   Dados dois inteiros a e b, eliminar todos os nós n da árvore tais que a <= n <= b
    if(arvore!=NULL)
    {
        if(arvore->info>=a)
        {
            imprimeIntervalo(arvore->esq, a, b);
            if(arvore->info<=b)
                printf("%d ", arvore->info);
            imprimeIntervalo(arvore->dir, a, b);
        }
        else
            imprimeIntervalo(arvore->dir, a, b);
    }
}
void ancestralComum(struct no *arvore)
{   //Exercicio 32 - Lista de exercicio   Dados dois inteiros a e b, verificar se eles estão na árvore e, caso estejam, retornar o primeiro ancestral comum a eles


}


int main (void)
{
    struct no *arvEsp = NULL;
    struct no *a1 = NULL;
    InserirRec(&a1, 10);
    InserirRec(&a1, 4);
    InserirRec(&a1, 2);
    InserirRec(&a1, 6);
    InserirRec(&a1, 3);
    InserirRec(&a1, 12);
    InserirRec(&a1, 11);
    InserirRec(&a1, 20);
    InserirRec(&a1, 1);
    InserirRec(&a1, -7);
    printf("\n  Pre-Ordem: ");
    PreOrdem(a1);
    //printf("\n");
    //PreOrdemIt(a1);
    printf("\n  In-Ordem: ");
    InOrdem(a1);
    //printf("\n");
    //InOrdemIt(a1);
    printf("\n  Pos-Ordem: ");
    PosOrdem(a1);
    //printf("\n");
    //PosOrdemIt(a1);
    //printf("\n  Por-Nivel: ");
    //PorNivel(a1);
    //printf("\n  Ordem descrescente: ");
    //ordemDecresc(a1);

    printf("\n  Imprimir valor maior: ");
    imprimeMaior(a1, 11);
    printf("\n  Imprimir valor maior: ");
    imprimeMaiorABB(a1, 11);
    printf("\n  Remover valor entre um intervalo: ");
    imprimeIntervalo(a1, 1, 13);
    //PreOrdem(a1);

    /*novaArvoreEspelhada(a1, &arvEsp);
    printf("\n  Pre-Ordem: ");
    PreOrdem(arvEsp);
    if(arvoresSemelhantes(a1, arvEsp))
        printf("\n  SEMELHANTES!!!");
    else
        printf("\n  NAO SEMELHANTES!!!");
    arvoreEspelhada(&a1);
    printf("\n  Pre-Ordem: ");
    PreOrdem(a1);
    if(arvoresSemelhantes(a1, arvEsp))
        printf("\n  SEMELHANTES!!!");
    else
        printf("\n  NAO SEMELHANTES!!!");*/

    //printf("\n  Numeros pares: %d", contaNumPares(a1));
    /*if(BuscarRec(a1, 40))
        printf("\n  Encontrei");
    else
        printf("\n  Nao encontrei");*/
    //RemoverRec2(&a1, 10);
    //printf("\n  Pre-Ordem: ");
    //PreOrdem(&a1);
    //printf("\n  Altura: %d", Altura(a1));
    //printf("\n  Nivel: %d", Nivel(a1,1));
    //printf("\n  Grau: %d", Grau(a1,5));
    /*if(Balanceada(a1))
        printf("\n  balanceada");
    else
        printf("\n  Nao balanceada");*/
    /*if(PerfBalanceada(a1))
        printf("\n  perf balanceada");
    else
        printf("\n  Nao perf balanceada");*/
    /*if(EstritBin(a1))
        printf("\n  estrit binaria");
    else
        printf("\n  Nao estrit binaria");*/

    /*struct arvore a2;
    a2.raiz=NULL;
    Inserir(&a2, 10);
    Inserir(&a2, 4);
    Inserir(&a2, 2);
    Inserir(&a2, 6);
    Inserir(&a2, 3);
    Inserir(&a2, 12);
    Inserir(&a2, 11);
    Inserir(&a2, 20);
    printf("\n\n  Pre-Ordem: ");
    PreOrdem(a2.raiz);
    printf("\n  In-Ordem: ");
    InOrdem(a2.raiz);
    printf("\n  Pos-Ordem: ");
    PosOrdem(a2.raiz);
    if(BuscarRec(a2.raiz, 1))
        printf("\n  Encontrei");
    else
        printf("\n  Nao encontrei");
    RemoverRec(&a2.raiz, 10);
    printf("\n  Pre-Ordem: ");
    PreOrdem(&a2.raiz);*/

    return(0);
}
