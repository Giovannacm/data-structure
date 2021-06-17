#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <limits.h>

#define INFINITO INT_MAX/2          //Ser� usada para representar as constantes infinito do algoritmo

FILE *arqLog;                       //Arquivo de log (est� global para todas as fun��es terem acesso � ele)
char nomeArqGrafo[50];              //Nome do arquivo do grafo (est� global pois ser� necess�rio imprimir o nome do grafo em todas as tabelas)

struct grafo                        //Struct que representa o grafo
{
    int tipo;                       //0: grafo (n�o orientado), 1: digrafo (orientado)
    int maxV;                       //Quantidade de v�rtices
    int *matrizAdj;                 //A matriz ser� alocada dinamicamente ap�s o valor de maxV ser determinado
};
void inicializar(struct grafo *g)   //Aloca a matriz dinamicamente ap�s saber o valor de maxV
{
    int i, j;
    g->matrizAdj = (int *)calloc((g->maxV)*(g->maxV), sizeof(int)); //calloc j� coloca 0 nas posi��es
}
void imprimir(struct grafo g)       //Imprime o grafo na implementa��o de matriz de adjacencia
{
    int i, j;
    printf("\n  [TRIPO DO GRAFO]: %s", (g.tipo) ? "D�grafo" : "Grafo nao orientado");
    printf("\n  [V�RTICES]: %d", g.maxV);
    printf("\n  [MATRIZ]:\n       ");
    for(i=0 ; i<g.maxV ; i++)
        printf("%d  ", i);
    printf("\n");
    for(i=0 ; i<g.maxV ; i++)
    {
        printf("\n  %d  ", i);
        for(j=0 ; j<g.maxV ; j++)
        {
            printf("  %d", *(g.matrizAdj +i*g.maxV + j));
        }
    }
}
void copiaMatriz(struct grafo g, struct grafo *gNovo)                       //Fun��o que copia a matriz de adjac�ncia de g em gNovo
{
    int i, j;
    for(i=0 ; i<gNovo->maxV ; i++)
    {
        for(j=0 ; j<gNovo->maxV ; j++)
            *(gNovo->matrizAdj +i*gNovo->maxV + j) = *(g.matrizAdj +i*g.maxV + j);
    }
}
short adicionarAresta(struct grafo *g, int ini, int fim, int peso)          //Fun��o para adicionar uma aresta (ini, fim) com peso
{
    fprintf(arqLog, "\n [adicionarAresta] Adicionando a aresta %d -> %d de peso %d no grafo.", ini, fim, peso);
    if(*(g->matrizAdj +ini*g->maxV + fim)!=0)   //Aresta j� existe
    {
        fprintf(arqLog, "\n   [adicionarAresta] ERRO: aresta %d -> %d de peso %d j� existe no grafo.", ini, fim, peso);
        return(0);
    }
    *(g->matrizAdj +ini*g->maxV + fim) += peso; //Adicionando o peso na posi��o correspondentr da matriz [ini][fim]
    if(!g->tipo)    //Grafo (n�o orientado)
    {
        if(ini!=fim)
            *(g->matrizAdj +fim*g->maxV + ini) += peso; //Adicionando o peso na posi��o [fim][ini] (pois a matriz � sim�trica)
    }
    return(1);
}
short removerAresta(struct grafo *g, int ini, int fim)                      //Fun��o para remover uma aresta (ini, fim) do grafo
{
    fprintf(arqLog, "\n [removerAresta] Removendo a aresta %d -> %d do grafo.", ini, fim);
    if(*(g->matrizAdj +ini*g->maxV + fim)==0)   //Aresta n�o existe
    {
        fprintf(arqLog, "\n   [removerAresta] ERRO: aresta %d -> %d n�o existe no grafo.", ini, fim);
        return(0);
    }
    *(g->matrizAdj +ini*g->maxV + fim) = 0; //Coloca 0 na posi��o [ini][fim] (valor de inicializa��o)
    if(!g->tipo)    //Grafo (n�o orientado)
    {
        if(ini!=fim)
            *(g->matrizAdj +fim*g->maxV + ini) = 0; //Coloca 0 na posi��o [fim][ini] (pois a matriz � sim�trica)
    }
    return(1);
}
void adicionarVertice(struct grafo g, struct grafo *gNovo, int quantidade)  //Fun��o para adicionar v�rtices no grafo gNovo
{
    fprintf(arqLog, "\n [adicionarVertice] Adicionando o vertice %d no grafo.", g.maxV-1);
    gNovo->tipo = g.tipo;               //Copiando o tipo do grafo
    gNovo->maxV = g.maxV + quantidade;  //Mudando o valor de maxV de acordo com a quantidade de vertices a sererm adicionados
    inicializar(gNovo);     //A matriz de adjacencia � alocada novamente (j� que o valor de maxV mudou)
    copiaMatriz(g, gNovo);  //O grafo g (ou seja, a c�pia do grafo original) possui as arestas anteriores que ser�o passadas novamente para gNovo
}
short removerVertice(struct grafo g, struct grafo *gNovo, int quantidade)   //Fun��o para remover v�rtices do grafo gNovo
{
    fprintf(arqLog, "\n [removerVertice] Removendo o vertice %d do grafo.", g.maxV);
    if(g.maxV==0)   //Grafo sem v�rtices
    {
        fprintf(arqLog, "\n   [removerVertice] ERRO Grafo vazio.");
        return(0);
    }
    gNovo->tipo = g.tipo;               //Copiando o tipo do grafo
    gNovo->maxV = g.maxV - quantidade;  //Mudando o valor de maxV de acordo com a quantidade de vertices a serem removidos
    inicializar(gNovo);    //A matriz de adjacencia � alocada novamente (j� que o valor de maxV mudou)
    copiaMatriz(g, gNovo); //O grafo g (ou seja, a c�pia do grafo original) possui as arestas anteriores que ser�o passadas novamente para gNovo
    return(1);
}
void transformaTextoGrafo(FILE *arq, struct grafo *g)       //Texto -> Matriz de adjacencia. Considera um arquivo sem coment�rios
{
    fprintf(arqLog, "\n [transformaTextoGrafo] Transformando o arquivo em matriz de adjacencia");
    int i, ini, fim, peso;
    fscanf(arq, "%d", &g->tipo);    //Leitura e armazenamento do tipo do grafo
    fprintf(arqLog, "\n   [transformaTextoGrafo] Tipo do grafo: %s", (g->tipo) ? "D�grafo" : "Grafo nao orientado");
    fscanf(arq, "%d", &g->maxV);    //Leitura e armazenamento da quantidade de v�rtices
    fprintf(arqLog, "\n   [transformaTextoGrafo] Numero de vertices: %d", g->maxV);
    inicializar(g);                 //Alocando a matriz de adjacencia dinamicamente
    for(i=0 ; !feof(arq) ; i++)     //Leitura das arestas at� o fim do arquivo
    {
        fscanf(arq, "%d", &ini);    //Leitura do vertice inicial
        fscanf(arq, "%d", &fim);    //Leitura do vertice final
        fscanf(arq, "%d", &peso);   //Leitura do peso
        fprintf(arqLog, "\n   [transformaTextoGrafo] Leitura da aresta: %d -> %d", ini, fim);
        adicionarAresta(g, ini, fim, peso); //Inser��o da aresta
    }
}
void transformaGrafoTexto(FILE *arq, struct grafo g)        //Matriz de adjacencia -> Texto
{
    fprintf(arqLog, "\n [transformaGrafoTexto] Transformando a matriz de adjacencia em um arquivo");
    int i, j;
    fprintf(arq, "%d", g.tipo);     //Escrevendo o tipo do grafo
    fprintf(arqLog, "\n   [transformaGrafoTexto] Tipo do grafo: %s", (g.tipo) ? "D�grafo" : "Grafo nao orientado");
    fprintf(arq, "\n%d", g.maxV);   //Escrevendo a quantidade de v�rtices do grafo
    fprintf(arqLog, "\n   [transformaGrafoTexto] Numero de vertices: %d", g.maxV);
    for (i=0 ; i<g.maxV ; i++ )     //Escrevendo as arestas a partir da leitura da matriz de adjacencia
    {
        if(!g.tipo)                 //Se for um grafo n�o orientado, basta percorrer acima da diagonal principal (a matriz � sim�trica)
            j=i;
        else
            j=0;                    //Se for um d�grafo, percorre a matriz normalmente
        for(  ; j<g.maxV ; j++ )
        {
            if(*(g.matrizAdj +i*g.maxV + j)!=0) //Existe aresta, logo escreve no arquivo
            {
                fprintf(arq, "\n%d %d %d", i, j, *(g.matrizAdj +i*g.maxV + j)); //Escrevendo a aresta i->j com peso
                fprintf(arqLog, "\n   [transformaGrafoTexto] Escrita da aresta: %d -> %d", i, j);
            }
        }
    }
}

void inicializa(struct grafo *g, int s, int *d, int *p)     //Fun��o de inicializa��o para o caminho
{
    int i;
    for(i=0 ; i<g->maxV ; i++)  //Inicializado os vetores d e p com infinito e -1, respectivamente
    {
        d[i] = INFINITO;
        p[i] = -1;
    }
    d[s] = 0;   //Colocando 0 na distancia do vertice inicial do caminho
}
void relaxa(int u, int v, int w, int *d, int *p)            //Fun��o relaxa para o caminho
{
    if(d[u]!=INFINITO || d[v]!=INFINITO)    //Se os dois valores a serem comparados forem infinito, sabemos que infinito n�o � maior que infinito, logo, n�o � feita a compara��o
    {
        if(d[v] > (d[u]+w))
        {
            fprintf(arqLog, "\n   [relaxa] (%d, %d, %d)", u, v, w);
            d[v] = d[u]+w;
            p[v] = u;
        }
    }
}
int minimo(char *Q, int *d, int n)  //Fun��o que retorna o v�rtice que possui menor distancia (d) e ainda est� marcado com X (em Q)
{
    int i, min=INFINITO, pos=0;
    for(i=0 ; i<n ; i++)        //Encontrando o menor valor de d que ainda possui o valor X no vetor Q
    {
        if(Q[i]=='X' && d[i]<min)
        {
            min = d[i];         //Vari�vel min recebe a menor distancia
            pos = i;            //Armazenando a posi��o para ser retornada
        }
    }
    fprintf(arqLog, "\n   [minimo] Minimo(Q): %d", pos);
    return(pos);
}
short vazio(char *Q, int n)         //Fun��o para verificar se o vetor Q est� vazio (ou seja, sem X)
{
    int i;
    for(i=0 ; i<n ; i++)
    {
        if(Q[i]=='X')       //Ainda tem X
            return(0);
    }
    return(1);
}
void caminhoDijkstra(FILE *tabela, struct grafo *g, int s)  //Fun��o principal para o caminho Dijkstra
{
    fprintf(arqLog, "\n [caminhoDijkstra] Caminho Dijkstra a partir do v�rtice: %d", s);
    int i, u, v, d[g->maxV], p[g->maxV];
    char S[g->maxV], Q[g->maxV];
    inicializa(g, s, d, p);         //Inicializando os vetores d e p
    for(i=0 ; i<g->maxV ; i++)      //Inicializando os vetores S e Q
    {
        S[i] = '-';
        Q[i] = 'X';
    }
    while(!vazio(Q, g->maxV))       //Enquanto n�o tiver nenhum elemento com X em Q
    {
        u = minimo(Q, d, g->maxV);  //Pegando o vertice com menor dist�ncia que ainda est� marcado com X em Q
        Q[u] = '-';
        S[u] = 'X';
        for(v=0 ; v<g->maxV ; v++)      //Busca pelos v�rtices adjacentes (v) do v�rtice u
        {
            if(*(g->matrizAdj +u*g->maxV + v) != 0)
                relaxa(u, v, *(g->matrizAdj +u*g->maxV + v), d, p); //Aplicando a fun��o relaxa nos v�rtices adjacentes
        }
    }
    //Passando o resultado para a tabela
    fprintf(arqLog, "\n   [caminhoDijkstra] Escrevendo o resultado final do caminho em um arquivo.");
    fprintf(tabela, "  TABELA: CAMINHO DIJKSTRA");
    fprintf(tabela, "\n  ARQUIVO DO GRAFO: %s", nomeArqGrafo);
    fprintf(tabela, "\n  VERTICE RAIZ: %d", s);
    fprintf(tabela, "\n  VERTICE  D  PI  Q  S");
    for(i=0 ; i<g->maxV ; i++)
    {
        if(d[i]>=INFINITO)
            fprintf(tabela, "\n    %2d    inf %2d %2c %2c", i, p[i], Q[i], S[i]);
        else
            fprintf(tabela, "\n    %2d    %2d  %2d %2c %2c", i, d[i], p[i], Q[i], S[i]);
    }
}

short caminhoBellmanFord(FILE *tabela, struct grafo *g, int s)          //Fun��o principal para o caminho Bellman Ford
{
    fprintf(arqLog, "\n [caminhoBellmanFord] Caminho Bellman-Ford a partir do v�rtice: %d", s);
    int i, u, v, d[g->maxV], p[g->maxV];
    inicializa(g, s, d, p);
    for(i=1 ; i<g->maxV-1 ; i++)    //Aplicando a fun��o relaxa nas arestas existentes do grafo maxV-1 vezes
    {
        for(u=0 ; u<g->maxV ; u++)              //Para cada aresta (u,v) do grafo
        {
            for(v=0 ; v<g->maxV ; v++)
            {
                if(*(g->matrizAdj +u*g->maxV + v) != 0)
                    relaxa(u, v, *(g->matrizAdj +u*g->maxV + v), d, p); //Aplica a fun��o relaxa para essa aresta
            }
        }
    }
    for(u=0 ; u<g->maxV ; u++)              //Para cada aresta (u,v) do grafo
    {
        for(v=0 ; v<g->maxV ; v++)
        {
            if((*(g->matrizAdj +u*g->maxV + v) != 0) && (d[v] > d[u] + *(g->matrizAdj +u*g->maxV + v))) //Verifica se h� um ciclo de peso negativo
                return(0);
        }
    }
    //Passando o resultado para a tabela
    fprintf(arqLog, "\n   [caminhoBellmanFord] Escrevendo o resultado final do caminho em um arquivo.");
    fprintf(tabela, "  TABELA: CAMINHO BELLMAN-FORD");
    fprintf(tabela, "\n  ARQUIVO DO GRAFO: %s", nomeArqGrafo);
    fprintf(tabela, "\n  VERTICE RAIZ: %d", s);
    fprintf(tabela, "\n  VERTICE  D  PI");
    for(i=0 ; i<g->maxV ; i++)
    {
        if(d[i]>=INFINITO)
            fprintf(tabela, "\n    %2d    inf %2d ", i, p[i]);
        else
            fprintf(tabela, "\n    %2d    %2d  %2d ", i, d[i], p[i]);
    }
    return(1);
}

void verificaPropriedades(struct grafo g, char *cor, short buscaLarg)   //Fun��o para verificar propriedades do grafo. O vetor cor e a vari�vel buscaLarg ser� utilizados para verificar se o grafo � conexo
{
    int i, j, tamanho=0;
    short gSimples=0, gCompleto=0, gConexo=0;
    //A ordem � o n�mero de v�rtices de um grafo
    printf("\n  [ORDEM]: %d", g.maxV);
    fprintf(arqLog, "\n   [verificaPropriedades] Ordem: %d", g.maxV);

    //O tamanho � o n�mero de arestas de um grafo
    for (i=0 ; i<g.maxV ; i++ ) //Percorrendo a matriz para encontrar arestas
    {
        if(!g.tipo)     //Se for grafo n�o orientado, percorre apenas acima da diagonal principal
            j=i;
        else            //Se for grafo orientado, percorre a matriz normalmente
            j=0;
        for( ; j<g.maxV ; j++)
        {
            if(*(g.matrizAdj +i*g.maxV + j)!=0)    //Tem aresta, ent�o incrementa a vari�vel tamanho
                tamanho++;
        }
    }
    printf("\n  [TAMANHO]: %d", tamanho);
    fprintf(arqLog, "\n   [verificaPropriedades] Tamanho: %d", tamanho);

    //Grafo Simples: Sem la�os, Sem arestas m�ltiplas
        //Essa implementa��o n�o usa arestas m�ltiplas (apenas no d�grafo)
        //Para ver la�os: vertice n -> matriz[n][n] == 1
    if(!g.tipo)     //Se for um grafo n�o orientado, a verifica��o ser� feita
    {
        gSimples=1;
        for(i=0 ; i<g.maxV && gSimples ; i++)   //Percorrendo apenas a diagonal principal
        {
            if(*(g.matrizAdj +i*g.maxV + i)!=0) //Tem la�o
                gSimples=0;                     //Ent�o, n�o � grafo simples
        }
        if(gSimples)
        {
            printf("\n  [� UM GRAFO SIMPLES]");
            fprintf(arqLog, "\n   [verificaPropriedades] � um grafo simples");
        }
        else
        {
            printf("\n  [N�O � UM GRAFO SIMPLES]");
            fprintf(arqLog, "\n   [verificaPropriedades] N�o � um grafo simples");
        }
    }

    //Grafo Completo: Grafo simples em que qualquer par de v�rtices distintos s�o adjacentes (apenas a diagonal principal � nula (todo o restante � != 0))
    if(!g.tipo && gSimples) //Se for um grafo n�o orientado e simples (visto anteriormente), a verifica��o ser� feita
    {
        gCompleto=1;
        for(i=0 ; i<g.maxV && gCompleto ; i++)      //Percorrendo toda a matriz
        {
            for( j=i ; j<g.maxV && gCompleto ; j++ )
            {
                if(*(g.matrizAdj +i*g.maxV + j)!=0 && (i==j))    //Tem aresta e est� na diagonal principal, logo n�o � completo
                    gCompleto=0;
                if(*(g.matrizAdj +i*g.maxV + j)==0 && (i!=j))    //N�o tem aresta e n�o est� na diagonal principal, logo n�o � completo
                    gCompleto=0;
            }
        }
        if(gCompleto)
        {
            printf("\n  [� UM GRAFO COMPLETO]");
            fprintf(arqLog, "\n   [verificaPropriedades] � um grafo completo");
        }
        else
        {
            printf("\n  [N�O � UM GRAFO COMPLETO]");
            fprintf(arqLog, "\n   [verificaPropriedades] N�o � um grafo completo");
        }
    }

    //Para ver se � conexo: basta ver o vetor de cor da busca em largura (depois do algoritmo de busca) se h� alguma posi��o branca (se tiver, n�o � conexo)
    //                      ou da pra ver se a distancia � o valor que inicializou o vetor (INFINITO)
    if(buscaLarg)   //Se foi feito a busca em largura, a verifica��o ser� feita, caso contr�rio � indicado para o usu�rio realizar a busca em largura para verificar essa propriedade
    {
        gConexo=1;
        for(i=0 ; i<g.maxV && gConexo ; i++)    //Percorrendo o vetor de cores utilizado na busca em largura (est� assegurado que a busca foi feita)
        {
            if(cor[i]=='b')
                gConexo=0;
        }
        if(gConexo)
        {
            printf("\n  [� UM GRAFO CONEXO]");
            fprintf(arqLog, "\n   [verificaPropriedades] � um grafo conexo");
        }
        else
        {
            printf("\n  [N�O � UM GRAFO CONEXO]");
            fprintf(arqLog, "\n   [verificaPropriedades] N�o � um grafo conexo");
        }
    }
    else
        printf("\n  [DICA]: FA�A A BUSCA EM LARGURA PARA VERIFICAR SE O GRAFO � CONEXO");
}

void visitaDfs(struct grafo *g, int u, int *tempo, int *d, int *f, char *cor)   //Funs�o visita DFS para busca em profundidade
{
    fprintf(arqLog, "\n   [visitaDfs] Visitando o vertice: %d", u);
    int v;
    cor[u] = 'c';                       //Muda a cor do grafo visitado
    *tempo = *tempo + 1;
    d[u] = *tempo;                      //Assim como seu tempo de descoberta
    for(v=0 ; v<g->maxV ; v++)          //Busca pelos v�rtices adjacentes (v) do v�rtice u
    {
        if(*(g->matrizAdj +u*g->maxV + v) != 0)
        {
            if(cor[v] == 'b')           //Se a cor do v�rtice adjacente for branco
                visitaDfs(g, v, tempo, d, f, cor);  //Aplica a fun��o visitaDfs para o v�rtice adjacente
        }
    }
    cor[u] = 'p';           //Quando todos os vertices adjacentes forem visitados, a cor de u muda para preto
    *tempo = *tempo + 1;
    f[u] = *tempo;          //E seu tempo de finaliza��o recebe tempo+1;
}
void buscaEmProfundidade(FILE *tabela, struct grafo *g, int vInicial)   //Fun��o principal para busca em profundidade
{
    fprintf(arqLog, "\n [buscaEmProfundidade] Busca em profundidade a partir do v�rtice: %d", vInicial);
    int u, tempo, d[g->maxV], f[g->maxV];
    char cor[g->maxV];
    for(u=0; u<g->maxV ; u++)   //Inicializando a cor de todos os v�rtices para branco
        cor[u] = 'b';
    tempo = 0;
    if(cor[vInicial] == 'b')    //Fazendo a busca primeiro para o vertice inicial
            visitaDfs(g, vInicial, &tempo, d, f, cor);
    for(u=0; u<g->maxV ; u++)   //Se houve vertices que nao foram visitados, � feito a busca para os outros vertices (dessa forma, TODOS os v�rtices ser�o visitados, mesmo que o grafico nao seja conexo
    {
        if(cor[u] == 'b' && u!=vInicial)
            visitaDfs(g, u, &tempo, d, f, cor);
    }
    //Passando o resultado para a tabela
    fprintf(arqLog, "\n   [buscaEmProfundidade] Escrevendo o resultado final da busca em um arquivo.");
    fprintf(tabela, "  TABELA: BUSCA EM PROFUNDIDADE");
    fprintf(tabela, "\n  ARQUIVO DO GRAFO: %s", nomeArqGrafo);
    fprintf(tabela, "\n  VERTICE RAIZ: %d", vInicial);
    fprintf(tabela, "\n  VERTICE  COR  DESCOBERTA  FINALIZACAO");
    for(u=0 ; u<g->maxV ; u++)
    {
        fprintf(tabela, "\n  %4d     %2c      %2d           %2d", u, cor[u], d[u], f[u]);
    }
}

struct no               //No para a fila
{
    int vertice;
    struct no *prox;
};
struct fila             //Struct para a fila
{
    struct no *ini, *fim;
};
void enfileira(struct fila *f, int valor)   //Fun��o para enfileirar um valor na fila
{
    struct no *p=NULL;
    p=(struct no *)malloc(sizeof(struct no));
    if(p==NULL)
        return;
    p->vertice = valor;
    p->prox = NULL;
    if(f->ini == NULL)
        f->ini = p;
    else
        f->fim->prox = p;
    f->fim = p;
}
int desenfileira(struct fila *f)            //Fun��o para remover um valor da fila
{
    int c;
    struct no *p=NULL;
    if(f->ini==NULL)        //Fila vazia
        return(-1);
    p=f->ini;
    f->ini=p->prox;
    c=p->vertice;
    free(p);
    return(c);  //Retorna o valor removido
}
void buscaEmLargura(FILE *tabela, struct grafo *g, char *cor, int s)   //Fun��o principal para busca em largura no v�tice inicial s
{
    fprintf(arqLog, "\n [buscaEmLargura] Busca em largura a partir do v�rtice: %d", s);
    int u, v, d[g->maxV], p[g->maxV]; //Essa busca em largura utiliza o vetor cor declarado no main, pois depois conseguimos acessar esse vetor para verificar se ele � conexo
    for(u=0; u<g->maxV ; u++)   //Inicilizando os vetores cor, distancia e pai para todos os v�rtices
    {
        cor[u] = 'b';
        d[u] = INFINITO;
        p[u] = -1;
    }
    cor[s] = 'c';               //Inicializando os vetores cor, distancia e pai para o v�rtice inicial
    d[s] = 0;
    p[s] = -1;
    struct fila Q;              //Declarando uma pilha e inicializando
    Q.ini = NULL;
    Q.fim = NULL;
    enfileira(&Q, s);           //Enfileirando o vertice inicial
    fprintf(arqLog, "\n [buscaEmLargura] Enfileirando o vertice: %d", s);
    while(Q.ini!=NULL)          //Enquanto a fila n�o for vazia
    {
        u = desenfileira(&Q);   //Desenfileira um v�rtice
        fprintf(arqLog, "\n [buscaEmLargura] Desenfileirando o vertice: %d", u);
        for(v=0 ; v<g->maxV ; v++)         //Busca pelos v�rtices adjacentes (v) do v�rtice u (que foi desenfileirado)
        {
            if(*(g->matrizAdj +u*g->maxV + v) != 0) //Tem aresta (u,v)
            {
                if(cor[v] == 'b')           //Se a cor do v�rtice adjacente for branco
                {
                    cor[v]='c';             //Sua cor � alterada para cinza
                    d[v]=d[u]+1;            //Sua distancia recebe a distancia de seu "pai" + 1
                    p[v]=u;                 //E seu pai se torna u
                    enfileira(&Q, v);       //Enfileira esse v�rtice v
                    fprintf(arqLog, "\n [buscaEmLargura] Enfileirando o vertice: %d", v);
                }
            }
        }
        cor[u]='p';             //Quando todos os vertices adjacente forem enfileirados, a cor de u muda para preto
    }
    //Passando o resultado para a tabela
    fprintf(arqLog, "\n   [buscaEmLargura] Escrevendo o resultado final da busca em um arquivo.");
    fprintf(tabela, "  TABELA: BUSCA EM LARGURA");
    fprintf(tabela, "\n  ARQUIVO DO GRAFO: %s", nomeArqGrafo);
    fprintf(tabela, "\n  VERTICE RAIZ: %d", s);
    fprintf(tabela, "\n  VERTICE  COR  DISTANCIA  PAI");
    for(u=0 ; u<g->maxV ; u++)
    {
        if(d[u]>=INFINITO)
            fprintf(tabela, "\n  %4d     %2c      inf     %2d", u, cor[u], p[u]);
        else
            fprintf(tabela, "\n  %4d     %2c      %2d      %2d", u, cor[u], d[u], p[u]);
    }
}

int menu(void)
{
    int opcao;
    printf("\n    --------------------------------------------------------------\n");
    printf("    --TRABALHO ESTRUTURA DE DADOS II - GIOVANNA CARREIRA MARINHO--\n");
    printf("    --------------------------------------------------------------\n");
    printf("    -------------Instru��es: 1� Carregue o grafo------------------\n");
    printf("    -------------------------2� Escolha a opera��o----------------\n");
    printf("    --------------------------------------------------------------\n\n");
    printf("    [0]  SAIR \n");
    printf("    [1]  CARREGAR GRAFO \n");
    printf("    [2]  BUSCA EM PROFUNDIDADE \n");
    printf("    [3]  BUSCA EM LARGURA \n");
    printf("    [4]  CAMINHO DIJKSTRA \n");
    printf("    [5]  CAMINHO BELLMAN-FORD \n");
    printf("    [6]  VISUALIZAR MATRIZ DE ADJAC�NCIA \n");
    printf("    [7]  VERIFICAR PROPRIEDADES \n");
    printf("    [8]  ADICIONAR ARESTA \n");
    printf("    [9]  REMOVER ARESTA \n");
    printf("    [10] ADICIONAR V�RTICE \n");
    printf("    [11] REMOVER V�RTICE \n");
    printf("    [12] SALVAR ALTERA��ES \n\n");
    printf("    --------------------------------------------------------------\n\n");
    printf("    [Digite a opera��o desejada]: ");
    scanf("%d", &opcao);
    return(opcao);
    /**
    Obs: -N�o � considerado o peso 0, pois a matriz de adjac�ncia � inicializada com este valor.
         -Todos os v�rtices s�o numerados de 0->maxV-1, logo a adi��o/remo��o de um v�rtice implica em adicionar/remover o maxV/maxV-1 v�rtice.
         -Todas as altera��es adicionar/remover aresta/v�rtice s�o feitas no g1 (grafo principal), no final, o usu�rio poder� gravar as altera��es em um arquivo texto.
    **/
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");
    FILE *grafoTxt=NULL, *tabBuscaLarg=NULL, *tabBuscaProf=NULL, *tabCaminhoDi=NULL, *tabCaminhoBF=NULL, *grafoAt=NULL;
    short buscaLarg=0;    //Variavel que indica se foi feito busca em largura no grafo (ser� utilizada para verificar se um grafo � conexo)
    int op=0, op2=0, vInicial, vFinal, quant, peso;
    char aux[50], copia[50], *cor;
    struct grafo g1, gCopia;
    do
    {
        op = menu();    //Lendo a opera��o digitada pelo usu�rio
        system("cls");
        switch(op)
        {
            case 0:
                fclose(arqLog);
                printf("  [VOC� ESCOLHEU A OP��O SAIR]");
                break;
            case 1:
                printf("\n  [CARREGAR GRAFO] ");
                printf("\n  [NOME DO ARQUIVO .txt COM O GRAFO]: ");
                scanf("%s", &nomeArqGrafo);
                strcpy(copia, nomeArqGrafo);
                strcpy(aux, nomeArqGrafo);
                if((grafoTxt = fopen(strcat(nomeArqGrafo, ".txt"), "r"))==NULL)  //Abrindo o arquivo do grafo
                {
                    printf("  ERRO DE ABERTURA DO ARQUIVO: %s", nomeArqGrafo);
                    printf("\n  VERIFIQUE SUA EXISTENCIA E/OU SEU NOME");
                    break;
                }
                if((arqLog = fopen(strcat(aux, "Logger.txt"), "w"))==NULL)      //Criando o arquivo de Log
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                transformaTextoGrafo(grafoTxt, &g1);            //Transformando o grafo em matriz de adjac�ncia
                fclose(grafoTxt);                               //Fechando o arquivo texto (j� que n�o ser� mais utilizado)
                cor = malloc(sizeof(char)*g1.maxV);             //Alocando dinamicamente o vetor de cor (que ser� utilzado na busca em largura e na fun��o de verificar propriedades)
                buscaLarg = 0;                                  //Inicializando a variavel buscaLarg com 0, uma vez que o grafo acabou de ser carregado e a busca em largura nao foi feita ainda
                printf("  [GRAFO CARREGADO COM SUCESSO] ");
                break;
            case 2:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [BUSCA EM PROFUNDIDADE]");
                strcpy(aux, copia);
                if((tabBuscaProf = fopen(strcat(aux, "TabBProf.txt"), "w"))==NULL)  //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA BUSCA EM PROFUNDIDADE]: ");
                scanf("%d", &vInicial);
                buscaEmProfundidade(tabBuscaProf, &g1, vInicial);       //Chamada a fun��o de busca em profundidade no vertice inicial solicitado ao usu�rio
                fclose(tabBuscaProf);                                   //Fechando o arquivo da tabela pois n�o ser� mais utilizado
                printf("  [BUSCA FEITA COM SUCESSO] ");
                break;
            case 3:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [BUSCA EM LARGURA]");
                strcpy(aux, copia);
                if((tabBuscaLarg = fopen(strcat(aux, "TabBLarg.txt"), "w"))==NULL)  //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA BUSCA EM LARGURA]: ");
                scanf("%d", &vInicial);
                buscaEmLargura(tabBuscaLarg, &g1, cor, vInicial);       //Chamada a fun��o de busca em largura no vertice inicial solicitado ao usu�rio
                buscaLarg=1;                                            //Como foi feito a busca em largura, a vari�vel buscaLarg = 1
                fclose(tabBuscaLarg);                                   //Fechando o arquivo da tabela pois n�o ser� mais utilizado
                printf("  [BUSCA FEITA COM SUCESSO]");
                break;
            case 4:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [CAMINHO DIJKSTRA]");
                strcpy(aux, copia);
                if((tabCaminhoDi = fopen(strcat(aux, "TabCDijkstra.txt"), "w"))==NULL) //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA O CAMINHO DIJKSTRA]: ");
                scanf("%d", &vInicial);
                caminhoDijkstra(tabCaminhoDi, &g1, vInicial);           //Chamada a fun��o de caminho dijkstra no vertice inicial solicitado ao usu�rio
                fclose(tabCaminhoDi);                                   //Fechando o arquivo da tabela pois n�o ser� mais utilizado
                printf("  [CAMINHO FEITO COM SUCESSO]");
                break;
            case 5:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [CAMINHO BELLMAN-FORD]");
                strcpy(aux, copia);
                if((tabCaminhoBF = fopen(strcat(aux, "TabCBFord.txt"), "w"))==NULL) //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA O CAMINHO BELLMAN-FORD]: ");
                scanf("%d", &vInicial);
                caminhoBellmanFord(tabCaminhoBF, &g1, vInicial);        //Chamada a fun��o de caminho bellman ford no vertice inicial solicitado ao usu�rio
                fclose(tabCaminhoBF);                                   //Fechando o arquivo da tabela pois n�o ser� mais utilizado
                printf("  [CAMINHO FEITO COM SUCESSO]");
                break;
            case 6:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [VISUALIZAR MATRIZ DE ADJAC�NCIA]");
                imprimir(g1);       //Chamando a fun��o imprimir para imprimir a matriz de adjacencia
                break;
            case 7:
                printf("\n  [VERIFICAR PROPRIEDADES]");
                verificaPropriedades(g1, cor, buscaLarg);   //Chamada a fun��o para verificar as propriedades do grafo (nessa fun��o ser� impresso as propriedades)
                break;
            case 8:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [ADICIONAR ARESTA]");
                printf("\n  [VERTICE INICIAL DA ARESTA]: ");
                scanf("%d", &vInicial);
                printf("  [VERTICE FINAL DA ARESTA]: ");
                scanf("%d", &vFinal);
                printf("  [PESO DA ARESTA]: ");
                scanf("%d", &peso);
                if(!adicionarAresta(&g1, vInicial, vFinal, peso))       //Chamada a fun��o para adicionar a aresta, cujo o vertice inicial, final e o peso foram solicitados ao usu�rio
                    printf("  [ARESTA %d->%d J� EXISTENTE]", vInicial, vFinal);
                else
                    printf("  [ARESTA %d->%d ADICIONADA COM SUCESSO]", vInicial, vFinal);
                break;
            case 9:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [REMOVER ARESTA]");
                printf("\n  [VERTICE INICIAL DA ARESTA]: ");
                scanf("%d", &vInicial);
                printf("  [VERTICE FINAL DA ARESTA]: ");
                scanf("%d", &vFinal);
                if(!removerAresta(&g1, vInicial, vFinal))           //Chamada a fun��o para remover a aresta, cujo o vertice inicial e final foram solicitados ao usu�rio
                    printf("  [ARESTA %d->%d INEXISTENTE]", vInicial, vFinal);
                else
                    printf("  [ARESTA %d->%d REMOVIDA COM SUCESSO]", vInicial, vFinal);
                break;
            case 10:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [ADICIONAR V�RTICE]");
                printf("\n  [QUANTIDADE DE V�RTICES A SEREM ADICIONADOS]: ");
                scanf("%d", &quant);
                gCopia.tipo = g1.tipo;      //Salvando em gCopia a situa��o atual de g1 (ser� utilizado na fun��o de adicionar)
                gCopia.maxV = g1.maxV;      //esse grafo foi necess�rio pois como o valor de maxV ser� alterado, a matriz de adjac�ncia ser� alocada novamente e suas arestas ser�o perdidas nesse processo
                inicializar(&gCopia);
                copiaMatriz(g1, &gCopia);
                adicionarVertice(gCopia, &g1, quant);   //Adicionando o/os v�rtices
                printf("  [V�RTICE %d ADICIONADO COM SUCESSO]", g1.maxV-1);
                break;
            case 11:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERA��O]");
                    break;
                }
                printf("\n  [REMOVER V�RTICE]");
                printf("\n  [QUANTIDADE DE V�RTICES A SEREM REMOVIDOS]: ");
                scanf("%d", &quant);
                gCopia.tipo = g1.tipo;      //Salvando em gCopia a situa��o atual de g1 (ser� utilizado na fun��o de remo�ao)
                gCopia.maxV = g1.maxV;      //esse grafo foi necess�rio pois como o valor de maxV ser� alterado, a matriz de adjac�ncia ser� alocada novamente e suas arestas ser�o perdidas nesse processo
                inicializar(&gCopia);
                copiaMatriz(g1, &gCopia);
                if(!removerVertice(gCopia, &g1, quant)) //Removendo o/os vertices
                    printf("  [GRAFO VAZIO]");
                else
                    printf("  [V�RTICE %d REMOVIDO COM SUCESSO]", g1.maxV);
                break;
            case 12:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [N�O H� NENHUMA ALTERA��O FEITA NO GRAFO]");
                    break;
                }
                printf("\n  [SALVAR ALTERA��ES]");
                strcpy(aux, copia);
                if((grafoAt = fopen(strcat(aux, "Atualizado.txt"), "w"))==NULL)          //Criando o arquivo para escrever o grafo atualizado
                {
                    printf("  ERRO DE CRIA��O DO ARQUIVO: %s", aux);
                    break;
                }
                transformaGrafoTexto(grafoAt, g1);  //Chamada a fun��o para transformar o grafo em sua representa��o textual
                fclose(grafoAt);                    //Fechando o arquivo texto (j� que n�o ser� mais utilizado)
                printf("\n  [ALTERA��ES FEITAS COM SUCESSO]");
                break;
            default:
                printf("\n  [OPERA��O INV�LIDA]");
        }
        printf("\n");
        system("pause");
        system("cls");
    }
    while (op!=0);
    return(0);
}
