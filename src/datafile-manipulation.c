/* Implementação das funções definidas na biblioteca */

#include "../include/datafile-manipulation.h"

extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Função que verifica se o arquivo de dados existe. Retorna 0 se não existir e 1 se existir*/
int verificaDados(){
    FILE *fp;

    if((fp = fopen(NOME_ARQ_DADOS, "r+")) == NULL){ //não existe arquivo;
        return 0;
    }

    free(fp);
    return 1;
}


/* Altera registro no arquivo de dados. Retorna 1 se a operação teve sucesso e 0 caso não
int alteraRegistro(char novaNota, char *idFilme){
    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");

    if(!dadosp){
        printf("Erro ao abrir o arquivo para alteração");

        return 0;
    }

    int pos = buscaPrimaria(idFilme, 0, numeroFilmes); //identifica filme pela chave primária

    fseek(dadosp, vetorPrimario[pos].RRN, SEEK_SET); //posiciona ponteiro

    char buffer[TAM_REGISTRO + 1], tituloaux[MAX_NOME + 1], tituloportaux[MAX_NOME + 1], diretoraux[MAX_NOME + 1], anoaux[5], paisaux[MAX_NOME + 1]; //strings auxiliares para leitura de campos

    fgets(buffer, TAM_REGISTRO + 1, dadosp); //lê registro

    if (sscanf(buffer, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", idFilme, tituloaux, tituloportaux, diretoraux, anoaux, paisaux) != 6) { //lê campos
        printf("Erro ao ler os campos.\n");
        return 0;
    } 

    long int tamNota;
    tamNota = strlen(idFilme) + strlen(tituloaux) + strlen(tituloportaux) + strlen(diretoraux) + strlen(anoaux) + strlen(paisaux) + 6; //tamanho de cada campo + separadores

    fseek(dadosp, tamNota - TAM_REGISTRO , SEEK_CUR); //posiciona o ponteiro no campo da nota

    fputc(novaNota, dadosp); //atualiza a nota

    fclose(dadosp);

    return 1;
}

/* Chama função de atualizar índices e libera as memórias alocadas
void sair(){
    atualizaIndices();

    free(vetorPrimario);
    free(vetorTitulos);
}
*/