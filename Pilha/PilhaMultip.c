#include <stdio.h>
#define MAX 12
#define NPilha 4

struct PilhaMultp
{
    int P[MAX];
    int base[NPilha];
    int topo[NPilha];
    int NElemP;     //Numero de elementos por pilha
};
void InicializaPilhas(struct PilhaMultp *PM)         //Inicializando todas as pilhas, ou seja, dando valor à base e topo
{
    int i;
    PM->NElemP = MAX/NPilha;
    for(i=0 ; i<NPilha ; i++)
    {
        PM->base[i] = PM->NElemP*i;
        PM->topo[i] = PM->base[i];
    }
    for(i=0 ; i<MAX ; i++)
        PM->P[i]=0;
}
short PilhaXCheia(struct PilhaMultp PM, int PilhaX)  //Verifica se a pilha X está cheia
{
    if((PM.topo[PilhaX]-PM.base[PilhaX])<PM.NElemP)
        return(0);
    else
        return(1);
}
void ImprimePilhaMultp(struct PilhaMultp PM)
{
    int i;
    printf("      ");
    for(i=0 ; i<MAX ; i++)
        printf("  %d", PM.P[i]);
    for(i=0 ; i<NPilha ; i++)
    {
        printf("\n      BASE[%d]:%2.d", i+1, PM.base[i]+1);
        printf("      TOPO[%d]:%2.d", i+1, PM.topo[i]+1);
    }
}
short Insere(struct PilhaMultp *PM, int PilhaX, int Valor)  //Insere na Pilha X (que está na pilha PM) o valor Valor
{
    if(!PilhaXCheia(*PM, PilhaX))
    {
        PM->P[PM->topo[PilhaX]] = Valor;
        PM->topo[PilhaX]++;
    }
    else        //Pilha cheia
        return(0);
}
void MovePEsquerda(struct PilhaMultp *PM, int PilhaX) //Move a PilhaX 1 posição para a Esquerda
{
    int i;
    for(i=PM->base[PilhaX]-1 ; i<PM->topo[PilhaX]-1 ; i++)
        PM->P[i]=PM->P[i+1];
    PM->P[i]=0;
    PM->topo[PilhaX]--;
    PM->base[PilhaX]--;
}
void MovePDireita(struct PilhaMultp *PM, int PilhaX) //Move a PilhaX 1 posição para a Direita
{
    int i;
    for(i=PM->topo[PilhaX] ; i>PM->base[PilhaX] ; i--)
        PM->P[i]=PM->P[i-1];
    PM->P[i]=0;
    PM->topo[PilhaX]++;
    PM->base[PilhaX]++;
}
short Insere2(struct PilhaMultp *PM, int PilhaX, int Valor) //Se não for possível inserir na pilha, verificamos se as outras possuem espaço, se sim, serão deslocadas
{
    int i, AUX;
    if(!PilhaXCheia(*PM, PilhaX))       //Pilha ainda está vazia, então insere normalmente
    {
        PM->P[PM->topo[PilhaX]] = Valor;
        PM->topo[PilhaX]++;
    }
    else                                //Pilha cheia
    {
        for(i=0 ; i<NPilha ; i++)       //Busca por outras pilhas (antes e depois de PilhaX) que estão vazias
        {
            if(!PilhaXCheia(*PM, i))    //Achei uma pilha (i) que nao esta cheia
            {
                AUX=i;
                if(i<PilhaX)            //Verifico se ela está à direita (else) ou à esquerda (if) de PilhaX
                {
                    while(i<PilhaX) //Vou deslocando as pilhas entre i e PilhaX uma posição para trás (já que há uma pilha com espaço vazio (i) à esquerda
                    {
                        i++;        //Incrementa antes pois não quero deslocar a pilha i, e sim as posições (i, PilhaX]
                        MovePEsquerda(PM, i);
                    }
                }
                else
                {
                    while(i>PilhaX) // Vou deslocando as pilhas entre i e PilhaX uma posição para frente (já que há uma pilha com espaço vazio (i) à direita,
                    {
                        MovePDireita(PM, i);
                        i--;        //Decrementa depois pois quero deslocar a pilha i (PilhaX, i] para ir disponibilizando o espaço até PilhaX (que não é deslocada)
                    }
                }
                PM->P[PM->topo[PilhaX]] = Valor;//Agora que tenho espaço, adiciono o valor desejado na PilhaX.
                PM->topo[PilhaX]++;
                printf("\n\n        [ATENCAO] PILHA %d TEVE SEU TAMANHO MODIFICADO!!", AUX+1);
                break;                          //Saindo do loop já que consegui realizar a operação
            }
        }
        if(i==NPilha)
            printf("\n\n        [ERRO] NAO HA ESPACO SUFICIENTE!!");
    }
}
short Remove(struct PilhaMultp *PM, int PilhaX)
{
    if(PM->topo[PilhaX] > PM->base[PilhaX])     //Há elementos na pilha > retira-se o valor PM->P[PM->topo[PilhaX]];
    {
        PM->topo[PilhaX]--;
    }
    else    //Não há elementos na pilha ou o topo da pilha X esta na base da pilha X+1;
        return(0);
}
int main (void)
{
    int i, PilhaX, Valor, Quantidade;
    struct PilhaMultp P1;
    InicializaPilhas(&P1);

    //---------Inserindo elementos na pilha------//
    printf("\n      QUANTOS VALORES  DESEJA INSERIR NA PILHA? [1, %d]: ", MAX);
    scanf("%d", &Quantidade);
    for(i=0 ; i<Quantidade ; i++)
    {
        printf("\n      PILHA [1,%d]: ", NPilha);
        scanf("%d", &PilhaX);
        printf("      VALOR: ");
        scanf("%d", &Valor);
        Insere2(&P1, PilhaX-1, Valor);  //PilhaX-1 pois [0,4)
    }

    printf("\n      RESULTADO FINAL: \n");
    ImprimePilhaMultp(P1);
    return(0);
}
/*if(!PilhaXCheia(*PM, i))    //Achei uma pilha (i) vazia
{
    printf("\n      MOVENDO PILHA %d!!", i+1);
    if(i<PilhaX)            //Verifico se ela está à direita (else) ou à esquerda (if) de PilhaX
        MovePEsquerda(PM, PilhaX);  //Se há uma pilha (i) vazia à esquerda, move a PilhaX uma posição para trás
    else
        MovePDireita(PM, i);        //Se há uma pilha (i) vazia á direita, move a pilha i uma posição para frente.
    PM->P[PM->topo[PilhaX]] = Valor;//Agora que tenho espaço, adiciono o valor desejado na PilhaX.
    PM->topo[PilhaX]++;
    break;                          //Saindo do loop já que consegui realizar a operação
}*/
