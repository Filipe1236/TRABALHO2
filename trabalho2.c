/*----------------------------------------------------------------------
TRABALHO 2 DE PROGRAMACAO 2024/2025
Realizado por:
Beatriz de Carvalho Vaz n.113407
Filipe Braz Gomes n.114217
LEAer2021
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "headertrabalho2.h"

/*--------------------------------------
| Nome: validar_coordenadas
| Accao: Verifica se as coordenadas estao na gama de valores correta
| Return: 1 se houver erro; 0 se estiver tudo bem
+---------------------------------------*/
int validar_coordenadas(CIDADE localidade){
    int erro = 0;

    if((localidade.latitude < -90) || (localidade.latitude > 90)){
        fprintf(stderr, "ERRO: latitude fora dos limites (menor que -90 graus ou maior que 90 graus)!\n");
        erro = 1;
    }

    if((localidade.longitude < -180) || (localidade.longitude > 180)){
        fprintf(stderr, "ERRO: longitude fora dos limites (menor que -180 graus ou maior que 180 graus)!\n");
        erro = 1;
    }

    return erro;
}



/*--------------------------------------
| Nome: mostrar_utilizacao
| Accao: Descreve a utilizacao do programa e os seus argumentos
+---------------------------------------*/
void mostrar_utilizacao(void){
    printf("Utilizacao: processa_rotas [ARGUMENTOS]\n");
    printf("Comandos disponiveis (a ordem dos parametros que comecam com '-' pode ser diferente!):\n");
    printf("->1)./processa_rotas -LO <nomeoutputlocalidades> -TL\n");
        printf("\tTesta a leitura do localidades.txt\n");
        printf("\tIndicar nome do ficheiro de output 'a frente do -LO\n");
    printf("->2)./processa_rotas -LO <nomeoutputlocalidades> -ADI <nome> <latitude> <longitude>\n");
        printf("\tAdiciona uma localidade 'a memoria do programa\n");
    printf("->3)./processa_rotas -LO <nomeoutputlocalidades> -REM <nome>\n");
        printf("\tRemove uma localidade da memoria do programa\n");
    printf("->4)./processa_rotas -LO <nomeoutputrotas> -LR <nomeoutputrotas> -ROTAS <numerorota>\n");
        printf("\tProcessa tanto localidades como a rota de numero especificado.\n");
        printf("\tIndicar nome do ficheiro de output da rota 'a frente do -LR\n");
        printf("\tSe nao for indicada nenhuma rota 'a frente do -ROTAS, processa-se todas as rotas do rotas.txt\n");
}

/*--------------------------------------
| Nome: validar_calloc
| Accao: Verifica se a alocacao de memoria teve sucesso.
+---------------------------------------*/
void validar_calloc(void * pointer){
    if(pointer == NULL){
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
| Quarto parametro: se o parametro TL esta' presente (1) ou nao(0)!
| Colocar nesse parametro negativo se for para inserir pelo -ADI! Zero caso seja desnecessario
| Exemplo de lista criada neste método: topo->|Null_Island|->|Aveiro|->|Braga|->|Bruxelas|->|Coimbra|->|Lisboa|...
+---------------------------------------*/
void insere_na_lista_cidades_alfabeticamente (BLOCOCIDADE * topo, CIDADE cidade_por_inserir, int contador_linha, int booleanTL){
    BLOCOCIDADE * novo;
    BLOCOCIDADE * anterior_insersao;

    novo = (BLOCOCIDADE *) calloc(1, sizeof(BLOCOCIDADE));
    validar_calloc(novo);
    novo->cidade = cidade_por_inserir;
    anterior_insersao = encontrar_ponto_insersao_alfabeticamente(topo, novo);
    /*Aproveitar a ocasiao para verificar se estamos a acrescentar 'a lista uma localidade que ja' la' esta!*/
    if(strcmp(anterior_insersao->cidade.nome, novo->cidade.nome) == 0){
        if(booleanTL)
            printf("AVISO: localidades.txt -> linha %d: a localidade %s encontra-se duplicada!\n",contador_linha, novo->cidade.nome);
        if(contador_linha == CONTADOR_LINHA_ADI)
            printf("AVISO: a cidade dada pelo -ADI vai ser ignorada porque ja' existe no localidades.txt!\n");
        free(novo);
        novo = NULL;
    }
    /*O codigo abaixo so deve ser executado quando a cidade por adicionar nao e' nenhum duplicado do que ja' esta' na lista!*/
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
| Nome: libertar_lista_cidades
| Accao: liberta todos os blocos alocados da lista de cidades
+---------------------------------------*/
void libertar_lista_cidades(BLOCOCIDADE ** topo){
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
| OBS: Tem como parametro um "boolean" da existencia ou nao do parametro TL, permitindo mostrar ou nao os avisos!
| RETURN DO POINTER DA LISTA DE CIDADES!
+---------------------------------------*/
BLOCOCIDADE * TL_ler_cidades(int booleanTL){
    BLOCOCIDADE * listatopo;
    char buffer [255];
    int linha = 1;
    int erro = 0;
    int return_scan;
    CIDADE cidade_intermediaria;
    FILE * ficheirolocalidades;
    ficheirolocalidades = fopen("localidades.txt", "r");
    if(ficheirolocalidades == NULL){
        fprintf(stderr, "ERRO: O ficheiro localidades.txt nao existe! Por favor criar e preencher!\n");
        exit(-1);
    }
    listatopo = criar_lista_cidades();

    while(fgets(buffer,255,ficheirolocalidades) != NULL){
        /*if((buffer[0] == '\n') || (buffer[0] == '\r')){
            buffer[0] = '\0';
        }*/
        if(((return_scan = SSCANF_LOCALIDADES) == 3) && (validar_coordenadas(cidade_intermediaria) == 0))
            insere_na_lista_cidades_alfabeticamente(listatopo,cidade_intermediaria, linha, booleanTL);
        else if (validar_coordenadas(cidade_intermediaria) && booleanTL){
            printf("Erro nos valores das coordenadas na linha %d.\n", linha);
            erro = 1;
        }
        else if ((return_scan > 0) && booleanTL){
            printf("ERRO: localidades.txt -> linha %d: Erro no numero de parametros!\n", linha);
            erro = 1;
        }
        buffer[0] = '\0';
        linha++;
    }

    fclose(ficheirolocalidades);
    if(erro && booleanTL)
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

/*--------------------------------------------------------------------
| Nome: distancia_em_x
|
| Acao: calcula a distancia de B em relacao a A no eixo x no plano 2D
|
| Return: a longitude de B em relacao a A
+--------------------------------------------------------------------*/
float distancia_em_x(CIDADE A, CIDADE B){
    float longitude_f;
    float graus = B.longitude - A.longitude;

    longitude_f = RT * tan(graus * PI / 180);

    return longitude_f;
}

/*---------------------------------------------------------------------
| Nome: distancia_em_y
|
| Acao: calcula a distancia de B em relacao a A no eixo y no plano 2D
|
| Return: a latitude de B em relacao a A
+---------------------------------------------------------------------*/
float distancia_em_y(CIDADE A, CIDADE B){
    float latitude_f;
    float graus = B.latitude - A.latitude;

    latitude_f = RT * tan(graus * PI / 180);

    return latitude_f;
}

/*---------------------------------------------------------------------
| Nome: distancia_entre_cidades
|
| Acao: calcula a distancia de B em relacao a A tendo em conta os
|       valores obtidos para x e y de B em relacao a A
|
| Return: distancia linear de A a B
+---------------------------------------------------------------------*/
float distancia_entre_cidades (CIDADE A, CIDADE B){
    float distancia;
    float xab = distancia_em_x(A,B);
    float yab = distancia_em_y(A,B);

    distancia = sqrt (pow(xab,2)+pow(yab,2));

    return distancia;
}

/*--------------------------------------
| Nome: procurar_cidade
| Accao: procura na lista de cidades a cidade com o nome especificado
| Return: o pointer ANTERIOR ao BLOCOCIDADE com a cidade do nome indicado
| RETURN NULL se a funcao percorrer a lista toda e nao encontrar a cidade com esse nome
+---------------------------------------*/
BLOCOCIDADE * procurar_cidade(BLOCOCIDADE * lista, char nome_procurado[]){
    BLOCOCIDADE * step;
    for (step = lista;(step->prox!=NULL)&&(strcmp(step->prox->cidade.nome, nome_procurado) != 0);step = step->prox); 
    /*Caso nao encontre, return de NULL*/
    if(step->prox == NULL)
        return NULL;
    /*Caso encontre, para dar return do anterior*/
    return step;
}

/*--------------------------------------
| Nome: remover_da_listacidades
| Accao: procura na lista das cidades um bloco com uma cidade com o nome indicado e remove esse bloco da lista. Util para o argumento -REM
| Deve libertar o bloco com esse nome com o free(pointer_do_bloco) e nao deve estragar as ligacoes dos blocos da lista
| Antes: |Cidade1|->|Cidade2|->|Cidade3|
| Depois, ao fazer remover_da_listacidades("nome da cidade2"): |Cidade1|->|Cidade3|
+---------------------------------------*/
void remover_da_listacidades(BLOCOCIDADE * lista, char nome_a_remover[]){
    BLOCOCIDADE *ant_remover, *remover;
    
    ant_remover = procurar_cidade (lista, nome_a_remover);
    if ( ant_remover == NULL){
        printf("Nao foi encontrada uma cidade com o nome %s. Por favor tente de novo.\n", nome_a_remover);
        return;
    }
    remover = ant_remover->prox;
    ant_remover->prox = remover->prox;
    free(remover);
}

/*--------------------------------------
| Nome: compara_numericamente
| Accao: dados dois floats (duas distancias), verifica se a segunda distancia (a que supostamente deve ser maior que a primeira) e' mesmo maior que a primeira
| Return: 1 se o segundo numero for maior ou igual que o primeiro
| Caso contrario (menor), return 0
| Por exemplo compara_numericamente(100.5, 200.43) == 1
| Ja compara_numericamente(200.43, 100.5) == 0, porque nao e' verdade que o segundo numero e' maior que o primeiro
| Esta funcao vai ser util para escolher onde e' que se deve colocar um bloco de ROTA's para a lista de rotas ficar por ordem crescente das distancias totais!
+---------------------------------------*/
int compara_numericamente(float menor, float maior){
    if (menor <= maior)
        return 1;
    return 0;
}

/*FUNCOES PARA O CICLO DE LEITURA DO ROTAS.TXT E CONSTRUCAO DA LISTA COM AS ROTAS*/

/*--------------------------------------
| Nome: ciclo_leitura_rotas
| Accao: depois da lista de cidades ter sido criada, ler o ficheiro rotas.txt e construir a lista de rotas. 
| A lista de rotas será também uma lista com um registo separado para a base.
| Return: apontador para o início da lista das rotas
+---------------------------------------*/
ROTA * ciclo_leitura_rotas(BLOCOCIDADE * lista_cidades){
    FILE * ficheirorotas;
    ROTA * lista_rotas, * novo;
    long posicao_inicial;
    int numero_rota;
    char buffer[255];
    ficheirorotas = fopen("rotas.txt", "r");
    if(ficheirorotas == NULL){
        fprintf(stderr, "ERRO: o ficheiro rotas.txt nao existe!\n");
        exit(-1);
    }
    lista_rotas = criar_lista_rotas();
    while(fgets(buffer,255,ficheirorotas) != NULL){
        if(sscanf(buffer,"#ROTA %d", &numero_rota)==1){
            posicao_inicial = ftell(ficheirorotas); /*Tenho de guardar esta posicao no ficheiro porque depois vou avancar demais no construir_bloco_rota, por isso convem voltar atras, logo depois de encontrar o #ROTA serve*/
            printf("Rota %d encontrada!\n", numero_rota);
            buffer[0] = '\0';
            novo = construir_bloco_rota(lista_cidades, ficheirorotas, numero_rota);
            if(novo != NULL)
                inserir_rota_distancia_crescente(lista_rotas, novo);
            fseek(ficheirorotas, posicao_inicial, SEEK_SET); /*Voltar atras no ficheiro*/
        }
        else
            buffer[0] = '\0';
    }
    fclose(ficheirorotas);
    return lista_rotas;
}

/*--------------------------------------
| Nome: construir_bloco_rota
| Accao: funcao auxiliar do ciclo_leitura_rotas: assim que o ciclo de leitura encontra um #ROTA, esta funcao passa a ser chamada para recolher o nome de todos os itinerarios e ir preenchendo todos os campos necessarios (primeiro o numero da rota, depois a lista de SUBROTA's, e por fim a distancia_total)
| Return: apontador do bloco ROTA criado
| Return NULL se a rota estiver vazia ou nao apresentar itinerarios contidos na lista de cidades (tem de haver correspondencia total nos nomes!). A propria funcao liberta o bloco criado nesse caso!
+---------------------------------------*/
ROTA * construir_bloco_rota(BLOCOCIDADE * lista_cidades, FILE * ficheirorotas, int numero_rota){
    char buffer[255];
    int numero_itinerarios = 0;
    ROTA * novo;
    SUBROTA * aux_subrota;
    novo = (ROTA *) calloc(1, sizeof(ROTA));
    validar_calloc(novo);
    novo->prox = NULL;
    novo->numero = numero_rota;
    novo->lista = criar_lista_subrotas(lista_cidades);
    aux_subrota = novo->lista;
    while((fgets(buffer,255, ficheirorotas) != NULL)&&(buffer[0] != '#')){
        if(sera_letra(buffer[0])){
            tirar_newline(buffer);
            construir_bloco_subrota(lista_cidades, buffer, &aux_subrota, &numero_itinerarios);
        }
        buffer[0] = '\0';
    }

    if(numero_itinerarios<1){
        printf("AVISO: a rota %d esta' vazia ou nao apresenta itinerarios contidos no localidades.txt! Esta rota sera' ignorada!\n", numero_rota);
        free(novo->lista);
        novo->lista = NULL;
        free(novo);
        novo = NULL;
        return novo;
    }
    /*Quando chegar ao fim do loop, devemos ter um bloco de ROTA montado com toda a sua lista de itinerarios validos. Portanto, a distancia do ultimo bloco SUBROTA da lista (apontado pelo aux_subrota) deve ter a distancia_total a colocar dentro do bloco ROTA*/
    novo->distancia_total = aux_subrota->distancia;
    return novo;
}

/*--------------------------------------
| Nome: tirar_newline
| Accao: encontra um '\n' numa string e termina-a com '\0' na posicao da primeira ocorrencia desse '\n'
| Util para tirar o '\n' de strings como o buffer (o fgets capta a linha toda, incluindo o '\n', ficando com um cenario em que strcmp("Lisboa", "Lisboa\n")!=0)
| Observacao: devido a uns problemas com o '\r' entre windows e linux, duplicou-se o codigo para garantir que nao existe nem '\n' nem '\r' no fim do buffer!
+---------------------------------------*/
void tirar_newline(char str[]){
    char * ocorrencia_newline;
    ocorrencia_newline = strchr(str, '\n');
    if(ocorrencia_newline != NULL)
        *ocorrencia_newline = '\0';
    /*E agora faz-se o mesmo para o '\r'*/
    ocorrencia_newline = strchr(str, '\r');
    if(ocorrencia_newline != NULL)
        *ocorrencia_newline = '\0';
}

/*--------------------------------------
| Nome: construir_bloco_subrota
| Accao: funcao auxiliar para o construir_bloco_rota: dentro de cada ROTA, vai colocar o apontador itinerario a apontar para a respetiva cidade (de forma a recolher as coordenadas)
| E vai preencher o campo distancia enquanto acumula distancia de um bloco SUBROTA para outro, sendo usado de fora o apontador aux_subrota(passagem por endereco), que fica sempre a apontar para a zona em que se vai inserir um novo bloco, ie, no fim da lista
| Passa por endereco o numero de itinerarios que ja colocou na lista de SUBROTA's, util para garantir que o primeiro caso a distancia fica a zero e para contar quantos itinerarios conseguiu encontrar
+---------------------------------------*/
void construir_bloco_subrota(BLOCOCIDADE * lista_cidades, char nome_itinerario[], SUBROTA ** aux_subrota, int * numero_itinerarios){
    BLOCOCIDADE * aux_procurar_cidade;
    SUBROTA * novo_subrota;
    aux_procurar_cidade = procurar_cidade(lista_cidades, nome_itinerario);

    novo_subrota = (SUBROTA *) calloc(1, sizeof(SUBROTA));
    validar_calloc(novo_subrota);
    novo_subrota->prox = NULL;
    /*printf("Aux_procurar_cidade->prox esta a apontar para %s\n", aux_procurar_cidade->prox->cidade.nome);*/
    if(aux_procurar_cidade == NULL){
        printf("Nao foi encontrada nenhuma localidade com o nome %s!\n", nome_itinerario);
        free(novo_subrota);
        novo_subrota = NULL;
    }

    else{
        novo_subrota->itinerario = aux_procurar_cidade->prox;
        novo_subrota->distancia = (*aux_subrota)->distancia + sera_diferente_de_zero(*numero_itinerarios) * distancia_entre_cidades((*aux_subrota)->itinerario->cidade, novo_subrota->itinerario->cidade);
        (*aux_subrota)->prox = novo_subrota;
        *aux_subrota = (*aux_subrota)->prox;
        *numero_itinerarios = *numero_itinerarios + 1;
    }
}

/*--------------------------------------
| Nome: inserir_rota_distancia_crescente
| Accao: tendo o apontador do topo da lista de ROTA's e um apontador de um novo bloco, ja' preenchido com a distancia_total e todos os itinerarios, coloca-lo por ordem crescente da distancia_total na lista
| Exemplo da lista por ordem crescente, representando apenas o valor de distancia_total
| topo_lista-rotas->|-999KM|->|40KM|->|45.74KM|->|50KM|->|100KM|->|155KM|->NULL
| De lembrar que como o primeiro bloco do tipo ROTA na lista e' um registo separado que nao vai ser usado pelo programa, ele vai ter um valor de distancia total negativo
+---------------------------------------*/
void inserir_rota_distancia_crescente(ROTA * topo_lista_rotas, ROTA * rota_por_inserir){
    ROTA * step;
    for(step = topo_lista_rotas; (step->prox != NULL)&&(compara_numericamente(step->prox->distancia_total, rota_por_inserir->distancia_total)); step = step->prox){}
    rota_por_inserir->prox = step->prox;
    step->prox = rota_por_inserir;
}

/*--------------------------------------
| Nome: sera_letra
| Accao: dado um char, verifica se ele e' uma letra
| Return: 1 se for uma letra. Caso contrário, zero.
| Observacao: vai ser util para, no primeiro bloco de subrota efetivo da lista de subrotas, a sua distancia ficar zero KM, e nao igual 'a distancia entre o primeiro itinerario e a Null_Island!
+---------------------------------------*/
int sera_letra(char caracter){
    if((caracter >= 'A') && (caracter <= 'Z'))
        return 1;
    if((caracter >= 'a') && (caracter <= 'z'))
        return 1;
    return 0;
}

/*--------------------------------------
| Nome: sera_diferente_de_zero
| Accao: dado um numero, verifica se e' diferente de zero
| Return: zero se o numero for zero. Caso contrário, return 1
+---------------------------------------*/
int sera_diferente_de_zero(int numero){
    if(numero == 0)
        return 0;
    return 1;
}

/*--------------------------------------
| Nome: criar_lista_rotas
| Accao: depois da lista de cidades ter sido criada, ler o ficheiro rotas.txt e construir a lista de rotas. 
| A lista de rotas será uma lista com um registo separado para a base,
| Logo a ROTA do topo terá lista==NULL, numero [da rota] e distancia_total negativos para distinguir dos blocos com ROTA's efetivos.
| Return: apontador para o início da lista das rotas
+---------------------------------------*/
ROTA * criar_lista_rotas(void){
    ROTA * topo;
    topo = calloc(1, sizeof(ROTA));
    validar_calloc(topo);

    /*atribuicao de valores arbitrarios, maioria apenas possiveis de observar neste registo separado para a base:*/
    topo->distancia_total=-999;
    topo->numero=-123456789;
    topo->lista=NULL;
    topo->prox=NULL;

    return topo;
}

/*--------------------------------------
| Nome: criar_lista_subrotas
| Accao: criar uma lista de subrotas com o registo separado para a base a apontar para a Null_Island como itinerario, como forma de a distinguir das restantes, deve ter distancia zero para nao afetar os calculos da distancia!
| A lista de SUBROTA's dentro de cada ROTA será uma lista com um registo separado para a base,
| Logo a ROTA do topo terá lista==NULL, numero [da rota] e distancia_total negativos para distinguir dos blocos com ROTA's efetivos.
| Return: apontador para o início da lista das subrotas
+---------------------------------------*/
SUBROTA * criar_lista_subrotas(BLOCOCIDADE * lista_cidades){
    SUBROTA * subrota_nula;
    subrota_nula = (SUBROTA *)calloc(1, sizeof(SUBROTA));
    validar_calloc(subrota_nula);
    subrota_nula->itinerario = lista_cidades;
    subrota_nula->prox = NULL;
    subrota_nula->distancia = 0;
    return subrota_nula;
}

/*--------------------------------------
| Nome: output_lista_rotas
| Accao: escreve num ficheiro o output de todas as rotas processadas!
+---------------------------------------*/
void output_lista_rotas(ROTA * lista, char nome_ficheiro[]){
    FILE * outputrotas;
    ROTA * step;
    SUBROTA * aux;
    outputrotas = fopen(nome_ficheiro, "w");
    if(outputrotas == NULL){
        fprintf(stderr, "ERRO: nao foi possivel escrever ficheiro de output, por favor verificar permissoes!\n");
        exit(-1);
    }
    for(step = lista->prox;step != NULL; step = step->prox){
        fprintf(outputrotas, "#ROTA %d\n", step->numero);
        for(aux = step->lista->prox; aux != NULL; aux = aux->prox)
            fprintf(outputrotas,"%s %g Km\n", aux->itinerario->cidade.nome, aux->distancia);
    }
    fclose(outputrotas);
}

/*--------------------------------------
| Nome: output_rota_unica
| Accao: escreve num ficheiro o output da unica rota indicada pelo -ROTAS
+---------------------------------------*/
void output_rota_unica(ROTA * lista_rotas, char nome_ficheiro[], int numero_rota){
    ROTA * rota_procurada;
    SUBROTA * aux;
    FILE * outputrotas;
    rota_procurada = procurar_rota(lista_rotas, numero_rota);
    if(rota_procurada == NULL){
        fprintf(stderr, "ERRO: a rota indicada para processar pelo -ROTAS nao foi encontrada na lista de rotas. Por favor coloca-la no rotas.txt!\n");
        exit(-1);
    }
    outputrotas = fopen(nome_ficheiro, "w");
    if(outputrotas == NULL){
        fprintf(stderr, "ERRO: nao foi possivel escrever ficheiro de output, por favor verificar permissoes!\n");
        exit(-1);
    }
    fprintf(outputrotas, "#ROTA %d\n", rota_procurada->numero);
    for(aux = rota_procurada->lista->prox; aux != NULL; aux = aux->prox)
        fprintf(outputrotas,"%s %g Km\n", aux->itinerario->cidade.nome, aux->distancia);
    fclose(outputrotas);
}
/*--------------------------------------
| Nome: procurar_rota
| Accao: procura na lista de rotas a rota com o numero especificado
| Return: o pointer do BLOCOCIDADE com a rota do numero indicado. Aqui nao so' vamos mesmo usar para o bloco atual, nao o anterior
| RETURN NULL se a funcao percorrer a lista toda e nao encontrar a rota com esse numero
+---------------------------------------*/
ROTA * procurar_rota(ROTA * lista_rotas, int numero_rota_procurada){
    for (;(lista_rotas != NULL) && (lista_rotas->numero != numero_rota_procurada); lista_rotas = lista_rotas->prox);
    return lista_rotas;
}

/*--------------------------------------
| Nome: libertar_lista_subrotas
| Accao: liberta todos os blocos alocados da lista de subrotas
+---------------------------------------*/
void libertar_lista_subrotas(SUBROTA ** topo){
    SUBROTA * aux;
    while(*topo != NULL){
        aux = *topo;
        *topo = (*topo)->prox;
        free(aux);
        aux = NULL;
    }
}
/*--------------------------------------
| Nome: libertar_lista_rotas
| Accao: liberta todos os blocos alocados da lista de rotas, incluindo subrotas dentro de cada lista
+---------------------------------------*/
void libertar_lista_rotas(ROTA ** topo){
    ROTA * aux;
    while(*topo != NULL){
        aux = *topo;
        *topo = (*topo)->prox;
        libertar_lista_subrotas(&(aux->lista));
        free(aux);
        aux = NULL;
    }
}

/*--------------------------------------
| Nome: intervalo_argc_valido
| Accao: podemos chamar de etapa zero do reconhece_comandos. Verifica em primeiro lugar se o intervalo em que o argc se encontra fica entre 4 e 7, inclusive. Caso tal nao aconteca, assume logo que algo esta' mal e termina o programa
| Como todos os comandos do trabalho ficam com o argc entre 4 e 7, este raciocinio e' valido!
+---------------------------------------*/
void intervalo_argc_valido(int argc){
    if((argc < 4) || (argc > 7)){
        fprintf(stderr, "ERRO: numero incorreto de parametros introduzidos! Apenas existem comandos com 3 a 6 parametros!\n");
        mostrar_utilizacao();
        exit(-1);
    }
}

/*--------------------------------------
| Nome: reconhece_comandos_primeira_etapa
| Accao: dados o argc, o argv e um vetor auxiliar de inteiros, a funcao vai percorrer todos os argumentos dados e procurar: 
| -a presenca ou ausencia dos parametros (os primeiros 6 elementos do vetor vao servir como booleanos ou flags, ou seja, ficam a 0 se nao estiver, a 1 se estiver, estando cada elemento atribuido a um parametro atraves de defines no headertrabalho2.h)
| -se o parametro necessitar de mais "subparametros", verificar que estes podem estar presentes 'a frente do parametro (com a funcao erro_argumentos_agrupados) 
+---------------------------------------*/
void reconhece_comandos_primeira_etapa(int argc, char *argv[], int argumento[NUMERO_PARAMETROS]){
    int i;
    int retorno_sscanf;
    int erro = 0;
    /*Inicia todos os elementos do vetor argumento a zero.*/
    for(i = 0;i < NUMERO_PARAMETROS; i++)
        argumento[i] = 0;
    
    for (i=1; i<argc ; i = i+1){
        if (strcmp(argv[i],"-TL")==0)
            argumento[TL] = 1;

        if (strcmp(argv[i],"-LO")==0){
            argumento[LO] = 1;
            erro = erro + erro_argumentos_agrupados(i, 1, argc);
            LO_POS_NOME_FICHEIRO = i + 1;
        }

        if (strcmp(argv[i],"-ROTAS")==0){
            argumento[ROTAS] = 1;
            /*Como pode ou nao ter algo 'a frente a especificar o numero da rota a processar, nao vai haver verificacao de erros aqui!O if abaixo desempenha essa tarefa*/
            ROTAS_POS_NUMERO_ROTA = i + 1;
            if(ROTAS_POS_NUMERO_ROTA < argc)
                retorno_sscanf = sscanf(argv[ROTAS_POS_NUMERO_ROTA], "%d", &(NUMERO_ROTA));
        }

        if (strcmp(argv[i],"-LR")==0){
            argumento[LR] = 1;
            erro = erro + erro_argumentos_agrupados(i, 1, argc);
            LR_POS_NOME_FICHEIRO = i + 1;
        }

        if (strcmp(argv[i],"-ADI")==0){
            argumento[ADI] = 1;
            erro = erro + erro_argumentos_agrupados(i, 3, argc);
            ADI_POS_NOME = i + 1;
            ADI_POS_LATITUDE = i + 2;
            ADI_POS_LONGITUDE = i + 3;
        }

        if (strcmp(argv[i],"-REM")==0){
            argumento[REM] = 1;
            erro = erro + erro_argumentos_agrupados(i, 1, argc);
            REM_POS_NOME = i + 1;
        }
    }

    /*Ao fim do loop, verificar se houve pelo menos um erro. Se houver, o programa mostra a utilizacao e termina*/
    if(erro > 0){
        mostrar_utilizacao();
        exit(-1);
    }
    /*Verificar se, caso seja o comando 4 especificando a rota, o programa conseguiu ler o numero da rota a processar da linha de comandos*/
    if(FLAGS_ON3(LO, ROTAS, LR) && (argc == 7) && (retorno_sscanf == 0)){
        fprintf(stderr, "-ROTAS: ERRO: para 6 parametros, espera-se que o -ROTAS (comando 4 do enunciado) tenha 'a sua frente um numero de rota especificado, no entanto nao foi possivel encontra-lo!\n");
        exit(-1);
    }
}

/*--------------------------------------
| Nome: erro_argumentos_agrupados
| Accao: Protege o programa do caso em que argumentos como o -ADI, que exigem de seguida varios parametros, nao os tenham, impedindo que o programa procure elementos do argv de índice igual ou superior ao argc!
| Return: 1 se houver erro. 0 se estiver tudo bem!
+---------------------------------------*/
int erro_argumentos_agrupados(int posicao_argumento, int numero_argumentos_seguidos, int argc){
    int erro = 0;
    if((posicao_argumento + numero_argumentos_seguidos) > argc){
        fprintf(stderr, "ERRO: ultimo argumento indicado exige de seguida mais %d parametros para o definir na totalidade\n", numero_argumentos_seguidos);
        erro = 1;
    }
    return erro;
}

/*--------------------------------------
| Nome: reconhece_comandos_segunda_etapa
| Accao: segunda etapa, verifica e recolhe o conteudo dos "subparametros", aqueles dos quais parametros como o -ADI dependem (nome a adicionar, latitude, longitude) e se os mesmos sao validos
| Observacao: esta funcao nao recolhe o numero da rota a processar, caso este exista 'a frente do -ROTAS, isso ja' foi feito na primeira etapa por ser mais conveniente
| Return: CIDADE auxiliar, que vem ou com o nome da localidade a remover (-REM), ou com o nome e as coordenadas da localidade a adicionar (-ADI)
+---------------------------------------*/
CIDADE reconhece_comandos_segunda_etapa(char * argv[], int argumento[NUMERO_PARAMETROS]){
    int erro = 0;
    CIDADE cidade_aux = atribuir_cidade("Null_Island", 0, 0);

    if(argumento[LO] && argv[LO_POS_NOME_FICHEIRO][0] == '-'){
        fprintf(stderr,"-LO: ERRO: faltou fornecer o nome do ficheiro de output depois do parametro -LO!O nome do ficheiro nao deve comecar com '-'!\n");
        erro = 1;
    }

    if(argumento[LR] && argv[LR_POS_NOME_FICHEIRO][0] == '-'){
        fprintf(stderr,"-LR: ERRO: faltou fornecer o nome do ficheiro de output depois do parametro -LR!O nome do ficheiro nao deve comecar com '-'!\n");
        erro = 1;
    }

    if(argumento[ADI])
        strcpy(cidade_aux.nome, argv[ADI_POS_NOME]);
    
    if(argumento[ADI] && !sera_letra(argv[ADI_POS_NOME][0])){
        fprintf(stderr,"-ADI: ERRO: faltou fornecer o nome da cidade a adicionar depois do -ADI!O nome de uma cidade comeca com uma letra!\n");
        erro = 1;
    }

    if(argumento[ADI] && (!SSCANF_LATITUDE || !SSCANF_LONGITUDE)){
        fprintf(stderr,"-ADI: ERRO: faltou fornecer valores das coordenadas da cidade a adicionar!Parametros do -ADI:\n-ADI <nome> <latitude> <longitude>\nA latitude e a longitude devem ser numeros reais!\n");
        erro = 1;
    }

    if(argumento[REM])
        strcpy(cidade_aux.nome, argv[REM_POS_NOME]);
    
    if(argumento[REM] && !sera_letra(argv[REM_POS_NOME][0])){
        fprintf(stderr,"-REM: ERRO: faltou fornecer o nome da cidade a remover depois do -REM!O nome de uma cidade comeca com uma letra!\n");
        erro = 1;
    }
    /*Ao fim destas condicoes todas, verificar se houve pelo menos um erro. Se houve, o programa mostra a utilizacao e termina*/
    if(erro){
        mostrar_utilizacao();
        exit(-1);
    }

    return cidade_aux;
}

/*--------------------------------------
| Nome: reconhece_comandos_terceira_etapa
| Accao: ultima etapa: para o argc dado, verificar se foram usadas apenas combinacoes de parametros validas, ou seja, se os parametros combinados formam um dos comandos indicados no enunciado
| comandos em funcao do argc:
| argc == 4->comando 1) do enunciado:./processa_rotas -LO output1.txt -TL
| argc == 5->comando 3) do enunciado:./processa_rotas -LO output1.txt -REM <nome>
| argc == 6->comando 4) do enunciado, sem especificar rota:./processa_rotas -LO output1.txt -LR output2.txt -ROTAS
| argc == 7->comando 4) do enunciado, especificando rota ou comando 2) do enunciado:./processa_rotas -LO output1.txt -ADI <nome> <latitude> <longitude>
+---------------------------------------*/
void reconhece_comandos_terceira_etapa(int argc, int argumento[NUMERO_PARAMETROS]){
    int erro = 0;
    switch(argc){
        case 4:
            if(FLAGS_OFF2(TL, LO)){
                fprintf(stderr, "ERRO: parametros incorretos! Para %d parametros espera-se apenas o uso de -LO com -TL!\n", argc - 1);
                erro = 1;
            }
            break;
        case 5:
            if(FLAGS_OFF2(LO, REM)){
                fprintf(stderr, "ERRO: parametros incorretos! Para %d parametros espera-se apenas o uso de -LO com -REM!\n", argc - 1);
                erro = 1;
            }
            break;
        case 6:
            if(FLAGS_OFF3(LO, ROTAS, LR)){
                fprintf(stderr, "ERRO: parametros incorretos! Para %d parametros espera-se apenas o uso de -LO, -ROTAS sem especificar rota e -LR!\n", argc - 1);
                erro = 1;
            }
            break;
        case 7:
            if(FLAGS_OFF3(LO, ROTAS, LR) && FLAGS_OFF2(LO, ADI)){
                fprintf(stderr, "ERRO: parametros incorretos! Para %d parametros espera-se ou o uso de -LO, -LR e -ROTAS especificando a rota ou o uso de -LO e -ADI!\n", argc - 1);
                erro = 1;
            }
            break;
    }
    if(erro){
        mostrar_utilizacao();
        exit(-1);
    }
}