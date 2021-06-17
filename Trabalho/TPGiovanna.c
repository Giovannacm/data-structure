#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <limits.h>

#define INFINITO INT_MAX/2          //Será usada para representar as constantes infinito do algoritmo

FILE *arqLog;                       //Arquivo de log (está global para todas as funções terem acesso à ele)
char nomeArqGrafo[50];              //Nome do arquivo do grafo (está global pois será necessário imprimir o nome do grafo em todas as tabelas)

struct grafo                        //Struct que representa o grafo
{
    int tipo;                       //0: grafo (não orientado), 1: digrafo (orientado)
    int maxV;                       //Quantidade de vértices
    int *matrizAdj;                 //A matriz será alocada dinamicamente após o valor de maxV ser determinado
};
void inicializar(struct grafo *g)   //Aloca a matriz dinamicamente após saber o valor de maxV
{
    int i, j;
    g->matrizAdj = (int *)calloc((g->maxV)*(g->maxV), sizeof(int)); //calloc já coloca 0 nas posições
}
void imprimir(struct grafo g)       //Imprime o grafo na implementação de matriz de adjacencia
{
    int i, j;
    printf("\n  [TRIPO DO GRAFO]: %s", (g.tipo) ? "Dígrafo" : "Grafo nao orientado");
    printf("\n  [VÉRTICES]: %d", g.maxV);
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
void copiaMatriz(struct grafo g, struct grafo *gNovo)                       //Função que copia a matriz de adjacência de g em gNovo
{
    int i, j;
    for(i=0 ; i<gNovo->maxV ; i++)
    {
        for(j=0 ; j<gNovo->maxV ; j++)
            *(gNovo->matrizAdj +i*gNovo->maxV + j) = *(g.matrizAdj +i*g.maxV + j);
    }
}
short adicionarAresta(struct grafo *g, int ini, int fim, int peso)          //Função para adicionar uma aresta (ini, fim) com peso
{
    fprintf(arqLog, "\n [adicionarAresta] Adicionando a aresta %d -> %d de peso %d no grafo.", ini, fim, peso);
    if(*(g->matrizAdj +ini*g->maxV + fim)!=0)   //Aresta já existe
    {
        fprintf(arqLog, "\n   [adicionarAresta] ERRO: aresta %d -> %d de peso %d já existe no grafo.", ini, fim, peso);
        return(0);
    }
    *(g->matrizAdj +ini*g->maxV + fim) += peso; //Adicionando o peso na posição correspondentr da matriz [ini][fim]
    if(!g->tipo)    //Grafo (não orientado)
    {
        if(ini!=fim)
            *(g->matrizAdj +fim*g->maxV + ini) += peso; //Adicionando o peso na posição [fim][ini] (pois a matriz é simétrica)
    }
    return(1);
}
short removerAresta(struct grafo *g, int ini, int fim)                      //Função para remover uma aresta (ini, fim) do grafo
{
    fprintf(arqLog, "\n [removerAresta] Removendo a aresta %d -> %d do grafo.", ini, fim);
    if(*(g->matrizAdj +ini*g->maxV + fim)==0)   //Aresta não existe
    {
        fprintf(arqLog, "\n   [removerAresta] ERRO: aresta %d -> %d não existe no grafo.", ini, fim);
        return(0);
    }
    *(g->matrizAdj +ini*g->maxV + fim) = 0; //Coloca 0 na posição [ini][fim] (valor de inicialização)
    if(!g->tipo)    //Grafo (não orientado)
    {
        if(ini!=fim)
            *(g->matrizAdj +fim*g->maxV + ini) = 0; //Coloca 0 na posição [fim][ini] (pois a matriz é simétrica)
    }
    return(1);
}
void adicionarVertice(struct grafo g, struct grafo *gNovo, int quantidade)  //Função para adicionar vértices no grafo gNovo
{
    fprintf(arqLog, "\n [adicionarVertice] Adicionando o vertice %d no grafo.", g.maxV-1);
    gNovo->tipo = g.tipo;               //Copiando o tipo do grafo
    gNovo->maxV = g.maxV + quantidade;  //Mudando o valor de maxV de acordo com a quantidade de vertices a sererm adicionados
    inicializar(gNovo);     //A matriz de adjacencia é alocada novamente (já que o valor de maxV mudou)
    copiaMatriz(g, gNovo);  //O grafo g (ou seja, a cópia do grafo original) possui as arestas anteriores que serão passadas novamente para gNovo
}
short removerVertice(struct grafo g, struct grafo *gNovo, int quantidade)   //Função para remover vértices do grafo gNovo
{
    fprintf(arqLog, "\n [removerVertice] Removendo o vertice %d do grafo.", g.maxV);
    if(g.maxV==0)   //Grafo sem vértices
    {
        fprintf(arqLog, "\n   [removerVertice] ERRO Grafo vazio.");
        return(0);
    }
    gNovo->tipo = g.tipo;               //Copiando o tipo do grafo
    gNovo->maxV = g.maxV - quantidade;  //Mudando o valor de maxV de acordo com a quantidade de vertices a serem removidos
    inicializar(gNovo);    //A matriz de adjacencia é alocada novamente (já que o valor de maxV mudou)
    copiaMatriz(g, gNovo); //O grafo g (ou seja, a cópia do grafo original) possui as arestas anteriores que serão passadas novamente para gNovo
    return(1);
}
void transformaTextoGrafo(FILE *arq, struct grafo *g)       //Texto -> Matriz de adjacencia. Considera um arquivo sem comentários
{
    fprintf(arqLog, "\n [transformaTextoGrafo] Transformando o arquivo em matriz de adjacencia");
    int i, ini, fim, peso;
    fscanf(arq, "%d", &g->tipo);    //Leitura e armazenamento do tipo do grafo
    fprintf(arqLog, "\n   [transformaTextoGrafo] Tipo do grafo: %s", (g->tipo) ? "Dígrafo" : "Grafo nao orientado");
    fscanf(arq, "%d", &g->maxV);    //Leitura e armazenamento da quantidade de vértices
    fprintf(arqLog, "\n   [transformaTextoGrafo] Numero de vertices: %d", g->maxV);
    inicializar(g);                 //Alocando a matriz de adjacencia dinamicamente
    for(i=0 ; !feof(arq) ; i++)     //Leitura das arestas até o fim do arquivo
    {
        fscanf(arq, "%d", &ini);    //Leitura do vertice inicial
        fscanf(arq, "%d", &fim);    //Leitura do vertice final
        fscanf(arq, "%d", &peso);   //Leitura do peso
        fprintf(arqLog, "\n   [transformaTextoGrafo] Leitura da aresta: %d -> %d", ini, fim);
        adicionarAresta(g, ini, fim, peso); //Inserção da aresta
    }
}
void transformaGrafoTexto(FILE *arq, struct grafo g)        //Matriz de adjacencia -> Texto
{
    fprintf(arqLog, "\n [transformaGrafoTexto] Transformando a matriz de adjacencia em um arquivo");
    int i, j;
    fprintf(arq, "%d", g.tipo);     //Escrevendo o tipo do grafo
    fprintf(arqLog, "\n   [transformaGrafoTexto] Tipo do grafo: %s", (g.tipo) ? "Dígrafo" : "Grafo nao orientado");
    fprintf(arq, "\n%d", g.maxV);   //Escrevendo a quantidade de vértices do grafo
    fprintf(arqLog, "\n   [transformaGrafoTexto] Numero de vertices: %d", g.maxV);
    for (i=0 ; i<g.maxV ; i++ )     //Escrevendo as arestas a partir da leitura da matriz de adjacencia
    {
        if(!g.tipo)                 //Se for um grafo não orientado, basta percorrer acima da diagonal principal (a matriz é simétrica)
            j=i;
        else
            j=0;                    //Se for um dígrafo, percorre a matriz normalmente
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

void inicializa(struct grafo *g, int s, int *d, int *p)     //Função de inicialização para o caminho
{
    int i;
    for(i=0 ; i<g->maxV ; i++)  //Inicializado os vetores d e p com infinito e -1, respectivamente
    {
        d[i] = INFINITO;
        p[i] = -1;
    }
    d[s] = 0;   //Colocando 0 na distancia do vertice inicial do caminho
}
void relaxa(int u, int v, int w, int *d, int *p)            //Função relaxa para o caminho
{
    if(d[u]!=INFINITO || d[v]!=INFINITO)    //Se os dois valores a serem comparados forem infinito, sabemos que infinito não é maior que infinito, logo, não é feita a comparação
    {
        if(d[v] > (d[u]+w))
        {
            fprintf(arqLog, "\n   [relaxa] (%d, %d, %d)", u, v, w);
            d[v] = d[u]+w;
            p[v] = u;
        }
    }
}
int minimo(char *Q, int *d, int n)  //Função que retorna o vértice que possui menor distancia (d) e ainda está marcado com X (em Q)
{
    int i, min=INFINITO, pos=0;
    for(i=0 ; i<n ; i++)        //Encontrando o menor valor de d que ainda possui o valor X no vetor Q
    {
        if(Q[i]=='X' && d[i]<min)
        {
            min = d[i];         //Variável min recebe a menor distancia
            pos = i;            //Armazenando a posição para ser retornada
        }
    }
    fprintf(arqLog, "\n   [minimo] Minimo(Q): %d", pos);
    return(pos);
}
short vazio(char *Q, int n)         //Função para verificar se o vetor Q está vazio (ou seja, sem X)
{
    int i;
    for(i=0 ; i<n ; i++)
    {
        if(Q[i]=='X')       //Ainda tem X
            return(0);
    }
    return(1);
}
void caminhoDijkstra(FILE *tabela, struct grafo *g, int s)  //Função principal para o caminho Dijkstra
{
    fprintf(arqLog, "\n [caminhoDijkstra] Caminho Dijkstra a partir do vértice: %d", s);
    int i, u, v, d[g->maxV], p[g->maxV];
    char S[g->maxV], Q[g->maxV];
    inicializa(g, s, d, p);         //Inicializando os vetores d e p
    for(i=0 ; i<g->maxV ; i++)      //Inicializando os vetores S e Q
    {
        S[i] = '-';
        Q[i] = 'X';
    }
    while(!vazio(Q, g->maxV))       //Enquanto não tiver nenhum elemento com X em Q
    {
        u = minimo(Q, d, g->maxV);  //Pegando o vertice com menor distância que ainda está marcado com X em Q
        Q[u] = '-';
        S[u] = 'X';
        for(v=0 ; v<g->maxV ; v++)      //Busca pelos vértices adjacentes (v) do vértice u
        {
            if(*(g->matrizAdj +u*g->maxV + v) != 0)
                relaxa(u, v, *(g->matrizAdj +u*g->maxV + v), d, p); //Aplicando a função relaxa nos vértices adjacentes
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

short caminhoBellmanFord(FILE *tabela, struct grafo *g, int s)          //Função principal para o caminho Bellman Ford
{
    fprintf(arqLog, "\n [caminhoBellmanFord] Caminho Bellman-Ford a partir do vértice: %d", s);
    int i, u, v, d[g->maxV], p[g->maxV];
    inicializa(g, s, d, p);
    for(i=1 ; i<g->maxV-1 ; i++)    //Aplicando a função relaxa nas arestas existentes do grafo maxV-1 vezes
    {
        for(u=0 ; u<g->maxV ; u++)              //Para cada aresta (u,v) do grafo
        {
            for(v=0 ; v<g->maxV ; v++)
            {
                if(*(g->matrizAdj +u*g->maxV + v) != 0)
                    relaxa(u, v, *(g->matrizAdj +u*g->maxV + v), d, p); //Aplica a função relaxa para essa aresta
            }
        }
    }
    for(u=0 ; u<g->maxV ; u++)              //Para cada aresta (u,v) do grafo
    {
        for(v=0 ; v<g->maxV ; v++)
        {
            if((*(g->matrizAdj +u*g->maxV + v) != 0) && (d[v] > d[u] + *(g->matrizAdj +u*g->maxV + v))) //Verifica se há um ciclo de peso negativo
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

void verificaPropriedades(struct grafo g, char *cor, short buscaLarg)   //Função para verificar propriedades do grafo. O vetor cor e a variável buscaLarg será utilizados para verificar se o grafo é conexo
{
    int i, j, tamanho=0;
    short gSimples=0, gCompleto=0, gConexo=0;
    //A ordem é o número de vértices de um grafo
    printf("\n  [ORDEM]: %d", g.maxV);
    fprintf(arqLog, "\n   [verificaPropriedades] Ordem: %d", g.maxV);

    //O tamanho é o número de arestas de um grafo
    for (i=0 ; i<g.maxV ; i++ ) //Percorrendo a matriz para encontrar arestas
    {
        if(!g.tipo)     //Se for grafo não orientado, percorre apenas acima da diagonal principal
            j=i;
        else            //Se for grafo orientado, percorre a matriz normalmente
            j=0;
        for( ; j<g.maxV ; j++)
        {
            if(*(g.matrizAdj +i*g.maxV + j)!=0)    //Tem aresta, então incrementa a variável tamanho
                tamanho++;
        }
    }
    printf("\n  [TAMANHO]: %d", tamanho);
    fprintf(arqLog, "\n   [verificaPropriedades] Tamanho: %d", tamanho);

    //Grafo Simples: Sem laços, Sem arestas múltiplas
        //Essa implementação não usa arestas múltiplas (apenas no dígrafo)
        //Para ver laços: vertice n -> matriz[n][n] == 1
    if(!g.tipo)     //Se for um grafo não orientado, a verificação será feita
    {
        gSimples=1;
        for(i=0 ; i<g.maxV && gSimples ; i++)   //Percorrendo apenas a diagonal principal
        {
            if(*(g.matrizAdj +i*g.maxV + i)!=0) //Tem laço
                gSimples=0;                     //Então, não é grafo simples
        }
        if(gSimples)
        {
            printf("\n  [É UM GRAFO SIMPLES]");
            fprintf(arqLog, "\n   [verificaPropriedades] É um grafo simples");
        }
        else
        {
            printf("\n  [NÃO É UM GRAFO SIMPLES]");
            fprintf(arqLog, "\n   [verificaPropriedades] Não é um grafo simples");
        }
    }

    //Grafo Completo: Grafo simples em que qualquer par de vértices distintos são adjacentes (apenas a diagonal principal é nula (todo o restante é != 0))
    if(!g.tipo && gSimples) //Se for um grafo não orientado e simples (visto anteriormente), a verificação será feita
    {
        gCompleto=1;
        for(i=0 ; i<g.maxV && gCompleto ; i++)      //Percorrendo toda a matriz
        {
            for( j=i ; j<g.maxV && gCompleto ; j++ )
            {
                if(*(g.matrizAdj +i*g.maxV + j)!=0 && (i==j))    //Tem aresta e está na diagonal principal, logo não é completo
                    gCompleto=0;
                if(*(g.matrizAdj +i*g.maxV + j)==0 && (i!=j))    //Não tem aresta e não está na diagonal principal, logo não é completo
                    gCompleto=0;
            }
        }
        if(gCompleto)
        {
            printf("\n  [É UM GRAFO COMPLETO]");
            fprintf(arqLog, "\n   [verificaPropriedades] É um grafo completo");
        }
        else
        {
            printf("\n  [NÃO É UM GRAFO COMPLETO]");
            fprintf(arqLog, "\n   [verificaPropriedades] Não é um grafo completo");
        }
    }

    //Para ver se é conexo: basta ver o vetor de cor da busca em largura (depois do algoritmo de busca) se há alguma posição branca (se tiver, não é conexo)
    //                      ou da pra ver se a distancia é o valor que inicializou o vetor (INFINITO)
    if(buscaLarg)   //Se foi feito a busca em largura, a verificação será feita, caso contrário é indicado para o usuário realizar a busca em largura para verificar essa propriedade
    {
        gConexo=1;
        for(i=0 ; i<g.maxV && gConexo ; i++)    //Percorrendo o vetor de cores utilizado na busca em largura (está assegurado que a busca foi feita)
        {
            if(cor[i]=='b')
                gConexo=0;
        }
        if(gConexo)
        {
            printf("\n  [É UM GRAFO CONEXO]");
            fprintf(arqLog, "\n   [verificaPropriedades] É um grafo conexo");
        }
        else
        {
            printf("\n  [NÃO É UM GRAFO CONEXO]");
            fprintf(arqLog, "\n   [verificaPropriedades] Não é um grafo conexo");
        }
    }
    else
        printf("\n  [DICA]: FAÇA A BUSCA EM LARGURA PARA VERIFICAR SE O GRAFO É CONEXO");
}

void visitaDfs(struct grafo *g, int u, int *tempo, int *d, int *f, char *cor)   //Funsão visita DFS para busca em profundidade
{
    fprintf(arqLog, "\n   [visitaDfs] Visitando o vertice: %d", u);
    int v;
    cor[u] = 'c';                       //Muda a cor do grafo visitado
    *tempo = *tempo + 1;
    d[u] = *tempo;                      //Assim como seu tempo de descoberta
    for(v=0 ; v<g->maxV ; v++)          //Busca pelos vértices adjacentes (v) do vértice u
    {
        if(*(g->matrizAdj +u*g->maxV + v) != 0)
        {
            if(cor[v] == 'b')           //Se a cor do vértice adjacente for branco
                visitaDfs(g, v, tempo, d, f, cor);  //Aplica a função visitaDfs para o vértice adjacente
        }
    }
    cor[u] = 'p';           //Quando todos os vertices adjacentes forem visitados, a cor de u muda para preto
    *tempo = *tempo + 1;
    f[u] = *tempo;          //E seu tempo de finalização recebe tempo+1;
}
void buscaEmProfundidade(FILE *tabela, struct grafo *g, int vInicial)   //Função principal para busca em profundidade
{
    fprintf(arqLog, "\n [buscaEmProfundidade] Busca em profundidade a partir do vértice: %d", vInicial);
    int u, tempo, d[g->maxV], f[g->maxV];
    char cor[g->maxV];
    for(u=0; u<g->maxV ; u++)   //Inicializando a cor de todos os vértices para branco
        cor[u] = 'b';
    tempo = 0;
    if(cor[vInicial] == 'b')    //Fazendo a busca primeiro para o vertice inicial
            visitaDfs(g, vInicial, &tempo, d, f, cor);
    for(u=0; u<g->maxV ; u++)   //Se houve vertices que nao foram visitados, é feito a busca para os outros vertices (dessa forma, TODOS os vértices serão visitados, mesmo que o grafico nao seja conexo
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
void enfileira(struct fila *f, int valor)   //Função para enfileirar um valor na fila
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
int desenfileira(struct fila *f)            //Função para remover um valor da fila
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
void buscaEmLargura(FILE *tabela, struct grafo *g, char *cor, int s)   //Função principal para busca em largura no vétice inicial s
{
    fprintf(arqLog, "\n [buscaEmLargura] Busca em largura a partir do vértice: %d", s);
    int u, v, d[g->maxV], p[g->maxV]; //Essa busca em largura utiliza o vetor cor declarado no main, pois depois conseguimos acessar esse vetor para verificar se ele é conexo
    for(u=0; u<g->maxV ; u++)   //Inicilizando os vetores cor, distancia e pai para todos os vértices
    {
        cor[u] = 'b';
        d[u] = INFINITO;
        p[u] = -1;
    }
    cor[s] = 'c';               //Inicializando os vetores cor, distancia e pai para o vértice inicial
    d[s] = 0;
    p[s] = -1;
    struct fila Q;              //Declarando uma pilha e inicializando
    Q.ini = NULL;
    Q.fim = NULL;
    enfileira(&Q, s);           //Enfileirando o vertice inicial
    fprintf(arqLog, "\n [buscaEmLargura] Enfileirando o vertice: %d", s);
    while(Q.ini!=NULL)          //Enquanto a fila não for vazia
    {
        u = desenfileira(&Q);   //Desenfileira um vértice
        fprintf(arqLog, "\n [buscaEmLargura] Desenfileirando o vertice: %d", u);
        for(v=0 ; v<g->maxV ; v++)         //Busca pelos vértices adjacentes (v) do vértice u (que foi desenfileirado)
        {
            if(*(g->matrizAdj +u*g->maxV + v) != 0) //Tem aresta (u,v)
            {
                if(cor[v] == 'b')           //Se a cor do vértice adjacente for branco
                {
                    cor[v]='c';             //Sua cor é alterada para cinza
                    d[v]=d[u]+1;            //Sua distancia recebe a distancia de seu "pai" + 1
                    p[v]=u;                 //E seu pai se torna u
                    enfileira(&Q, v);       //Enfileira esse vértice v
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
    printf("    -------------Instruções: 1º Carregue o grafo------------------\n");
    printf("    -------------------------2º Escolha a operação----------------\n");
    printf("    --------------------------------------------------------------\n\n");
    printf("    [0]  SAIR \n");
    printf("    [1]  CARREGAR GRAFO \n");
    printf("    [2]  BUSCA EM PROFUNDIDADE \n");
    printf("    [3]  BUSCA EM LARGURA \n");
    printf("    [4]  CAMINHO DIJKSTRA \n");
    printf("    [5]  CAMINHO BELLMAN-FORD \n");
    printf("    [6]  VISUALIZAR MATRIZ DE ADJACÊNCIA \n");
    printf("    [7]  VERIFICAR PROPRIEDADES \n");
    printf("    [8]  ADICIONAR ARESTA \n");
    printf("    [9]  REMOVER ARESTA \n");
    printf("    [10] ADICIONAR VÉRTICE \n");
    printf("    [11] REMOVER VÉRTICE \n");
    printf("    [12] SALVAR ALTERAÇÕES \n\n");
    printf("    --------------------------------------------------------------\n\n");
    printf("    [Digite a operação desejada]: ");
    scanf("%d", &opcao);
    return(opcao);
    /**
    Obs: -Não é considerado o peso 0, pois a matriz de adjacência é inicializada com este valor.
         -Todos os vértices são numerados de 0->maxV-1, logo a adição/remoção de um vértice implica em adicionar/remover o maxV/maxV-1 vértice.
         -Todas as alterações adicionar/remover aresta/vértice são feitas no g1 (grafo principal), no final, o usuário poderá gravar as alterações em um arquivo texto.
    **/
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");
    FILE *grafoTxt=NULL, *tabBuscaLarg=NULL, *tabBuscaProf=NULL, *tabCaminhoDi=NULL, *tabCaminhoBF=NULL, *grafoAt=NULL;
    short buscaLarg=0;    //Variavel que indica se foi feito busca em largura no grafo (será utilizada para verificar se um grafo é conexo)
    int op=0, op2=0, vInicial, vFinal, quant, peso;
    char aux[50], copia[50], *cor;
    struct grafo g1, gCopia;
    do
    {
        op = menu();    //Lendo a operação digitada pelo usuário
        system("cls");
        switch(op)
        {
            case 0:
                fclose(arqLog);
                printf("  [VOCÊ ESCOLHEU A OPÇÃO SAIR]");
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
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                transformaTextoGrafo(grafoTxt, &g1);            //Transformando o grafo em matriz de adjacência
                fclose(grafoTxt);                               //Fechando o arquivo texto (já que não será mais utilizado)
                cor = malloc(sizeof(char)*g1.maxV);             //Alocando dinamicamente o vetor de cor (que será utilzado na busca em largura e na função de verificar propriedades)
                buscaLarg = 0;                                  //Inicializando a variavel buscaLarg com 0, uma vez que o grafo acabou de ser carregado e a busca em largura nao foi feita ainda
                printf("  [GRAFO CARREGADO COM SUCESSO] ");
                break;
            case 2:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [BUSCA EM PROFUNDIDADE]");
                strcpy(aux, copia);
                if((tabBuscaProf = fopen(strcat(aux, "TabBProf.txt"), "w"))==NULL)  //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA BUSCA EM PROFUNDIDADE]: ");
                scanf("%d", &vInicial);
                buscaEmProfundidade(tabBuscaProf, &g1, vInicial);       //Chamada a função de busca em profundidade no vertice inicial solicitado ao usuário
                fclose(tabBuscaProf);                                   //Fechando o arquivo da tabela pois não será mais utilizado
                printf("  [BUSCA FEITA COM SUCESSO] ");
                break;
            case 3:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [BUSCA EM LARGURA]");
                strcpy(aux, copia);
                if((tabBuscaLarg = fopen(strcat(aux, "TabBLarg.txt"), "w"))==NULL)  //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA BUSCA EM LARGURA]: ");
                scanf("%d", &vInicial);
                buscaEmLargura(tabBuscaLarg, &g1, cor, vInicial);       //Chamada a função de busca em largura no vertice inicial solicitado ao usuário
                buscaLarg=1;                                            //Como foi feito a busca em largura, a variável buscaLarg = 1
                fclose(tabBuscaLarg);                                   //Fechando o arquivo da tabela pois não será mais utilizado
                printf("  [BUSCA FEITA COM SUCESSO]");
                break;
            case 4:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [CAMINHO DIJKSTRA]");
                strcpy(aux, copia);
                if((tabCaminhoDi = fopen(strcat(aux, "TabCDijkstra.txt"), "w"))==NULL) //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA O CAMINHO DIJKSTRA]: ");
                scanf("%d", &vInicial);
                caminhoDijkstra(tabCaminhoDi, &g1, vInicial);           //Chamada a função de caminho dijkstra no vertice inicial solicitado ao usuário
                fclose(tabCaminhoDi);                                   //Fechando o arquivo da tabela pois não será mais utilizado
                printf("  [CAMINHO FEITO COM SUCESSO]");
                break;
            case 5:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [CAMINHO BELLMAN-FORD]");
                strcpy(aux, copia);
                if((tabCaminhoBF = fopen(strcat(aux, "TabCBFord.txt"), "w"))==NULL) //Criando o arquivo da tabela
                {
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                printf("\n  [VERTICE INICIAL PARA O CAMINHO BELLMAN-FORD]: ");
                scanf("%d", &vInicial);
                caminhoBellmanFord(tabCaminhoBF, &g1, vInicial);        //Chamada a função de caminho bellman ford no vertice inicial solicitado ao usuário
                fclose(tabCaminhoBF);                                   //Fechando o arquivo da tabela pois não será mais utilizado
                printf("  [CAMINHO FEITO COM SUCESSO]");
                break;
            case 6:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [VISUALIZAR MATRIZ DE ADJACÊNCIA]");
                imprimir(g1);       //Chamando a função imprimir para imprimir a matriz de adjacencia
                break;
            case 7:
                printf("\n  [VERIFICAR PROPRIEDADES]");
                verificaPropriedades(g1, cor, buscaLarg);   //Chamada a função para verificar as propriedades do grafo (nessa função será impresso as propriedades)
                break;
            case 8:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [ADICIONAR ARESTA]");
                printf("\n  [VERTICE INICIAL DA ARESTA]: ");
                scanf("%d", &vInicial);
                printf("  [VERTICE FINAL DA ARESTA]: ");
                scanf("%d", &vFinal);
                printf("  [PESO DA ARESTA]: ");
                scanf("%d", &peso);
                if(!adicionarAresta(&g1, vInicial, vFinal, peso))       //Chamada a função para adicionar a aresta, cujo o vertice inicial, final e o peso foram solicitados ao usuário
                    printf("  [ARESTA %d->%d JÁ EXISTENTE]", vInicial, vFinal);
                else
                    printf("  [ARESTA %d->%d ADICIONADA COM SUCESSO]", vInicial, vFinal);
                break;
            case 9:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [REMOVER ARESTA]");
                printf("\n  [VERTICE INICIAL DA ARESTA]: ");
                scanf("%d", &vInicial);
                printf("  [VERTICE FINAL DA ARESTA]: ");
                scanf("%d", &vFinal);
                if(!removerAresta(&g1, vInicial, vFinal))           //Chamada a função para remover a aresta, cujo o vertice inicial e final foram solicitados ao usuário
                    printf("  [ARESTA %d->%d INEXISTENTE]", vInicial, vFinal);
                else
                    printf("  [ARESTA %d->%d REMOVIDA COM SUCESSO]", vInicial, vFinal);
                break;
            case 10:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [ADICIONAR VÉRTICE]");
                printf("\n  [QUANTIDADE DE VÉRTICES A SEREM ADICIONADOS]: ");
                scanf("%d", &quant);
                gCopia.tipo = g1.tipo;      //Salvando em gCopia a situação atual de g1 (será utilizado na função de adicionar)
                gCopia.maxV = g1.maxV;      //esse grafo foi necessário pois como o valor de maxV será alterado, a matriz de adjacência será alocada novamente e suas arestas serão perdidas nesse processo
                inicializar(&gCopia);
                copiaMatriz(g1, &gCopia);
                adicionarVertice(gCopia, &g1, quant);   //Adicionando o/os vértices
                printf("  [VÉRTICE %d ADICIONADO COM SUCESSO]", g1.maxV-1);
                break;
            case 11:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [CARREGUE UM GRAFO ANTES DE REALIZAR ESTA OPERAÇÃO]");
                    break;
                }
                printf("\n  [REMOVER VÉRTICE]");
                printf("\n  [QUANTIDADE DE VÉRTICES A SEREM REMOVIDOS]: ");
                scanf("%d", &quant);
                gCopia.tipo = g1.tipo;      //Salvando em gCopia a situação atual de g1 (será utilizado na função de remoçao)
                gCopia.maxV = g1.maxV;      //esse grafo foi necessário pois como o valor de maxV será alterado, a matriz de adjacência será alocada novamente e suas arestas serão perdidas nesse processo
                inicializar(&gCopia);
                copiaMatriz(g1, &gCopia);
                if(!removerVertice(gCopia, &g1, quant)) //Removendo o/os vertices
                    printf("  [GRAFO VAZIO]");
                else
                    printf("  [VÉRTICE %d REMOVIDO COM SUCESSO]", g1.maxV);
                break;
            case 12:
                if(grafoTxt==NULL)  //Nenhum grafo foi carregado
                {
                    printf("\n  [NÃO HÁ NENHUMA ALTERAÇÃO FEITA NO GRAFO]");
                    break;
                }
                printf("\n  [SALVAR ALTERAÇÕES]");
                strcpy(aux, copia);
                if((grafoAt = fopen(strcat(aux, "Atualizado.txt"), "w"))==NULL)          //Criando o arquivo para escrever o grafo atualizado
                {
                    printf("  ERRO DE CRIAÇÃO DO ARQUIVO: %s", aux);
                    break;
                }
                transformaGrafoTexto(grafoAt, g1);  //Chamada a função para transformar o grafo em sua representação textual
                fclose(grafoAt);                    //Fechando o arquivo texto (já que não será mais utilizado)
                printf("\n  [ALTERAÇÕES FEITAS COM SUCESSO]");
                break;
            default:
                printf("\n  [OPERAÇÃO INVÁLIDA]");
        }
        printf("\n");
        system("pause");
        system("cls");
    }
    while (op!=0);
    return(0);
}
