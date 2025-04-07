#ifndef HEADER_TRAB2
#define HEADER_TRAB2

#define MAX 30
#define CODIGO_CONTADOR_LINHA_ADI -1 /*Usar quando for para adicionar a cidade extra do -ADI 'a lista!*/
#define MAXIMO_DE_ARGUMENTOS 14

#define RT 6371.0 /*Raio medio da Terra em km*/
#define PI 3.14159

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

typedef struct subrota{
    float distancia;
    BLOCOCIDADE * itinerario; /*Pointer para uma cidade na lista de localidades. Usamos o procurar_cidade para ir preenchendo, e assim ja temos acesso aos dados da cidade certa, sem ter de estar a duplicar os dados numa nova lista de cidades (ia gastar MUITA memoria desnecessariamente dessa forma)*/
    struct subrota * prox;
} SUBROTA;

typedef struct rota{
    float distancia_total;
    int numero;
    SUBROTA * lista; /*Topo da lista de subrotas*/
    struct blocorota * prox;
} ROTA;

/*------------------FIM DAS DEFINICOES DE TIPOS--------------*/

/*--------------PROTOTIPOS DE FUNCOES-------------------------*/
void mostrar_utilizacao(void);
CIDADE atribuir_cidade(char nome[], float latitude, float longitude);
void mostrar_lista_cidades(BLOCOCIDADE * topo);
BLOCOCIDADE * TL_ler_cidades(void);
BLOCOCIDADE * procurar_cidade(BLOCOCIDADE * lista, char nome_procurado[]);
void remover_da_listacidades(BLOCOCIDADE * lista, char nome_a_remover[]);
int compara_numericamente(float menor, float maior);
int validar_coordenadas(CIDADE localidade);
void protege_argumentos_agrupados_do_fim(int posicao_argumento, int numero_argumentos_seguidos, int argc);
void mostrar_utilizacao(void);
void validar_calloc(BLOCOCIDADE * pointer_blococidade);
BLOCOCIDADE * criar_lista_cidades(void);
int compara_alfabeticamente(char Abc[], char Def[]);
BLOCOCIDADE * encontrar_ponto_insersao_alfabeticamente(BLOCOCIDADE * topolista, BLOCOCIDADE * novo);
void insere_na_lista_cidades_alfabeticamente (BLOCOCIDADE * topo, CIDADE cidade_por_inserir, int contador_linha);
int conta_cidades_da_lista(BLOCOCIDADE * topo);
void libertar_lista(BLOCOCIDADE ** topo);
void LO_escrever_cidades(char nomeficheiro[], BLOCOCIDADE * lista);
float distancia_em_x(CIDADE A, CIDADE B);
float distancia_em_y(CIDADE A, CIDADE B);
float distancia_entre_cidades (CIDADE A, CIDADE B);

#endif