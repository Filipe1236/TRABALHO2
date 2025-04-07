/*Trabalho que podes ir fazendo enquanto ando ocupado com o relatório de MA*/
/*Vou te deixando protótipos de funções e os seus cabeçalhos a explicar o que elas fazem para depois ires desenvolvendo estas funcoes*/
/*QUALQUER AJUDA QUE PRECISES, POR FAVOR NAO HESITES EM DIZER-ME, eu vejo o que posso fazer!!!!*/

#include <stdio.h>
#include "headertrabalho2.h"
/*Nao te preocupes com includes nem nada disso, o que fores fazendo, deixa no headertrabalho2.h e no trabalho2.c, este ficheiro é só para te dar ideias do que podes ir fazendo*/
/*Preciso que depois passes as funcoes que fiz no main.c para o trabalho2.c ou outro parecido e que trates dos #include's*/

/*TAREFA: melhorar esta funcao para verificar que as coordenadas das cidades estao na gama de valores certa
e tentar implementá-la na funcao TL_ler_cidades
para mostrar ao longo de todo o ficheiro se há erros, e se houver pelo menos um erro, sair do programa (exit(-1);)*/
/*--------------------------------------
| Nome: validar_coordenadas
| Accao: Verifica se as coordenadas estao na gama de valores correta
| Return: 1 se houver erro; 0 se estiver tudo bem
+---------------------------------------*/
int validar_coordenadas(CIDADE localidade, int contador_linhas){
    int erro = 0;

    if((localidade.latitude < -90) || (localidade.latitude > 90)){
        fprintf(stderr, "ERRO: localidades.txt -> linha %d: erro latitude fora dos limites (menor que -90º ou maior que 90º)!\n", contador_linhas);
        erro = 1;
    }

    if((localidade.longitude < - 180) || (localidade.longitude > 180)){
        fprintf(stderr, "ERRO: localidades.txt -> linha %d: erro longitude fora dos limites (menor que -180º ou maior que 180º)!\n", contador_linhas);
        erro = 1;
    }

    return erro;
}

/*TAREFA: desenvolver estas funcoes. Podes mudar os nomes delas e das suas variaveis 'a vontade!
sugestao: strcmp(pointerbloco->cidade.nome, nome_procurado)==0 quando ambas sao iguais*/
/*--------------------------------------
| Nome: procurar_cidade
| Accao: procura na lista de cidades a cidade com o nome especificado
| Return: o pointer do BLOCOCIDADE com a cidade do nome indicado ou o anterior(o anterior e' capaz de ter mais utilidade, olha depois para a proxima funcao e decide)
| RETURN NULL se a funcao percorrer a lista toda e nao encontrar a cidade com esse nome
+---------------------------------------*/
BLOCOCIDADE * procurar_cidade(BLOCOCIDADE * lista, char nome_procurado[]);

/*--------------------------------------
| Nome: remover_da_listacidades
| Accao: procura na lista das cidades um bloco com uma cidade com o nome indicado e remove esse bloco da lista. Util para o argumento -REM
| Deve libertar o bloco com esse nome com o free(pointer_do_bloco) e nao deve estragar as ligacoes dos blocos da lista
| Antes: |Cidade1|->|Cidade2|->|Cidade3|
| Depois, ao fazer remover_da_listacidades("nome da cidade2"): |Cidade1|->|Cidade3|
+---------------------------------------*/
void remover_da_listacidades(BLOCOCIDADE * lista, char nome_a_remover[]);

/*--------------------------------------
| Nome: compara_numericamente
| Accao: dados dois floats (duas distancias), verifica se a segunda distancia (a que supostamente deve ser maior que a primeira) e' mesmo maior que a primeira
| Return: 1 se o segundo numero for maior ou igual que o primeiro
| Caso contrario (menor), return 0
| Por exemplo compara_numericamente(100.5, 200.43) == 1
| Ja compara_numericamente(200.43, 100.5) == 0, porque nao e' verdade que o segundo numero e' maior que o primeiro
| Esta funcao vai ser util para escolher onde e' que se deve colocar um bloco de ROTA's para a lista de rotas ficar por ordem crescente das distancias totais!
+---------------------------------------*/
int compara_numericamente(float menor, float maior);


/*TAREFA: podes ir comecando a pensar na lista das rotas, eu proprio ja' pensei um bocado em como fazer, mas tens mesmo a certeza que queres fazer isso? Esta parte e' mesmo mindblowing!*/
/*Tens aqui as minhas sugestoes de typedefs que podemos usar*/

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

/*Eu depois posso tentar fazer um esquema de como esta lista dentro de uma lista vai funcionar*/
/*Bom trabalho!*/