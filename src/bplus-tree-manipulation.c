/* Implementação das funções definidas na biblioteca */

#include "../include/bplus-tree-manipulation.h"

// "importando" variáveis globais da main
extern No* raiz;
extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Cria nó vazio */
No *criaNo(){
    int i;
    No *novo = malloc(sizeof(No)); // aloca espaço

    for(i = 0; i < ORDEM; i ++){
        strcpy(novo->chaves[i], "NULL");
        novo->dadosRRN[i] = -1;
        novo->filhos[i] = -1;
    }

    novo->eFolha = -1;
    novo->numChaves = 0;
    novo->pai = -1;
    novo->prox = -1;
    novo-> RRN = -1;

    return novo;
}

/* Armazena novo nó da árvore no arquivo */
void armazenaNo(No *novoNo){
    printf("CHEGOU");

    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "a+");
    if(!fp){
        perror("Não foi possível abrir o arquivo");
        return;
    }

    fwrite(novoNo, sizeof(No), 1, fp);

    fclose(fp);
}

/* A partir do RRN, lê nó da arvore do arquivo */
No *carregaNo(long RRN){
    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "r+");

    if(!fp){
        perror("Não foi possível abrir o arquivo");
        return NULL;    
    }
    
    fseek(fp, RRN, SEEK_SET);

    No *noLido = malloc(sizeof(No));
    fread(noLido, sizeof(No), 1, fp);

    fclose(fp);

    return noLido;
}

/* Busca nó que contém a chave */
No *buscaNo(char* chave){
    No *noAtual = raiz; // inicia busca pela raiz

    while(! noAtual->eFolha){
        for(int i=0; i < noAtual->numChaves; i ++){ //percorre todas as chaves
            if(! strcmp(chave, noAtual->chaves[i])){ // se achou a chave
                noAtual = carregaNo(noAtual->filhos[i + 1]); //carrega filho à direita
                break;
            } else if(chave < noAtual->chaves[i]){
                noAtual = carregaNo(noAtual->filhos[i]); //carrega filho à esquerda
                break;
            } else if(i + 1 == noAtual->numChaves){
                noAtual = carregaNo(noAtual->filhos[i + 1]); //carrega último filho
                break;
            }
        }
    }

    return noAtual;
}

/* Insere novo nó na arvore, considerando a chave primária do filme */
void insereNo(char*, long);

/* Insere nova chave no nó folha */
void insereNoFolha(No *folha, char*chave, long RRNChaveDados){
    if(folha->numChaves == 0){ //folha vazia
        strcpy(folha->chaves[0], chave); // chave na primeira posição
        folha->dadosRRN[0] = RRNChaveDados;
        folha->numChaves++;
    } else{
        for(int i = 0; i < folha->numChaves -1; i ++){
            if(chave == folha->chaves[i]){ //chave já foi inserida
                perror("Chave já inserida"); // TODO verificar
                break;
            } else if(chave < folha->chaves[i]){ //encontrada posição onde inserir
                // reposiciona chaves para direita
                for(int j = folha->numChaves; j > i; j --){
                    strcpy(folha->chaves[j], folha->chaves[j-1]);
                    folha->dadosRRN[j] = folha->dadosRRN[j-1];
                }

                // adiciona chave
                strcpy(folha->chaves[i], chave);
                folha->dadosRRN[i] = RRNChaveDados;

                break;
            } else if(i + 1 == folha->numChaves){ //adicion chave na última posição
                strcpy(folha->chaves[i+1], chave);
                folha->dadosRRN[i+1] = RRNChaveDados;
            }
        }
    }
}

/* Após overflow, insere chave promovida no nó pai, nó interno da árvore*/
void insereNoPai(No*, char*, No*);


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
