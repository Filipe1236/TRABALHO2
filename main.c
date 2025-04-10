#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "headertrabalho2.h"
#include "trabalho2.c"

/*ORIGINAL ESTA EM TRABALHO_V2!!!*/

int main(int argc, char * argv[]){
    BLOCOCIDADE * lista_cidades;
    ROTA * lista_rotas;
    lista_cidades = TL_ler_cidades();
    remover_da_listacidades(lista_cidades, "Leiria");
    LO_escrever_cidades("outputcidades.txt", lista_cidades);
    mostrar_lista_cidades(lista_cidades);
    lista_rotas = ciclo_leitura_rotas(lista_cidades);
    mostrar_lista_rotas(lista_rotas);
    libertar_lista(&lista_cidades);
    
    return 0;
}
