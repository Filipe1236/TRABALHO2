#ifndef HEADER_TRAB2
#define HEADER_TRAB2

#define MAX 25
#define MAXIMO_DE_ARGUMENTOS 14

#define SEM_ROTA -123456789
/*------------------DEFINICOES DE TIPOS--------------*/
typedef struct {
    char nome[MAX];
    float latitude;
    float longitude;
} CIDADE;

typedef struct blocolocalidade{
    CIDADE cidade;
    struct blocolocalidade * prox;
} BLOCOCIDADE;

/*------------------FIM DAS DEFINICOES DE TIPOS--------------*/

/*--------------PROTOTIPOS DE FUNCOES-------------------------*/
void mostrar_utilizacao(void);
CIDADE atribuir_cidade(char nome[], float latitude, float longitude);
void mostrar_lista_cidades(BLOCOCIDADE * topo);
#endif