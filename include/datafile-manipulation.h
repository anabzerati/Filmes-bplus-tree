/* Biblioteca para manipulação do arquivo de dados */
#ifndef DATAFILEMANIPULATION_H
#define DATAFILEMANIPULATION_H

#include "structs.h"

/* Função que verifica se o arquivo de dados existe. Retorna 0 se não existir e 1 se existir*/
int verificaDados();

/* Insere registro no arquivo de dados. Retorna o byte-offset se a operação teve sucesso e 0 caso não*/
int insereArquivoDados(Filme *);

Filme *imprimeFilmeChavePrimaria(long);

/* Altera registro no arquivo de dados. Retorna 1 se a operação teve sucesso e 0 caso não*/
int alteraRegistro(char, char *);

/* Chama função de atualizar índices e libera as memórias alocadas*/
void sair();

#endif //DATAFILEMANIPULATION_H