int main(int argc, char * argv[]){
    int i;
    char LO_nome_ficheiro_output [MAX];
    char LR_nome_ficheiro_output [MAX];
    char REM_localidade_removida [MAX];
    CIDADE ADI_localidade_adicionada;
    int ROTAS_numero_rota = SEM_ROTA;
    /*int aux;*/

    if((argc == 1) || argc > MAXIMO_DE_ARGUMENTOS){
        fprintf(stderr,"ERRO: argumentos fornecidos incorretos!\n");
        mostrar_utilizacao();
        exit(-1);
    }

    for(i = 1;i < argc;i++){

        if(strcmp(argv[i],"-TL") == 0)
            printf("-TL encontrado!\n");

        if(strcmp(argv[i],"-LO") == 0){
            protege_argumentos_agrupados_do_fim(i,1,argc);
            strcpy(LO_nome_ficheiro_output, argv[i+1]);
        }

        if(strcmp(argv[i], "-ADI") == 0){
            protege_argumentos_agrupados_do_fim(i, 3, argc);
            strcpy(ADI_localidade_adicionada.nome, argv[i + 1]);
            sscanf(argv[i + 2], "%f", &(ADI_localidade_adicionada.latitude));
            sscanf(argv[i + 3], "%f", &(ADI_localidade_adicionada.longitude));
            validar_coordenadas(ADI_localidade_adicionada);
        }

        if(strcmp(argv[i],"-REM") == 0){
            protege_argumentos_agrupados_do_fim(i, 1, argc);
            strcpy(REM_localidade_removida, argv[i + 1]);
        }
        /*
        if(strcmp(argv[i],"-ROTAS") == 0){
            sscanf(argv[i + 1], "%d", &ROTAS_numero_rota);
        }
        
        if(strcmp(argv[i],"-LR") == 0){
            strcpy(LR_nome_ficheiro_output, argv[i+1]);
        }
        */
    }
    return 0;
}