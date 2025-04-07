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
| Return: 1 se a segunda string encontrar-se mais para a frente no alfabeto do que a primeira ou se ambas forem exatamente iguais!
| Caso contrario, return 0
+---------------------------------------*/
int compara_alfabeticamente(char Abc[], char Def[]){
    int i;
    /*if(strcmp(Abc, Def) == 0){
        printf("São iguais!\n");
        return 2;
    }*/
    
    for(i = 0; (Abc[i]!= '\0') || (Def[i] != '\0'); i++){
        if (Def[i] > Abc[i])
            return 1;
        if(Def[i] < Abc[i])
            return 0;
    }
    
    /*Ao fim do loop, se uma string terminar mas os caracteres das duas estarem sempre a coincidir, por exemplo, Porto e Porto_Salvo, a string maior sera a mais avancada no alfabeto*/
    /*Vai dar return de 1 se forem exatamente iguais*/
    if(strlen(Def) >= strlen(Abc))
        return 1;
    else
        return 0;
}

/*--------------------------------------
| Nome: encontrar_ponto_insersao_alfabeticamente
| Accao: dada uma lista e um ponteiro do bloco de cidade a ser inserido na lista, encontrar o local em que este novo bloco deve ser inserido na lista para a mesma estar disposta por ordem alfabética
| Exemplo de lista criada neste método: topo->|Null_Island|->|Aveiro|->|Braga|->|Bruxelas|->|Coimbra|->|Lisboa|...
| Return: Ponteiro do bloco anterior ao ponto de insersao!
+---------------------------------------*/
BLOCOCIDADE * encontrar_ponto_insersao_alfabeticamente(BLOCOCIDADE * topolista, BLOCOCIDADE * novo){
    BLOCOCIDADE * anterior;
    /*Para o caso da lista estar ainda a um único bloco, o que tem a Null_Island, devemos retornar logo o topo!!!*/
    if(topolista->prox == NULL){
        return topolista;
    }
    /*Desloca o pointer anterior para a posicao anterior ao bloco que deve inserir, ou seja, o proximo bloco ja' tem uma cidade com o nome à frente no abecedario!
    Pelo compara_alfabeticamente dar return de 1 quando ambas as strings sao iguais, o ponteiro fica a apontar para o bloco com o mesmo nome do bloco novo nesse cenario*/
    for(anterior = topolista; ((anterior->prox != NULL) && (compara_alfabeticamente(anterior->prox->cidade.nome, novo->cidade.nome))); anterior = anterior->prox){}
    return anterior;
}

/*--------------------------------------
| Nome: insere_na_lista_cidades_alfabeticamente
| Accao: dada uma cidade completamente definida, inseri-la na lista dinamica por ordem alfabetica
| Terceiro parametro: contador da posicao da linha do ficheiro a ser lida para indicar no aviso caso essa linha se encontre duplicada!
| Colocar nesse parametro negativo se for para inserir pelo -ADI! Zero caso seja desnecessario
| Exemplo de lista criada neste método: topo->|Null_Island|->|Aveiro|->|Braga|->|Bruxelas|->|Coimbra|->|Lisboa|...
+---------------------------------------*/
void insere_na_lista_cidades_alfabeticamente (BLOCOCIDADE * topo, CIDADE cidade_por_inserir, int contador_linha){
    BLOCOCIDADE * novo;
    BLOCOCIDADE * anterior_insersao;

    novo = (BLOCOCIDADE *) calloc(1, sizeof(BLOCOCIDADE));
    validar_calloc(novo);
    novo->cidade = cidade_por_inserir;
    anterior_insersao = encontrar_ponto_insersao_alfabeticamente(topo, novo);
    /*Aproveitar a ocasiao para verificar se estamos a acrescentar 'a lista uma localidade que ja' la' esta!*/
    if(strcmp(anterior_insersao->cidade.nome, novo->cidade.nome) == 0){
        printf("AVISO: localidades.txt -> linha %d: a localidade %s encontra-se duplicada pelo argumento -ADI (linha negativa) ou no localidades.txt (linha positiva)!\n",contador_linha, novo->cidade.nome);
        free(novo);
        novo = NULL;
    }
    /*O codigo abaixo so deve ser executada quando a cidade por adicionar nao e' nenhum duplicado do que ja' esta' na lista!*/
    else{
        novo->prox = anterior_insersao->prox;
        anterior_insersao->prox = novo;
    }
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
| Nome: conta_cidades_da_lista
| Accao: percorre a lista toda e conta quantas cidades estao la', incluindo a Null_Island, que serviu como registo separado para a base!!
| RETURN: numero total de elementos da lista!
+---------------------------------------*/
int conta_cidades_da_lista(BLOCOCIDADE * topo){
    int elementos = 0;
    for(; topo != NULL; topo = topo->prox, elementos++){}
    return elementos;
}

/*--------------------------------------
| Nome: libertar_lista
| Accao: liberta todos os blocos alocados de uma lista
+---------------------------------------*/
void libertar_lista(BLOCOCIDADE ** topo){
    BLOCOCIDADE * step;
    while(*topo != NULL){
        step = *topo;
        *topo = (*topo)->prox;
        free(step);
    }
}

/*--------------------------------------
| Nome: TL_ler_cidades
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
            insere_na_lista_cidades_alfabeticamente(listatopo,cidade_intermediaria, linha);
        linha++;
    }
    fclose(ficheirolocalidades);
    if(erro)
        exit(-1);


    return listatopo;
}

/*--------------------------------------
| Nome: LO_escrever_cidades
| Accao: escreve o ficheiro de output dos dados da lista das cidades para o argumento -LO
+---------------------------------------*/
void LO_escrever_cidades(char nomeficheiro[], BLOCOCIDADE * lista){
    FILE * outputcidades;
    BLOCOCIDADE * step;
    int elementos_lista_contados = conta_cidades_da_lista(lista);
    int numerocidades = elementos_lista_contados - 1;
    int bytes_alocados_lista = elementos_lista_contados * sizeof(BLOCOCIDADE); /*Obs: o sizeof costuma funcionar melhor com o tipo size_t, mas parece que alguns compiladores nao o reconhecem (foi o meu caso), por isso fui pelo int, mais universal!*/
    outputcidades = fopen(nomeficheiro, "w");
    if(outputcidades == NULL){
        fprintf(stderr, "ERRO: nao foi possivel escrever ficheiro de output, por favor verificar permissoes!\n");
        exit(-1);
    }
    fprintf(outputcidades, "Numero de cidades:%d\n",numerocidades);
    fprintf(outputcidades, "Foram alocados %d bytes para a lista!\n", bytes_alocados_lista);
    fprintf(outputcidades, "Formato <NOME> <LATITUDE> <lONGITUDE>\n");
    for(step = lista->prox; step != NULL; step = step->prox){
        fprintf(outputcidades, "%s %g %g\n", step->cidade.nome, step->cidade.latitude, step->cidade.longitude);
    }
    fclose(outputcidades);
}

/*TESTES DE DEBUG_____________________*/
BLOCOCIDADE * procurar_cidade(BLOCOCIDADE * lista, char nome_procurado[]){
    BLOCOCIDADE * step, * aux;
    step = lista;
    for(;(step != NULL)&&(strcmp(step->cidade.nome,nome_procurado));step = step->prox);
    for(aux = lista; aux->prox != step; aux = aux->prox);

    return aux;
}

void remover_da_listacidades(BLOCOCIDADE * lista, char nome_a_remover[]){
    BLOCOCIDADE * ant_remover, * remover;
    ant_remover = procurar_cidade(lista, nome_a_remover);
    remover = ant_remover->prox;
    if(remover==NULL)
        printf("Nao foi encontrada uma cidade com esse nome.\n");
    else{
        ant_remover->prox = remover->prox;
        free(remover);
    }
}


/*_________________________*/

int main(int argc, char * argv[]){
    BLOCOCIDADE * lista;
    lista = TL_ler_cidades();
    LO_escrever_cidades("outputcidades.txt", lista);
    mostrar_lista_cidades(lista);
    remover_da_listacidades(lista, "Porto");
    mostrar_lista_cidades(lista);
    libertar_lista(&lista);
    return 0;
}
