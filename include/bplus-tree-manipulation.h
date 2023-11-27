/* Biblioteca para manipulação do arquivo da árvore B+ */
#ifndef BPLUSTREEMANIPULATION_H
#define BPLUSTREEMANIPULATION_H

#include <math.h>
#include "structs.h"

/* Carrega raíz para memória principal */
void carregaRaiz();

/* Cria nó vazio */
No *criaNo();

/* Armazena novo nó da árvore no arquivo */
long armazenaNo(No *);

/* A partir do RRN, lê nó da arvore do arquivo */
No *carregaNo(long RRN);

/* Busca nó que contém a chave */
No *buscaNo(char*);

void buscaRange(char*, char*);

/* Insere novo nó na arvore, considerando a chave primária do filme */
void insereNo(char*, long);

/* Insere nova chave no nó folha */
void insereNoFolha(No *, char*, long);

/* Após overflow, insere chave promovida no nó pai, nó interno da árvore*/
void insereNoPai(No*, char*, No*);

/* atualiza o RRN da raíz no arquivo que mantém a árvore */
void salvaRaiz();

#endif //BPLUSTREEMANIPULATION_H