#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definições */
#define ORDEM 4
#define TAM_HEADER 20 

#define MAX_NOME 30
#define MAX_QUANTIDADE 100
#define TAM_CHAVE 5
#define TAM_REGISTRO 192

#define NOME_ARQ_DADOS "../data/movies.dat"
#define NOME_INDICE_PRIMARIO "../data/arvore.idx"
#define NOME_INDICE_TITULO "../data/ititles.idx"

/* Definição de estruturas */
typedef struct{
    char chavePrimaria[TAM_CHAVE + 1];
    char tituloPortugues[MAX_NOME + 1];
    char tituloOriginal[MAX_NOME + 1];
    char diretor[MAX_NOME + 1];
    char anoLancamento[5];
    char pais[MAX_NOME + 1];
    char nota;
} Filme;

typedef struct{
    long RRN;
    int eFolha;
    char chaves[ORDEM][TAM_CHAVE+1]; //array de chaves
    long dadosRRN[ORDEM];
    long filhos[ORDEM + 1];
    int numChaves;
    int pai;
    long prox;
} No;

typedef struct{
    char titulo[MAX_NOME + 1];
    char chavePrimaria[TAM_CHAVE + 1]; 
} IndiceSecundario;

#endif //STRUCTS_H