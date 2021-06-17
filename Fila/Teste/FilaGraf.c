#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA_TELA 900
#define ALTURA_TELA  450

struct Pessoa
{
    short Op, Cliente;
    struct Pessoa *next;
};
struct Fila
{
    struct Pessoa *ini, *fim;
    int cont;                   //Contador que conta o número de pessoas na fila
};
void Inicializa(struct Fila **C)
{
    int i=0;
    for(i=0 ; i<4 ; i++)
    {
        C[i] = (struct Fila *)malloc(sizeof (struct Fila));     //Alocando espaço de memoria para a struct Lista para cada caixa
        C[i]->ini = NULL;                                       //Inicializando os campos ini e fim do caixa
        C[i]->fim = NULL;
        C[i]->cont=0;
    }
}
void Imprime(struct Fila **C)
{
    ALLEGRO_FONT* font = al_create_builtin_font();

    int i=0, j=0, pos=50;
    struct Pessoa *p=NULL;
    for(i=0 ; i<4 ; i++)
    {
        j=0;
        al_draw_filled_rectangle( pos, 25 + 100*i, 150, 75 + 100*i, al_map_rgb(145, 145, 145));
        al_draw_textf(font, al_map_rgb(0, 0, 0), pos+75, 50 + 100*i, ALLEGRO_ALIGN_RIGHT, "CAIXA %d", i+1);
        if(C[i]->ini==NULL)
            al_draw_filled_circle( 200 + pos*j, pos + 100*i, 15, al_map_rgb(0, 0, 0));
        else
        {
            for(p=C[i]->ini ; p!=NULL ; p=p->next)
            {
                if((p->Cliente)&&(p->Op))           //Cliente saque - vermelho
                    al_draw_filled_circle( 200 + pos*j, pos + 100*i, 15, al_map_rgb(184, 56, 50));
                else if((p->Cliente) && !(p->Op))   //Cliente deposito - verde
                    al_draw_filled_circle( 200 + pos*j, pos + 100*i, 15, al_map_rgb(65, 150, 69));
                else if(!(p->Cliente) && (p->Op))   //Não cliente saque - azul
                    al_draw_filled_circle( 200 + pos*j, pos + 100*i, 15, al_map_rgb(67, 78, 148));
                else                                //Não cliente deposito - laranja
                    al_draw_filled_circle( 200 + pos*j, pos + 100*i, 15, al_map_rgb(250, 160, 39));
                al_draw_filled_circle( 250 + pos*j, pos + 100*i, 15, al_map_rgb(0, 0, 0));
                j++;
            }
        }
    }

    al_draw_textf(font, al_map_rgb(184, 56, 50), 890, 410, ALLEGRO_ALIGN_RIGHT, "CLIENTE - SAQUE");
    al_draw_textf(font, al_map_rgb(65, 150, 69), 890, 420, ALLEGRO_ALIGN_RIGHT, "CLIENTE - DEPOSITO");
    al_draw_textf(font, al_map_rgb(67, 78, 148), 890, 430, ALLEGRO_ALIGN_RIGHT, "NAO CLIENTE - SAQUE");
    al_draw_textf(font, al_map_rgb(250, 160, 39), 890, 440, ALLEGRO_ALIGN_RIGHT, "NAO CLIENTE - DEPOSITO");

    al_flip_display();  //<Allegro> Atualiza janela.
    al_rest(0.5);
}
short InserePessoa(struct Fila **C, int X, struct Pessoa PX)
{
    struct Pessoa *p=NULL;
    p=(struct Pessoa *)malloc(sizeof(struct Pessoa));
    if(p==NULL)
        return(0);
    p->Cliente = PX.Cliente;
    p->Op = PX.Op;
    p->next = NULL;
    if(C[X]->ini == NULL)
        C[X]->ini = p;
    else
        C[X]->fim->next = p;
    C[X]->fim = p;
    C[X]->cont++;
    return(1);
}
short Remove(struct Fila **C, int X)
{
    struct Pessoa *p=NULL;
    if(C[X]->ini==NULL)        //Fila vazia
        return(0);
    p=C[X]->ini;
    C[X]->ini=p->next;
    C[X]->cont--;
    free(p);
    return(1);
}
void Imprime2(struct Fila **C)
{
    int i=0;
    struct Pessoa *p=NULL;
    for(i=0 ; i<4 ; i++)
    {
        printf("\n\n      [CAIXA %d]", i+1);
        if(C[i]->ini==NULL)
            printf(" CAIXA VAZIO!");
        else
        {
            for(p=C[i]->ini ; p!=NULL ; p=p->next)
            {
                if(p->Cliente)
                    printf(" C-");
                else
                    printf(" NC-");
                if(p->Op)
                    printf("S ");
                else
                    printf("D ");
            }
        }
    }
}
int main(void)
{
    struct Fila * Caixa[4];             //Vetor de ponteiros para a struct Fila -> só irá apontar para ini ou fim (já que são elementos da estrutura Fila)
    struct Pessoa P;                    //Struct Pessoa para receber os valores de rand() e ser passada como parametro na funcao inserir
    int TempoMAX=500, Tempo=0, C, i=0, Menor, Pos=0;     //Variaveis auxiliares

    al_init();                      //<Allegro> Inicializando a biblioteca.
    al_init_primitives_addon();     //<Allegro> Inicializando a biblioteca de primitivas (para fazer o tabuleiro).
    ALLEGRO_DISPLAY *janela = NULL; //<Allegro> Criando e inicializando a variável que representa a janela principal.
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);//<Allegro> Criando a janela, cujo a altura e largura são estipuladas pelas variaveis LARGURA_TELA e ALTURA_TELA.

    Inicializa(Caixa);                  //Inicializando o vetor de ponteiros para a Struct Lista
    Imprime(Caixa);
    srand(time(NULL));                  //Funcao auxiliar para a funcao rand()
    while(Tempo<TempoMAX)
    {
        if(Tempo%5==0)
        {
            P.Cliente=rand()%2;
            P.Op=rand()%2;
            C=rand()%4;
            if(Caixa[C]->ini!=NULL)     //Se no caixa dado por rand() já está ocupado
            {
                for(i=0 ; i<4 ; i++)    //Busco por um caixa vazio
                {
                    if(i==C)
                        continue;
                    if(Caixa[i]->ini==NULL)
                    {
                        InserePessoa(Caixa, i, P);  //E insere no caixa vazio
                        break;
                    }
                }
                if(i==4)                //Não achou, então insere em um caixa com menos pessoa
                {
                    Pos=C;
                    Menor=Caixa[C]->cont;
                    for(i=0 ; i<4 ; i++)    //Busco por um caixa com menos clientes
                    {
                        if(i==C)
                            continue;
                        if(Caixa[i]->cont<Menor)
                        {
                            Menor=Caixa[i]->cont;
                            Pos=i;
                        }
                    }
                    InserePessoa(Caixa, Pos, P);
                }
            }
            else
                InserePessoa(Caixa, C, P);
            system("cls");
            Imprime2(Caixa);
            Imprime(Caixa);
        }
        if(Tempo%6==0)
        {
            C=rand()%4;
            if(Caixa[C]->ini!=NULL)
            {
                Remove(Caixa, C);
                system("cls");
                Imprime2(Caixa);
                Imprime(Caixa);
            }
        }
        Tempo++;
    }

    al_destroy_display(janela);     //<Allegro> Finalizando a janela.
    return(0);
}
