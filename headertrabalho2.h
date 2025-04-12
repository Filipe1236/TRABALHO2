/*----------------------------------------------------------------------
TRABALHO 2 DE PROGRAMAÇÃO 2024/2025
Realizado por:
Beatriz de Carvalho Vaz n.113407
Filipe Braz Gomes n.114217
LEAer2021
-------------------------------------------------------------------------*/
#ifndef HEADER_TRAB2
#define HEADER_TRAB2

/*Constantes simbolicas:*/
#define MAX 30
#define CONTADOR_LINHA_ADI -1 /*Usar quando for para adicionar a cidade extra do -ADI 'a lista!*/
#define RT 6371.0 /*Raio medio da Terra em km*/
#define PI 3.14159

/*Macros que substituem funcoes longas nas expressoes dentro dos if's:*/
#define SSCANF_LOCALIDADES sscanf(buffer,"%s %f %f", cidade_intermediaria.nome, &(cidade_intermediaria.latitude), &(cidade_intermediaria.longitude))
#define SSCANF_LATITUDE sscanf(argv[ADI_POS_LATITUDE], "%f", &(cidade_aux.latitude))
#define SSCANF_LONGITUDE sscanf(argv[ADI_POS_LONGITUDE], "%f", &(cidade_aux.longitude))
/*Macros que servem como funcoes*/
#define FLAGS_ON2(x, y) (argumento[(x)] && argumento[(y)])
#define FLAGS_OFF2(x, y) (!argumento[(x)] || !argumento[(y)])
#define FLAGS_ON3(x, y, z) (argumento[(x)] && argumento[(y)] && argumento[(z)])
#define FLAGS_OFF3(x, y, z) (!argumento[(x)] || !argumento[(y)] || !argumento[(z)])
/*Estes macros formam condicoes em que se verificam todos os argumentos (FLAGS_ON) ou em que nao se verifica pelo menos um deles (FLAGS_OFF)*/

/*DEFINES PARA OS ARGUMENTOS DA MAIN------------------------*/
#define NUMERO_PARAMETROS 14
/*Os primeiros 6 elementos do vetor vao servir como booleanos/flags para detetar se o argumento esta' ou nao presente. 
Ou seja, ficam a 0 se nao estiver, a 1 se estiver*/
#define TL 0
#define LO 1
#define ADI 2
#define REM 3
#define ROTAS 4
#define LR 5
/*Os ultimos elementos vao indicar a posicao no argv em que dados auxiliares associados a um comando se encontram.
Por exemplo, argv[LO_POS_NOME_FICHEIRO] vai ser um apontador da string que tem o nome fornecido para o ficheiro de output logo apos o -LO*/
#define LO_POS_NOME_FICHEIRO argumento[6]
#define ADI_POS_NOME argumento[7]
#define ADI_POS_LATITUDE argumento[8]
#define ADI_POS_LONGITUDE argumento[9]
#define REM_POS_NOME argumento[10]
#define ROTAS_POS_NUMERO_ROTA argumento[11]
#define LR_POS_NOME_FICHEIRO argumento[12]
#define NUMERO_ROTA argumento[13]
/*FIM DOS DEFINES DOS ARGUMENTOS---------------------------------*/


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
    struct rota * prox;
} ROTA;

/*------------------FIM DAS DEFINICOES DE TIPOS--------------*/


/*--------------PROTOTIPOS DE FUNCOES-------------------------*/
void mostrar_utilizacao(void);
CIDADE atribuir_cidade(char nome[], float latitude, float longitude);
void mostrar_lista_cidades(BLOCOCIDADE * topo);
BLOCOCIDADE * TL_ler_cidades(int booleanTL);
BLOCOCIDADE * procurar_cidade(BLOCOCIDADE * lista, char nome_procurado[]);
void remover_da_listacidades(BLOCOCIDADE * lista, char nome_a_remover[]);
int compara_numericamente(float menor, float maior);
int validar_coordenadas(CIDADE localidade);
int erro_argumentos_agrupados(int posicao_argumento, int numero_argumentos_seguidos, int argc);
void mostrar_utilizacao(void);
void validar_calloc(void * pointer);
BLOCOCIDADE * criar_lista_cidades(void);
int compara_alfabeticamente(char Abc[], char Def[]);
BLOCOCIDADE * encontrar_ponto_insersao_alfabeticamente(BLOCOCIDADE * topolista, BLOCOCIDADE * novo);
void insere_na_lista_cidades_alfabeticamente (BLOCOCIDADE * topo, CIDADE cidade_por_inserir, int contador_linha, int booleanTL);
int conta_cidades_da_lista(BLOCOCIDADE * topo);
void libertar_lista(BLOCOCIDADE ** topo);
void LO_escrever_cidades(char nomeficheiro[], BLOCOCIDADE * lista);
float distancia_em_x(CIDADE A, CIDADE B);
float distancia_em_y(CIDADE A, CIDADE B);
float distancia_entre_cidades (CIDADE A, CIDADE B);

ROTA * ciclo_leitura_rotas(BLOCOCIDADE * lista_cidades);
ROTA * construir_bloco_rota(BLOCOCIDADE * lista_cidades, FILE * ficheirorotas, int numero_rota);
void construir_bloco_subrota(BLOCOCIDADE * lista_cidades, char nome_itinerario[], SUBROTA ** aux_subrota, int * numero_itinerarios);
void inserir_rota_distancia_crescente(ROTA * topo_lista_rotas, ROTA * rota_por_inserir);
int sera_letra(char caracter);
int sera_diferente_de_zero(int numero);
ROTA * criar_lista_rotas(void);
SUBROTA * criar_lista_subrotas(BLOCOCIDADE * lista_cidades);
void mostrar_lista_rotas(ROTA * lista);
void tirar_newline(char str[]);

void explica_relacao_argc_comando(int argc);
void libertar_lista_rotas(ROTA **lista);
void libertar_lista_subrotas(SUBROTA **lista);
void output_lista_rotas(ROTA *lista, char nome_ficheiro[]);
void output_rota_unica(ROTA *lista, char nome_ficheiro[], int numero_rota);
void reconhece_comandos_primeira_etapa(int argc, char *argv[], int argumento[NUMERO_PARAMETROS]);
CIDADE reconhece_comandos_segunda_etapa(char *argv[], int argumento[NUMERO_PARAMETROS]);
ROTA * procurar_rota(ROTA * lista_rotas, int numero_rota_procurada);

#endif