/* Biblioteca com funções de interação com o usuário ao projeto */
#ifndef INTERACAO_H
#define INTERACAO_H

#include "structs.h"
#include "datafile-manipulation.h"
#include "secondaryindex-manipulation.h"
#include "bplus-tree-manipulation.h"
#include <ctype.h>

/* Lê dados do filme inseridos pelo usuário e retorna uma struct preenchida */
Filme *leDadosFilme();

/* Recebe uma struct com dados do filme e os imprime na tela*/
void imprimeFilme(Filme *);

/* Faz o intermédio entre ler os dados do filme e chamar a função de inserir */
void insercao();

/* Lê dados para a busca, chama a função que a realiza e, por fim, caso seja encontrado um filme, lê seus dados do arquivo de dados e imprime na tela. Caso não encontre, imrpime mensagem de erro*/
void busca();

/* Lê dados necessários e edita nota de um filme, no arquivo de dados, a partir de sua chave primária. Verifica sucesso e imprime uma mensagem na tela*/
void editarNota();

/* Percorre o arquivo de dados (seguindo a ordem alfabética de diretores), lendo o registro para RAM e imprimindo na tela suas informações. Permite busca em range */
void listarFilmes();

#endif //INTERACAO_H