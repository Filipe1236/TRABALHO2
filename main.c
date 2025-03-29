#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headertrabalho2.h"


/*--------------------------------------
| Nome: validar_coordenadas
| Accao: Verifica se as coordenadas estao na gama de valores correta
| Return: 1 se houver erro; 0 se estiver tudo bem
+---------------------------------------*/
int validar_coordenadas(CIDADE localidade){
    int erro = 0;

    if((localidade.latitude < -90) || (localidade.latitude > 90)){
        fprintf(stderr, "ERRO: latitude fora dos limites (menor que -90º ou maior que 90º)!\n");
        erro = 1;
    }

    if((localidade.longitude < - 180) || (localidade.longitude > 180)){
        fprintf(stderr, "ERRO: longitude fora dos limites (menor que -180º ou maior que 180º)!\n");
        erro = 1;
    }

    return erro;
}

/*--------------------------------------
| Nome: verificar_argumentos_agrupados
| Accao: Protege o programa do caso em que argumentos como o -ADI, que exigem de seguida varios parametros, nao os tenham, impedindo que o programa procure elementos do argv de índice superior ao argc!
+---------------------------------------*/
void protege_argumentos_agrupados_do_fim(int posicao_argumento, int numero_argumentos_seguidos, int argc){
    if((posicao_argumento + numero_argumentos_seguidos) > argc){
        fprintf(stderr, "ERRO: ultimo argumento indicado exige de seguida mais %d parametros para o definir na totalidade\n", numero_argumentos_seguidos);
        mostrar_utilizacao();
        exit(-1);
    }
}

/*--------------------------------------
| Nome: mostrar_utilizacao
| Accao: Descreve a utilizacao do programa e os seus argumentos
+---------------------------------------*/
void mostrar_utilizacao(void){
    printf("Utilizacao: processa_rotas [ARGUMENTOS]\n");
    printf("\n::\n::ARGUMENTOS OBRIGATORIOS (vai ser necessario incluir pelo menos um destes para o funcionamento do programa):\n::\n");
    printf("\t-TL :: testa a leitura do ficheiro localidades.txt e valida todos os dados desse ficheiro\n");
    printf("\t-LO <NOME DO FICHEIRO DE OUTPUT PARA AS LOCALIDADES> :: faz o mesmo que -TL, colocando todos os dados recolhidos no ficheiro com o nome especificado\n");
    printf("\t-ROTAS <NUMERO DA ROTA> :: faz o mesmo que -TL, lendo adicionalmente o ficheiro rotas.txt e processando a rota cujo nome foi especificado de seguida ou, caso nao seja indicado, para todas as rotas do ficheiro\n");
    printf("\t\t-LR <NOME DO FICHEIRO DE OUTPUT PARA O PROCESSAMENTO DE ROTAS> :: indica o nome do ficheiro de output onde as rotas vao ser usadas, devendo ser usado com -ROTAS, caso contrario, o ficheiro de output sera output_rotas.txt\n");
    printf("\n::\n::ARGUMENTOS OPCIONAIS:\n::\n");
    printf("\t-ADI <nome> <latitude> <longitude> :: acrescenta uma localidade adicional 'a lista (na memoria do programa) para ser contabilizada nessa execucao\n");
    printf("\t-REM <nome> :: tira da memoria do programa a localidade de nome especificado, deixando de ser contabilizada na execucao\n");
}

/*--------------------------------------
| Nome: validar_calloc
| Accao: Verifica se a alocacao de memoria teve sucesso
+---------------------------------------*/
void validar_calloc(BLOCOCIDADE * pointer_blococidade){
    if(pointer_blococidade == NULL){
        fprintf(stderr, "ERRO: Falha ao alocar memoria!\n");
        exit(-1);
    }
}

/*--------------------------------------
| Nome: criar_lista_cidades
| Accao: Cria a lista de cidades na memoria com um registo separado para a base
| Return: O apontador da base/topo da lista criada
+---------------------------------------*/
BLOCOCIDADE * criar_lista_cidades(void){
    BLOCOCIDADE * novo;
    novo = (BLOCOCIDADE *) calloc(1,sizeof(BLOCOCIDADE));
    validar_calloc(novo);
    /*Atribuicao no bloco, como este registo separado para a base vai ser irrelevante na execucao, vamos chama-lo de Null_Island, assim se ele for contabilizado pelo programa, sabemos logo que tal nao era suposto acontecer!*/
    novo->prox = NULL;
    novo->cidade = atribuir_cidade("Null_Island", 0, 0);

    return novo;
}

/*--------------------------------------
| Nome: compara_alfabeticamente
| Accao: dadas duas strings, verifica se a segunda encontra-se mais para a frente no alfabeto do que a primeira
| Por exemplo compara_alfabeticamente("Aveiro", "Coimbra") == 1, evidente logo pela primeira letra, 'C' > 'A'
| Ja compara_alfabeticamente("Porto_Santo", "Porto_Salvo") == 0, neste caso sendo mais dificil por maioria dos caracteres coincidirem! Já que 'l' da segunda string < 'n' da primeira
| Return: 1 se a segunda string encontrar-se mais para a frente no alfabeto do que a primeira.
| Caso contrario, return 0
| Se ambas as strings forem exatamente iguais, return -1
+---------------------------------------*/
int compara_alfabeticamente(char Abc[], char Def[]){
    int i;
    if(strcmp(Abc, Def) == 0)
        return -1;
    
    for(i = 0; (Abc[i]!= '\0') || (Def[i] != '\0'); i++){
        if (Def[i] > Abc[i])
            return 1;
        if(Def[i] < Abc[i])
            return 0;
    }
    
    /*Ao fim do loop, se uma string terminar mas os caracteres das duas estarem sempre a coincidir, por exemplo, Porto e Porto_Salvo, a string maior sera a mais avancada no alfabeto*/
    if(strlen(Def) > strlen(Abc))
        return 1;
    else
        return 0;
}

/*--------------------------------------
| Nome: encontrar_ponto_insersao_alfabeticamente
| Accao: dada uma lista e um ponteiro do bloco de cidade a ser inserido na lista, encontrar o local em que este novo bloco deve ser inserido na lista para a mesma estar disposta por ordem alfabética
| Exemplo de lista criada neste método: topo->|Null_Island|->|Aveiro|->|Braga|->|Bruxelas|->|Coimbra|->|Lisboa|...
| Return: Ponteiro do bloco anterior ao ponto de insersao!
| RETURN NULL se a cidade a inserir tiver o mesmo nome de uma cidade presente na lista!
+---------------------------------------*/
BLOCOCIDADE * encontrar_ponto_insersao_alfabeticamente(BLOCOCIDADE * topolista, BLOCOCIDADE * novo){
    BLOCOCIDADE * anterior;
    /*Para o caso da lista estar ainda a um único bloco, o que tem a Null_Island, devemos retornar logo o topo!!!*/
    if(topolista->prox == NULL){
        return topolista;
    }
    for(anterior = topolista; ((anterior->prox != NULL) && (compara_alfabeticamente(anterior->prox->cidade.nome, novo->cidade.nome)== 1)); anterior = anterior->prox){
        /*Aproveitar a ocasiao para verificar se estamos a acrescentar 'a lista uma localidade que ja la esta!*/
        if(compara_alfabeticamente(anterior->prox->cidade.nome, novo->cidade.nome) == -1){
            printf("AVISO: a localidade %s encontra-se duplicada pelo argumento -ADI ou no localidades.txt!\n",novo->cidade.nome);
            anterior = NULL;
            return anterior;
        }
    }
    return anterior;
}

/*--------------------------------------
| Nome: insere_na_lista_cidades_alfabeticamente
| Accao: dada uma cidade completamente definida, inseri-la na lista dinamica por ordem alfabetica
| Exemplo de lista criada neste método: topo->|Null_Island|->|Aveiro|->|Braga|->|Bruxelas|->|Coimbra|->|Lisboa|...
+---------------------------------------*/
void insere_na_lista_cidades_alfabeticamente (BLOCOCIDADE * topo, CIDADE cidade_por_inserir){
    BLOCOCIDADE * novo;
    BLOCOCIDADE * anterior_ao_ponto_insersao;

    novo = (BLOCOCIDADE *) calloc(1, sizeof(BLOCOCIDADE));
    validar_calloc(novo);
    novo->cidade = cidade_por_inserir;
    anterior_ao_ponto_insersao = encontrar_ponto_insersao_alfabeticamente(topo, novo);
    /*O codigo abaixo so deve ser executada quando a cidade por adicionar nao e' nenhum duplicado do que ja' esta' na lista!, ou seja, quando o encontrar_ponto_insersao_alfabeticamente(topo, novo) nao da return de NULL*/
    if(anterior_ao_ponto_insersao != NULL){
    novo->prox = anterior_ao_ponto_insersao->prox;
    anterior_ao_ponto_insersao->prox = novo;
    }
    mostrar_lista_cidades(topo);
}


/*--------------------------------------
| Nome: atribuir_cidade
| Accao: coloca o nome e coordenadas numa estrutura de cidade, da qual dá return. Funcao para testes/debug
| Return: cidade criada
+---------------------------------------*/
CIDADE atribuir_cidade(char nome[], float latitude, float longitude){
    CIDADE novacidade;
    strcpy(novacidade.nome,nome);
    novacidade.latitude = latitude;
    novacidade.longitude = longitude;

    return novacidade;
}

/*--------------------------------------
| Nome: mostrar_lista_cidades
| Accao: mostra a lista com o nome das cidades, esta funcao serve apenas para debug!
+---------------------------------------*/
void mostrar_lista_cidades(BLOCOCIDADE * topo){
    BLOCOCIDADE * step;
    for(step = topo->prox; step !=NULL; step = step->prox){
        printf("|%s|->", step->cidade.nome);
    }
    putchar('\n');
}

/*--------------------------------------
| Nome: TL_ler_config
| Accao: tenta ler o ficheiro localidades.txt e vai colocando as cidades lidas na lista por ordem alfabetica.
| OBS: NAO ESQUECER OS AVISOS de erros no ficheiro!
| RETURN DO POINTER DA LISTA DE CIDADES!
+---------------------------------------*/
BLOCOCIDADE * TL_ler_cidades(void){
    BLOCOCIDADE * listatopo;
    char buffer [255];
    int linha = 1;
    int erro = 0;
    CIDADE cidade_intermediaria;
    FILE * ficheirolocalidades;
    ficheirolocalidades = fopen("localidades.txt", "r");
    if(ficheirolocalidades == NULL){
        fprintf(stderr, "ERRO: O ficheiro localidades.txt nao existe! Por favor criar e preencher!\n");
        exit(-1);
    }
    listatopo = criar_lista_cidades();

    while(fgets(buffer,255,ficheirolocalidades) != NULL){
        if(buffer[0] == '\n'){
            buffer[0] = '\0';
        }
        else if(sscanf(buffer,"%s %f %f", cidade_intermediaria.nome, &(cidade_intermediaria.latitude), &(cidade_intermediaria.longitude)) != 3){
            buffer[0] = '\0';
            printf("ERRO: localidades.txt -> linha %d: Erro no numero de parametros!\n", linha);
            erro = 1;
        }
        else
            insere_na_lista_cidades_alfabeticamente(listatopo,cidade_intermediaria);
        linha++;
    }
    fclose(ficheirolocalidades);
    if(erro)
        exit(-1);


    return listatopo;
}


int main(int argc, char * argv[]){
    BLOCOCIDADE * lista;
    lista = TL_ler_cidades();
    mostrar_lista_cidades(lista);
    return 0;
}
