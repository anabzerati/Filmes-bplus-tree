/* Biblioteca para manipulação do arquivo de dados */
#ifndef DATAFILEMANIPULATION_H
#define DATAFILEMANIPULATION_H

#include "structs.h"

/* Função que verifica se o arquivo de dados existe. Retorna 0 se não existir e 1 se existir*/
int verificaDados();

/* Carrega índice secunsário do disco para RAM, retorna vetor de índices ordenado*/
IndiceSecundario *carregaSecundario();

/* Refaz indice secundário a partir do arquivo de dados*/
void criaSecundario(FILE *, FILE *);

/* Insere registro no arquivo de dados e nos vetores de índices. Retorna 1 se a operação teve sucesso e 0 caso não*/
int insereRegistro(Filme *);
// TODO inserir árvore

//insereRegistroDados(Filme *);

/* Altera registro no arquivo de dados. Retorna 1 se a operação teve sucesso e 0 caso não*/
int alteraRegistro(char, char *);

/* Ordena o vetor de índices secundários considerando o título. Insertion Sort*/
void ordenaSecundario();

/* Busca filme pelo índice secundário a partir do título e retorna os indices da primeira e útlima aparição dessa chave secundária. Caso não haja registro com esse ID, retorna -1 em j*/
void buscaSecundaria( char *titulo, int *, int *);

/* Atualiza dados dos índices da RAM para o disco. Atualiza flag da header*/
void atualizaIndices();

/* Chama função de atualizar índices e libera as memórias alocadas*/
void sair();

#endif //DATAFILEMANIPULATION_H