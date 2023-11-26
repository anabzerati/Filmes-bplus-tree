/* Biblioteca para manipulação do arquivo da árvore B+ */
#ifndef BPLUSTREEMANIPULATION_H
#define BPLUSTREEMANIPULATION_H

/*typedef struct{
    int RRN;
    int eFolha;
    char chaves[ORDEM][TAM_CHAVE+1]; //array de chaves
    int dadosRRN[ORDEM];
    int filhos[ORDEM];
    int numChaves;
    int pai;
    int prox;
} No;*/

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

void salvaRaiz();

long verificaFinalArquivo();

/* A partir do índice primário, lê os dados do filme correspondente e retorna uma struct preenchida com as informações*/
//Filme *leFilmeIndicePrimario(int);
// TODO ler nó/página

// inserção

//busca

#endif //BPLUSTREEMANIPULATION_H