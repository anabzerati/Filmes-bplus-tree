/* Implementação das funções definidas na biblioteca */

#include "../include/datafile-manipulation.h"
#include "../include/bplus-tree-manipulation.h"

extern IndiceSecundario *vetorTitulos;
extern No *raiz;
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

int insereArquivoDados(Filme *novoFilme){
    char buffer[TAM_REGISTRO + 1];

    FILE *dadosp = fopen(NOME_ARQ_DADOS, "a+");

    if(!dadosp){
        perror("Erro ao abrir o arquivo de dados");
        return -1;
    }

    // "concatenar" várias strings de uma vez - formatar string com @ entre os campos
    sprintf(buffer,"%s@%s@%s@%s@%s@%s@%c@", novoFilme->chavePrimaria, novoFilme->tituloOriginal, novoFilme->tituloPortugues, 
        novoFilme->diretor, novoFilme->anoLancamento, novoFilme->pais, novoFilme->nota);
    
    // preencher o resto do registro com '#'
    memset(buffer + strlen(buffer), '#', TAM_REGISTRO - strlen(buffer));
    buffer[TAM_REGISTRO] = '\0';

    // escrever registro no arquivo de dados
    fputs(buffer, dadosp);

    // após inserir o registro no arquivo de dados, é necessário adicioná-lo aos arquivos de índices
    numeroFilmes++;

    long auxRRN = ftell(dadosp) - TAM_REGISTRO; //posição onde inseriu

    //fclose (dadosp);

    return auxRRN; 
}

Filme *leFilmeChavePrimaria(long RRN){
    FILE *fp = fopen(NOME_ARQ_DADOS, "r+");
    char buffer[TAM_REGISTRO + 1];
    Filme *auxF = malloc(sizeof(Filme));

    if(! fp){
        perror("Erro ao abrir o arquivo");
    }

    fseek(fp, RRN, SEEK_SET);
    fgets(buffer, TAM_REGISTRO, fp); //lê registro

    //leitura formatada da string, atribuindo os campos do registro às variáveis da struct
    if (sscanf(buffer, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%c", auxF->chavePrimaria, auxF->tituloOriginal, auxF->tituloPortugues, auxF->diretor, auxF->anoLancamento, auxF->pais, &auxF->nota) != 7) {
        perror("Erro ao ler os campos.\n");
    } 

    fclose(fp);

    return auxF;
}

/* Altera registro no arquivo de dados. Retorna 1 se a operação teve sucesso e 0 caso não */
int alteraRegistro(char novaNota, char *idFilme){
    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");

    if(!dadosp){
        perror("Erro ao abrir o arquivo para alteração");
        return 0;
    }

    No *encontrado = buscaNo(idFilme); // busca nó que contém chave

    int i;
    for(i = 0; i < encontrado->numChaves; i ++){
        if(! strcmp(encontrado->chaves[i], idFilme)){ // busca chave
            break;
        }
    }

    fseek(dadosp, encontrado->dadosRRN[i], SEEK_SET); //posiciona ponteiro

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

    return 1;
}

/* Chama função de atualizar índices e libera as memórias alocadas
void sair(){
    atualizaIndices();

    free(vetorPrimario);
    free(vetorTitulos);
}
*/

/* Chama função de atualizar índices e libera as memórias alocadas*/
void sair(){
    FILE *primario = fopen(NOME_INDICE_PRIMARIO, "rb+");

    fprintf(primario, "%020ld\n", raiz->RRN); 

    atualizaIndices();
}

/* Atualiza dados dos índices da RAM para o disco. Atualiza flag da header*/
void atualizaIndices(){
    int i;

    //arquivo de índice secundário
    FILE *secundario =  fopen(NOME_INDICE_TITULO, "w");
    fprintf(secundario, "%d %d\n", 1, numeroFilmes); //flag e quantidade de registros 

    for(i = 0; i < numeroFilmes; i++){ //para cada registro, imprime os campos de chave primária e RRN
        fprintf(secundario, "%s@%s\n", vetorTitulos[i].titulo, vetorTitulos[i].chavePrimaria);
    }  
}
