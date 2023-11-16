/* Implementação das funções definidas na biblioteca */

#include "../include/bplus-tree-manipulation.h"

// "importando" variáveis globais da main
extern No* raiz;
extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Carrega raíz para memória principal */
void carregaRaiz(){
    long rrnRaiz;
    FILE *fp; 
    
    fp = fopen(NOME_INDICE_PRIMARIO, "rb+");

    if(! fp){ //arquivo não existe
        fp = fopen(NOME_INDICE_PRIMARIO, "wb+"); //cria arquivo
        fprintf(fp, "0\n"); // rrn da raíz

        raiz = criaNo(); //nó vazio
        raiz->eFolha = 1;

        fclose(fp);

        return;
    }

    //leitura do header
    fscanf(fp, "%ld", &rrnRaiz); //RRN da raíz

    if(rrnRaiz == -1){ //arquivo vazio
        fprintf(fp, "0\n"); // rrn da raíz

        raiz = criaNo(); //no vazio
        raiz->eFolha = 1;
    } else{
        raiz = carregaNo(rrnRaiz);
    }

    fclose(fp);
}

/* Cria nó vazio */
No *criaNo(){
    int i;
    No *novo = malloc(sizeof(No)); // aloca espaço

    // inicializa vetores como vazios
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
long armazenaNo(No *novoNo){
    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "rb+");
    fseek(fp, 0, SEEK_END);

    if(!fp){
        perror("Não foi possível abrir o arquivo");
        return -1;
    }

    long byteoffset;

    if(novoNo->RRN == -1){ // novo nó da árvore
        byteoffset = ftell(fp); // escreve após último registro
        novoNo->RRN = byteoffset;
    } else{ //sobrescrever nó já existente
        byteoffset = novoNo->RRN;
        fseek(fp, byteoffset, SEEK_SET); //posiciona ponteiro
    }

    fwrite(novoNo, sizeof(No), 1, fp);

    fclose(fp);

    return byteoffset;
}

/* A partir do RRN, lê nó da arvore do arquivo */
No *carregaNo(long RRN){
    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "rb+");

    if(!fp){
        perror("Não foi possível abrir o arquivo");
        return NULL;    
    }
    
    fseek(fp, RRN, SEEK_SET);

    No *noLido = malloc(sizeof(No)); //alocar espaço em memória
    fread(noLido, sizeof(No), 1, fp);

    fclose(fp);

    return noLido;
}

/* Busca nó que contém a chave */
No *buscaNo(char* chave){
    No *noAtual = raiz; // inicia busca pela raiz

    while(! noAtual->eFolha){ //enquanto não for folha
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
void insereNo(char* chave, long RRNChaveDados){
    No *noFolha = buscaNo(chave);

    insereNoFolha(noFolha, chave, RRNChaveDados);

    // verificar necessidade de split
    if(noFolha->numChaves == ORDEM){ // overflow (max = ORDEM-1)
        // cria novo nó (vazio) e inicializa
        No *novoNo = criaNo();

        novoNo->eFolha = 1;
        novoNo->pai = noFolha->pai;

        int posicaoMedia = (int) ceil(ORDEM / 2.0) - 1;

        int j;
        for (int i = posicaoMedia, j = 0; i < ORDEM; i++, j++) {
            strcpy(novoNo->chaves[j], noFolha->chaves[i]); //distribui chaves
            strcpy(noFolha->chaves[i], "NULL");

            novoNo->dadosRRN[j] = noFolha->dadosRRN[i];
            noFolha->dadosRRN[i] = -1;
        }

        novoNo->numChaves = ORDEM - posicaoMedia;
        noFolha->numChaves = posicaoMedia;

        long RRNNovoNo = armazenaNo(novoNo);

        noFolha->prox = RRNNovoNo;
        
        armazenaNo(noFolha);

        insereNoPai(noFolha, novoNo->chaves[0], novoNo);
    } else{
        // escrever no arquivo (sobrescrever)
        armazenaNo(noFolha);
    }
}

/* Insere nova chave no nó folha */
void insereNoFolha(No *folha, char *chave, long RRNChaveDados){
    if(folha->numChaves == 0){ //folha vazia
        strcpy(folha->chaves[0], chave); // chave na primeira posição
        folha->dadosRRN[0] = RRNChaveDados;
        folha->numChaves++;
    } else{
        int i = 0;

        while(i<folha->numChaves && strcmp(chave, folha->chaves[i]) > 0){ //encontra posição
            i++;
        }

        for (int j = folha->numChaves; j > i; j--) { //reposiciona chaves
            strcpy(folha->chaves[j], folha->chaves[j - 1]);
        }

        //adiciona nova chave
        strcpy(folha->chaves[i], chave);
        folha->dadosRRN[i] = RRNChaveDados;

        folha->numChaves++;
    }
}

/* Após overflow, insere chave promovida no nó pai, nó interno da árvore*/
//TODO terminar
void insereNoPai(No* noOriginal, char* chavePromovida, No* noNovo){
    if(raiz->RRN == noOriginal->RRN){ // condição de parada -> overflow na raíz
        No *novaRaiz = criaNo();

        novaRaiz->eFolha = 0;
        strcpy(novaRaiz->chaves[0], chavePromovida);
        novaRaiz->filhos[0] = noOriginal->RRN;
        novaRaiz->filhos[1] = noNovo->RRN;
        novaRaiz->numChaves = 1;
        
        raiz = novaRaiz;

        noOriginal->pai = raiz->RRN; //COMO SE AINDA N ESCREVI
        noNovo->pai = raiz->RRN;
    }
}


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
