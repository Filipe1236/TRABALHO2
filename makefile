#Parametros do gcc
CC = gcc
CFLAGS = -g -Wall -Werror -pedantic -ansi

#Nome do programa
TARGET = processa_rotas

#Ficheiros objeto para construir o executavel:
OBJS = main.o trabalho2.o

#Construcao do executavel:
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

#Dependencias de cada ficheiro .o
main.o: main.c headertrabalho2.h
	$(CC) $(CFLAGS) -c main.c -o main.o

trabalho2.o: trabalho2.c headertrabalho2.h
	$(CC) $(CFLAGS) -c trabalho2.c -o trabalho2.o

#Limpeza
clean:
	rm *.o $(TARGET)