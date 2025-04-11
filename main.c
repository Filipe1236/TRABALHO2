/*----------------------------------------------------------------------
TRABALHO 2 DE PROGRAMAÇÃO 2024/2025
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
#include "trabalho2.c"

int main(int argc, char * argv[]){
    BLOCOCIDADE * lista_cidades;
    ROTA * lista_rotas = NULL;
    int argumento[NUMERO_PARAMETROS];
    CIDADE cidade_ADI_REM;
    /*------------instrucoes que sao comuns a todos os comandos-------------------*/
    reconhece_comandos_primeira_etapa(argc, argv, argumento);
    cidade_ADI_REM = reconhece_comandos_segunda_etapa(argv, argumento);
    lista_cidades = TL_ler_cidades(argumento[TL]);
    
    switch(argc){
        case 4:
            /*verifica se os parametros estao incorretos para o argc*/
            if(!argumento[LO] || !argumento[TL])
                explica_relacao_argc_comando(argc);
            /*Se os parametros forem so' o -LO e o -TL, executa o comando. Nao ha' risco de haver um terceiro parametro, olhar para as funcoes reconhece_comandos*/
            else
                LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
            break;

        case 5:
            if(argumento[LO] && argumento[REM]){
                remover_da_listacidades(lista_cidades, cidade_ADI_REM.nome);
                LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
            }
            else
                explica_relacao_argc_comando(argc);
            break;

        case 6:
            if(argumento[LO] && argumento[ROTAS] && argumento[LR]){
                LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
                lista_rotas = ciclo_leitura_rotas(lista_cidades);
                output_lista_rotas(lista_rotas, argv[LR_POS_NOME_FICHEIRO]);
            }
            else
                explica_relacao_argc_comando(argc);
            break;
        
        case 7:
            if(argumento[LO] && argumento[ADI]){
                insere_na_lista_cidades_alfabeticamente(lista_cidades, cidade_ADI_REM, CONTADOR_LINHA_ADI, argumento[TL]);
                LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
            }
            else if(argumento[LO] && argumento[ROTAS] && argumento[LR]){
                LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
                lista_rotas = ciclo_leitura_rotas(lista_cidades);
                processar_rota_unica(lista_rotas, argv[LR_POS_NOME_FICHEIRO], NUMERO_ROTA);
            }
            else
                explica_relacao_argc_comando(argc);
            break;
    }
    /*libertar memoria!*/
    libertar_lista(&lista_cidades);
    libertar_lista_rotas(lista_rotas);
    return 0;
}
