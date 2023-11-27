/* Biblioteca para manipulação do arquivo de dados */
#ifndef DATAFILEMANIPULATION_H
#define DATAFILEMANIPULATION_H

#include "structs.h"

/* Função que verifica se o arquivo de dados existe. Retorna 0 se não existir e 1 se existir*/
int verificaDados();

/* Insere registro no arquivo de dados. Retorna o byte-offset se a operação teve sucesso e 0 caso não*/
int insereArquivoDados(Filme *);

/* A partir do RRN obtido pela chave primária, lê registro e returna struct com dados do filme */
Filme *leFilmeChavePrimaria(long);

/* Altera registro no arquivo de dados. Retorna 1 se a operação teve sucesso e 0 caso não*/
int alteraRegistro(char, char *);

/* Chama função de atualizar índice secundário e libera as memórias alocadas*/
void sair();

/* Atualiza dados do índice secundário da RAM para o disco. Atualiza flag da header*/
void atualizaIndice();

#endif //DATAFILEMANIPULATION_H