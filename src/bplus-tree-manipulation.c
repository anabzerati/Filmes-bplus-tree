/* Implementação das funções definidas na biblioteca */

#include "../include/bplus-tree-manipulation.h"

extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;


// TODO ler nó/página
/* A partir do índice primário, lê os dados do filme correspondente e retorna uma struct preenchida com as informações
Filme *leFilmeIndicePrimario(int pos){
    char buffer[TAM_REGISTRO + 1];
    Filme *auxF = malloc(sizeof(Filme));

    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");

    fseek(dadosp, vetorPrimario[pos].RRN, SEEK_SET); //posiciona ponteiro
    fgets(buffer, TAM_REGISTRO, dadosp); //lê registro

    //leitura formatada da string, atribuindo os campos do registro às variáveis da struct
    if (sscanf(buffer, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%c", auxF->chavePrimaria, auxF->tituloOriginal, auxF->tituloPortugues, auxF->diretor, auxF->anoLancamento, auxF->pais, &auxF->nota) != 7) {
        printf("Erro ao ler os campos.\n");
    } 

    fclose(dadosp);

    return auxF;
}
*/
