#include <stdio.h>
#include <stdlib.h>
struct node{
    int duration, time, type;   //Duration=periodo de dura��o da transa��o/do cliente de chegada no n� da fila evlist   Time=hora de chegada do cliente
    struct node *next;          //Type=inteiro de -1 a 3, onde -1 mostra um evento de chegada, e 1/3, de saida no caixa 1/3;
};
typedef struct node *NODEPTR;
struct queue{
    NODEPTR front, rear;
    int num;
};
struct queue q[4];              //Vetor que representa os caixas
struct node auxinfo;            //Armazena temporariamente a parte de informa��es de um n�
NODEPTR evlist;                 //Aponta para o inicio da FILA DE EVENTOS; fila de prioridade ascendente; onde o primero n� armazena o cliente que chega (n� de chegada), e o restante o primeiro cliente de cada caixa (que vai sair) (n�s de partida)
int atime, dtime, dur, qindx;
float count, tottime;           //tottime=conta o tempo total gasto por todos os clientes, count=contagem do n�mero de clientes que passaram pelo banco

void insert(struct queue *pq, struct node *auxinfo)
{
    NODEPTR p;
    p = (struct node *)malloc(sizeof(struct node));
    p->duration=auxinfo->duration;
    p->time=auxinfo->time;
    p->type=auxinfo->type;
    p->next=NULL;
    if(pq->front==NULL)
        pq->front=p;
    else
        pq->rear->next=p;
    pq->rear=p;
    pq->num++;
}
void removen(struct queue *pq)
{
    NODEPTR p;
    if(pq->front==NULL)
    {
        printf("\n ERRO DE REMOCAO!");
        exit(1);
    }
    p=pq->front;
    pq->front=p->next;
    pq->num--;
    free(p);
}
void place(NODEPTR *plist, struct node *auxinfo)           //Insercao ordenada em uma lista
{
    NODEPTR p, q, aux;
    aux = (struct node *)malloc(sizeof(struct node));
    aux->duration=auxinfo->duration;
    aux->time=auxinfo->time;
    aux->type=auxinfo->type;
    aux->next=NULL;
    q=NULL;
    for(p=*plist ; p!=NULL && auxinfo->type>p->type ; p=p->next)
        q=p;
    if(q==NULL) //Insere no inicio
    {
        aux->next=*plist->next;
        *plist=aux;
    }
    else
    {
        aux->next=q->next;
        q->next=aux;
    }

}
void arrive(int atime, int dur) //Modifica a lista de eventos e as filas de modo a refletir uma nova chegada na hora atime com uma transa�ao de dura��o dur.
{                               //Ela realiza a inser��o de um novo cliente (n�) no final da fila mais curta ao chamar a funcao insert.
    int i, j, small;
    //Localizando a fila mais curta
    j=0;
    small=q[0].num;
    for(i=1 ; i<4 ; i++)
    {
        if(q[i].num < small)
        {
            small=q[i].num;
            j=i;
        }
    }
    auxinfo.time=atime;
    auxinfo.duration=dur;
    auxinfo.type=j;
    insert(&q[j], &auxinfo);
    if(q[j].num == 1)           //Se este n� for o unico n� do caixa (da fila), o n� de partida do cliente deve ser colocado na lista de eventos por meio da funcao place
    {
        auxinfo.time=atime+dur;
        place(&evlist, &auxinfo);
    }
    //O proximo par de dados (se existir) ser� lido e um n� de partida ser� colocado na lista de eventos, de modo a substituir o que acabou de ser processado
    //Se n�o tiver mais entrada, a funcao retornar� sem incluir um novo n� de chegada e o programa processar� os n�s (de partida) restantes na lista de eventos
    printf("\n  DIGITE HORARIO: \n");
    if(scanf("%d", &auxinfo.time) != EOF)       //A fun��o scanf retorna o n�mero de elementos lidos com sucesso
    {
        printf("\n  DIGITE DURACAO: \n");
        scanf("%d", &auxinfo.duration);
        auxinfo.type=-1;
        place(&evlist, &auxinfo);
    }
}
void depart(int qindx, int dtime)   //Modifica a fila q[qindx] e a lista de eventos de modo a refletir a partida do primeiro cliente da fila na hora dtime
{                                   //O cliente da fila � removido a partir da chamada da funcao removen; O n� de partida do pr�ximo cliente na fila (se existir) substitui o n� de partida que acabou de ser removido da lista de eventos
    NODEPTR p;
    removen(&q[qindx]);
    tottime=tottime+(dtime-auxinfo.time);
    count++;
    //Se existir mais um cliente na fila, coloca a parti do proximo cliente na lista de eventos depois de calcular sua hora de partida
    if(q[qindx].num > 0)
    {
        p=q[qindx].front;
        auxinfo.time=dtime+p->duration;
        auxinfo.type=qindx;
        place(&evlist, &auxinfo);
    }
}
void popsub(&evlist, &auxinfo)
{

}
int main(void)
{
    //Inicializando os elementos da fila e lista;
    evlist=NULL;
    count=0;
    tottime=0;
    for(qindx=0 ; qindx<4 ; qindx++)
    {
        q[qindx].num=0;
        q[qindx].front=NULL;
        q[qindx].rear=NULL;
    }
    printf("\n Digite horario e duracao: \n");  //Inicializando a lista de eventos com a primeira chegada
    scanf("%d %d", &auxinfo.time, &auxinfo.duration);
    auxinfo.type=-1;
    place(&evlist, &auxinfo);           //Insere o n� com as informacoes contidas em auxinfo na posicao certa na lista de eventos
    while(evlist != NULL)               //Vai fazer a simulacao enquanto a lista de eventos nao estiver vazia
    {
        popsub(&evlist, &auxinfo);      //Remove o primeiro n� da lista de eventos e coloca a informacao desse n� em  auxinfo
        if(auxinfo.type==-1)            //Verifica se o proximo evento � uma chegada (if) ou partida (else)
        {
            atime=auxinfo.time;
            dur=auxinfo.duration;
            arrive(atime, dur);         //Chega um cliente na hora atime com uma transacao de duracao dur
        }
        else
        {
            qindx=auxinfo.type;
            dtime=auxinfo.time;
            depart(qindx, dtime);       //Sai o primeiro cliente da fila q[qindx] na hora dtime
        }
    }
    printf("\n  TEMPO MEDIO EH: %4.2f", tottime/count);
}
