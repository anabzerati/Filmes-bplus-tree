/* Biblioteca para manipulação do arquivo de índice secundário */
#ifndef SECONDARYINDEXMANIPULATION_H
#define SECONDARYINDEXMANIPULATION_H

#include "structs.h"
#include "datafile-manipulation.h"

/* Função que verifica se o índice secundário existe. Retorna 0 se não existir e 1 se existir*/
int verificaSecundario();

/* Insere registro no vetor de índice secundário */
int insereIndiceSecundario(Filme *);

/* Remove posição do vetor de índice secundário a partir de sua posição, readequando as posições dos demais elementos. Retorna 1 se a operação teve sucesso e 0 caso não*/
int removePosicaoSecundario(int pos);

/* Carrega índice secunsário do disco para RAM, retorna vetor de índices ordenado*/
void carregaSecundario();

/* Refaz indice secundário a partir do arquivo de dados*/
void criaSecundario(FILE *, FILE *);

/* Ordena o vetor de índices secundários considerando o título. Insertion Sort*/
void ordenaSecundario();

/* Busca filme pelo índice secundário a partir do título e retorna os indices da primeira e útlima aparição dessa chave secundária. Caso não haja registro com esse ID, retorna -1 em j*/
void buscaSecundaria( char *titulo, int *, int *);

/* Atualiza dados dos índices da RAM para o disco. Atualiza flag da header*/
void atualizaIndices();

#endif //SECONDARYINDEXMANIPULATION_H