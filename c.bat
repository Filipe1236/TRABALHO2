::VERSAO PARA COMPILAR EM WINDOWS!!!
gcc -c -Wall -pedantic -ansi trabalho2.c
gcc -c -Wall -pedantic -ansi main.c
gcc main.o trabalho2.o -o processa_rotas.exe
.\processa_rotas.exe