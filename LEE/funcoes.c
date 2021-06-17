#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define MAX 7

struct reg{
	int elem;
	int prox;
};

struct TipoLista{
	int dispo;
	int prim;
	struct reg A[MAX];
};

void Inicializar(struct TipoLista *L){
	int i;
	L->prim=-1;
	L->dispo=0;

	for(i=0;i<MAX-1;i++){
		L->A[i].prox=i+1;
	}
	L->A[i].prox=-1;
	return;
}

void Imprimir(struct TipoLista *L){
	int aux;
	aux=L->prim;
	if(aux==-1) printf("Lista vazia.\n");
	else{
		while(aux!=-1){
			printf("%d\n", L->A[aux].elem);
			aux=L->A[aux].prox;
		}
	}
	return;
}

void Inserir(struct TipoLista *lista, int elemento){
	int aux,atual,anterior;
	if(lista->dispo != -1){												//cabe.
		lista->A[lista->dispo].elem = elemento;
		aux=lista->A[lista->dispo].prox;								//recebe próx do dispo.
		if(lista->prim==-1){											//lista vazia(único elem).
			lista->prim=lista->dispo;									//o primeiro é o dispo.
			lista->A[lista->dispo].prox=-1;								//add e finaliza a lista.
			lista->dispo=aux;											//próx disponível.
		}
		else
			if(elemento < lista->A[lista->prim].elem){					//inserção no início da lista
				lista->A[lista->dispo].prox = lista->prim;
				lista->prim = lista->dispo;
				lista->dispo=aux;
			}
			else{														//não é no início.
				atual=lista->A[lista->prim].prox;
				while(atual!=-1 && elemento>lista->A[atual].elem){
					anterior=atual;
					atual=lista->A[atual].prox;
				}
				lista->A[anterior].prox=lista->dispo;
				lista->A[lista->dispo].prox=atual;
				lista->dispo=aux;
			}
		}
	else {																//dispo==-1(cheia).
		aux= lista->prim;
		if(elemento < lista->A[aux].elem){
			lista->A[aux].elem=elemento;
		}
		else
			while(lista->A[aux].elem < elemento){
				aux=lista->A[aux].prox;
			}
			lista->A[aux].elem=elemento;
	}
	return;
}
int Tamanho(struct TipoLista *L){
	int aux,tamanho;
	aux=L->prim;
	tamanho=0;
	while(aux!=-1){
		tamanho++;
		aux=L->A[aux].prox;
	}
	return tamanho;
}

short int Buscar(struct TipoLista *L, int elemento){
	int atual;
	atual=L->prim;
	while(atual!=-1 && L->A[atual].elem<=elemento){
		if(L->A[atual].elem==elemento)	return(1);
		atual=L->A[atual].prox;
	}
	return(0);
}

void Remover(struct TipoLista *L, int elemento){
	int i, anterior;
	int tamanho;
	tamanho=Tamanho(L);
	anterior=-1;
	i=L->prim;
	if(L->prim!=-1){
	if(tamanho>1){
		while(i!=-1 && elemento>L->A[i].elem){
			anterior=i;
			i=L->A[i].prox;
		}
		if(i!=-1 && elemento==L->A[i].elem){
			if(anterior==-1){					//Remove o primeiro elemento.
				L->dispo=i;
				L->prim=L->A[i].prox;
			}
			else if(L->A[i].prox==-1){
				L->A[anterior].prox=-1;
			}
			else{											//Remove no meio.
				L->dispo=L->A[anterior].prox;
				L->A[anterior].prox=L->A[i].prox;
			}
			tamanho--;
		}
	}
	else{
		if(L->A[L->prim].elem==elemento){	//Apenas um elemento.
			L->prim=-1;
			L->A[i].prox=L->dispo;
			L->dispo=i;
			tamanho--;
		}
	}
	}
	return;
}

int main(){
	setlocale(LC_ALL,"Portuguese");
	struct TipoLista L1;

	Inicializar(&L1);


	Inserir(&L1,4);
	Imprimir(&L1);
	if(Buscar(&L1,2)) printf("\nPertence.");
	else	printf("\nNão pertence.\n");
	if(Buscar(&L1,99)) printf("\nPertence.");
	else	printf("\nNão pertence.\n");
	Remover(&L1,4);

	//Remover(&L1,7);
	printf("Removendo.\n");
	Imprimir(&L1);
		Inserir(&L1,5);
	return(0);
	system("Pause");
}
