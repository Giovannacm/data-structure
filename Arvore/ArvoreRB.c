#include <stdio.h>
#include <stdlib.h>
#define MAX 20      //Pilha
#define RED 1
#define BLACK 0
//NÃO CONSEGUI FAZER
/*
1. Todo nó é vermelho ou preto
2. A raiz é preta
3. Toda folha (Nil) é preta
4. Se um nó é vermelho, então ambos os filhos são pretos
5. Para todo nó, todos os caminhos do nó até as folhas descendentes contêm o mesmo número de nós pretos
*/
struct no
{
    int info;
    struct no *esq;
    struct no *dir;
    int cor;
};
struct pilha
{
    struct no vet[MAX];
    int topo;
};
short push(struct pilha p, struct no *molde)
{
    struct no aux;
    aux.cor = molde->cor;
    aux.info = molde->info;
    p.vet[p.topo] = aux;
    return(1);
}
short pop(struct pilha p)
{
    p.topo--;
}
int cor(struct no *NO)          //Acessando a cor de um nó
{
    if(NO==NULL)
        return BLACK;
    return NO->cor;
}
void trocaCor(struct no *NO)    //Troca a cor do pai e de seus filhos
{
    NO->cor = !NO->cor;
    if(NO->esq != NULL)
        NO->esq->cor = !NO->esq->cor;
    if(NO->dir!=NULL)
        NO->dir->cor = !NO->dir->cor;
}
void RotacionaDir(struct no **arvore)
{
    struct no *aux;
    aux = (*arvore)->esq;
    (*arvore)->esq = aux->dir;
    aux->dir = *arvore;
    aux->cor = (*arvore)->cor;
    (*arvore)->cor = RED;
    *arvore = aux;
}
void RotacionaEsq(struct no **arvore)
{
    struct no *aux;
    aux = (*arvore)->dir;
    (*arvore)->dir = aux->esq;
    aux->esq = *arvore;
    aux->cor = (*arvore)->cor;
    (*arvore)->cor = RED;
    *arvore = aux;
}
void move2EsqRED(struct no **arvore)
{
    trocaCor(*arvore);
    if(cor((*arvore)->dir->esq) == RED)
    {
        RotacionaDir(&((*arvore)->dir));
        RotacionaEsq(&(*arvore));
        trocaCor(*arvore);
    }
}
void move2DirRED(struct no **arvore)
{
    trocaCor(*arvore);
    if(cor((*arvore)->esq->esq) == RED)
    {
        RotacionaDir(&(*arvore));
        trocaCor(*arvore);
    }
}
void balancear(struct no **arvore)
{
    if(cor((*arvore)->dir) == RED)  //Nó vermelho é sempre filho a esquerda
        RotacionaEsq(&(*arvore));
    if(((*arvore)->esq)!=NULL && cor((*arvore)->dir)==RED && cor((*arvore)->esq->esq)==RED) //Filho da direita e neto da esquerda são vermelho
        RotacionaDir(&(*arvore));
    if((cor((*arvore)->dir) == RED) && (cor((*arvore)->esq) == RED)) //2 filhos vermelhos
        trocaCor(*arvore);
}
short InsereNO(struct no **arvore, int elem)
{
    struct no *novo;
    novo = (struct no *)malloc(sizeof(struct no));
    if(novo==NULL)
        return(0);
    novo->info = elem;
    novo->dir = NULL;
    novo->esq = NULL;
    novo->cor = RED;
    *arvore = novo;
}
short InserirRec(struct no **arvore, int elem)
{
    struct pilha caminho;
    caminho.topo=0;
    struct no *aux = *arvore;
    while(aux != NULL)
    {
        push(caminho, aux);
        if(elem < (*arvore)->info)
            aux = (*arvore)->esq;
        else if(elem > (*arvore)->info)
            aux = (*arvore)->dir;
        else        //É igual, então não insere, pois não desejamos elementos repetidos
            return(0);
    }
    InsereNO(&aux, elem);
    push(caminho, aux);
    if(caminho.vet[caminho.topo-1].info==(*arvore)->info)  //CASO 1: raiz é PRETA - Se o nó inserido for a raiz da arvore
        caminho.vet[caminho.topo].cor = BLACK;
    else if(caminho.vet[caminho.topo-2].cor==RED)           //CASO 2: se um nó é vermelho, então seus filhos são pretos - Se o pai e o filho forem vermelho
    {
        if()    //CASO 3:   - Se o pai e o tio forem vermelho
        {

        }
        else    //CASO 4:   - Se o tio for preto
        {

        }
    }
}
void PreOrdem(struct no *arvore)
{
    if(arvore!=NULL)
    {
        printf("%d ", arvore->info);
        PreOrdem(arvore->esq);
        PreOrdem(arvore->dir);
    }
}
int main(void)
{
    struct no *a1=NULL;


    return(0);
}
