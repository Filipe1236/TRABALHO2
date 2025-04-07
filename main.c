#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "headertrabalho2.h"
#include "trabalho2.c"

/*ORIGINAL ESTA EM TRABALHO_V2!!!*/

int main(int argc, char * argv[]){
    BLOCOCIDADE * lista;
    CIDADE porto, braga;
    porto = atribuir_cidade ("Porto",41.1495, -8.6108);
    braga = atribuir_cidade("Braga", 41.5333, -8.4167);
    printf("Distancia em x= %f KM\n", distancia_em_x(braga, porto));
    printf("Distancia em y= %f KM\n", distancia_em_y(braga, porto));
    printf("Distancia = %f KM\n", distancia_entre_cidades(braga, porto));
    lista = TL_ler_cidades();
    LO_escrever_cidades("outputcidades.txt", lista);
    mostrar_lista_cidades(lista);
    libertar_lista(&lista);
    
    return 0;
}
