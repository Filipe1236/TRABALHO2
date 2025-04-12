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

int main(int argc, char * argv[]){
    BLOCOCIDADE * lista_cidades;
    ROTA * lista_rotas = NULL;
    int argumento[NUMERO_PARAMETROS];
    CIDADE cidade_por_adicionar_remover;
    /*------------instrucoes que sao comuns a todos os comandos-------------------*/
    intervalo_argc_valido(argc);
    reconhece_comandos_primeira_etapa(argc, argv, argumento);
    cidade_por_adicionar_remover = reconhece_comandos_segunda_etapa(argv, argumento);
    reconhece_comandos_terceira_etapa(argc, argumento);
    lista_cidades = TL_ler_cidades(argumento[TL]);
    /*-------------instrucoes de cada comando--------------------------*/
    /*Comando 1 do enunciado ./processa_rotas -LO output1.txt -TL
    (os testes do TL ja sao feitos no TL_ler_cidades ao passar 1 como argumento, nos restantes sem TL, passa-se 0 pela funcao, deixando de haver mensagens de avisos/erros)*/
    if(FLAGS_ON2(LO, TL))
        LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
    /*Comando 2 do enunciado ./processa_rotas -LO output1.txt -ADI <nome> <latitude> <longitude>*/
    if(FLAGS_ON2(LO, ADI) && !validar_coordenadas(cidade_por_adicionar_remover)){
        insere_na_lista_cidades_alfabeticamente(lista_cidades, cidade_por_adicionar_remover, CONTADOR_LINHA_ADI, argumento[TL]);
        LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
    }

    /*Comando 3 do enunciado ./processa_rotas -LO output1.txt -REM <nome>*/
    if(FLAGS_ON2(LO, REM)){
        remover_da_listacidades(lista_cidades, cidade_por_adicionar_remover.nome);
        LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
    }

    /*Comando 4 do enunciado ./processa_rotas -LO output1.txt -LR output2.txt -ROTAS <nome>
    O argc foi aproveitado para fazer a distincao entre o comando em que se especifica a rota e o que nao especifica*/
    if(FLAGS_ON3(LO, ROTAS, LR)){
        LO_escrever_cidades(argv[LO_POS_NOME_FICHEIRO], lista_cidades);
        lista_rotas = ciclo_leitura_rotas(lista_cidades);
        if(argc == 6)
            output_lista_rotas(lista_rotas, argv[LR_POS_NOME_FICHEIRO]);
        if(argc == 7)
            output_rota_unica(lista_rotas, argv[LR_POS_NOME_FICHEIRO], NUMERO_ROTA);
    }

    /*libertar memoria!*/
    libertar_lista_cidades(&lista_cidades);
    libertar_lista_rotas(&lista_rotas);
    return 0;
}
