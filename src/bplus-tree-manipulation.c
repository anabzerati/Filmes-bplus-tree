/* Implementação das funções definidas na biblioteca */

#include "../include/bplus-tree-manipulation.h"
#include "../include/datafile-manipulation.h"

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
        fprintf(fp, "%020ld\n", 0);

        raiz = criaNo(); //nó vazio
        raiz->eFolha = 1;

        fclose(fp);

        return;
    }

    //leitura do header
    fscanf(fp, "%020ld", &rrnRaiz); // RRN da raíz

    if(rrnRaiz == -1){ //arquivo vazio
        printf("entrou no if de arq vazio");
        fprintf(fp, "%020ld\n", 0);

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
        strcpy(novo->chaves[i], "NULL0");
        novo->dadosRRN[i] = -1;
        novo->filhos[i] = -1;
    }
    novo->filhos[ORDEM] = -1;

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
    }

    fseek(fp, byteoffset, SEEK_SET); //posiciona ponteiro

    char buffer[TAM_REGISTRO + 1];
    int pos = 0;

    // "concatenar" várias strings de uma vez - formatar string com @ entre os campos
    snprintf(buffer + pos, sizeof(buffer) - pos, "%ld@%d@", novoNo->RRN, novoNo->eFolha);
    pos = strlen(buffer);

    for(int i = 0; i < ORDEM; i++){
        snprintf(buffer + pos, sizeof(buffer) - pos, "%s@", novoNo->chaves[i]);
        pos = strlen(buffer);
    }

    for(int i = 0; i < ORDEM; i++){
        snprintf(buffer + pos, sizeof(buffer) - pos, "%ld@", novoNo->dadosRRN[i]);
        pos = strlen(buffer);
    }

    for(int i = 0; i < ORDEM + 1; i++){
        snprintf(buffer + pos, sizeof(buffer) - pos, "%ld@", novoNo->filhos[i]);
        pos = strlen(buffer);
    }

    snprintf(buffer + pos, sizeof(buffer) - pos, "%d@%d@%ld", novoNo->numChaves, novoNo->pai, novoNo->prox);
    pos = strlen(buffer);

    // preencher o resto do registro com '#'
    memset(buffer + pos, '#', sizeof(buffer) - pos - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // escrever registro no arquivo de dados
    printf("BUFFER\n %s", buffer);
    fputs(buffer, fp);
    printf("RRN APÓS ESCREVER NÓ %d", ftell(fp));

    //fwrite(novoNo, sizeof(No), 1, fp);

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

    if(RRN == -1){
        return NULL;
    }
    
    fseek(fp, RRN, SEEK_SET);

    No *noLido = malloc(sizeof(No)); //alocar espaço em memória
    //fread(noLido, sizeof(No), 1, fp);

    fscanf(fp, "%ld@%d@", &noLido->RRN, &noLido->eFolha);

    for (int i = 0; i < ORDEM; i++) {
        fscanf(fp, "%[^@]@", noLido->chaves[i]);
    }

    for (int i = 0; i < ORDEM; i++) {
        fscanf(fp, "%ld@", &noLido->dadosRRN[i]);
    }

    for (int i = 0; i < ORDEM + 1; i++) {
        fscanf(fp, "%ld@", &noLido->filhos[i]);
    }

    fscanf(fp, "%d@%ld@%ld", &noLido->numChaves, &noLido->pai, &noLido->prox);

    fclose(fp);

    return noLido;
}

/* Busca nó que contém a chave */
No *buscaNo(char *chave){
    No *noAtual = raiz; // inicia busca pela raiz

    while(! noAtual->eFolha){ //enquanto não for folha
        for(int i=0; i < noAtual->numChaves; i ++){ //percorre todas as chaves
            if(! strcmp(chave, noAtual->chaves[i])){ // se achou a chave
                noAtual = carregaNo(noAtual->filhos[i + 1]); //carrega filho à direita
                break;
            } else if(strcmp(chave, noAtual->chaves[i]) < 0){
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

void buscaRange(char *chaveInicial, char *chaveFinal){
    No *noInicial = buscaNo(chaveInicial);
    int i = 0;

    while(strcmp(noInicial->chaves[i], chaveInicial) < 0){ // posiciona índice
        i ++;
    }

    while(strcmp(noInicial->chaves[i], chaveFinal) <= 0){
        //leFilmeChavePrimaria(noInicial->dadosRRN[i]);
        printf("CHAVE ATUAL %s ", noInicial->chaves[i]);
        
        i++;
        if(i == noInicial->numChaves){ // percorreu todas as chaves, carrega próximo
            i = 0;
            noInicial = carregaNo(noInicial->prox);
            if(! noInicial){ // acabaram os elementos
                return;
            }
        }
    }
}

/* Insere novo nó na arvore, considerando a chave primária do filme */
// TODO verificar se chave já existe
void insereNo(char* chave, long RRNChaveDados){
    No *noFolha = buscaNo(chave);

    insereNoFolha(noFolha, chave, RRNChaveDados);

    if(noFolha->numChaves == ORDEM){ // overflow na folha (max = ORDEM-1)
        // cria novo nó (vazio) e inicializa
        No *novoNo = criaNo();

        novoNo->eFolha = 1;
        novoNo->pai = noFolha->pai; //mesmo pai do nó "original"

        int posicaoMedia = (int) ceil(ORDEM / 2.0) - 1;

        int i, j;
        for (i = posicaoMedia, j = 0; i < ORDEM; i++, j++) {
            strcpy(novoNo->chaves[j], noFolha->chaves[i]); //distribui chaves
            strcpy(noFolha->chaves[i], "NULL0"); //"esvazia" folha "original"

            novoNo->dadosRRN[j] = noFolha->dadosRRN[i];
            noFolha->dadosRRN[i] = -1;
        }

        novoNo->numChaves = ORDEM - posicaoMedia;
        noFolha->numChaves = posicaoMedia;

        armazenaNo(novoNo);

        insereNoPai(noFolha, novoNo->chaves[0], novoNo);

        armazenaNo(novoNo);
        noFolha->prox = novoNo->RRN; //nó antigo aponta para novo (seguinte)
        
        armazenaNo(noFolha); //sobrescreve
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
            folha->dadosRRN[j] = folha->dadosRRN[j - 1];
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
    if(raiz->RRN == noOriginal->RRN){ // condição inicial e de parada -> overflow na raíz
        printf("ENTROU NO OVERFLOW DA RAIZ");
        No *novaRaiz = criaNo();

        novaRaiz->eFolha = 0;
        strcpy(novaRaiz->chaves[0], chavePromovida);
        novaRaiz->filhos[0] = noOriginal->RRN;
        novaRaiz->filhos[1] = noNovo->RRN;
        novaRaiz->numChaves = 1;

        armazenaNo(novaRaiz);
        
        raiz = novaRaiz;
        salvaRaiz();

        noOriginal->pai = raiz->RRN; 
        noNovo->pai = raiz->RRN;

        armazenaNo(noOriginal); // TODO VERIFICAR NECESSIDADE
        armazenaNo(noNovo);

        return;
    }

    No *noPai = carregaNo(noOriginal->pai); //carrega pai (para promoção)
    
    int i = 0;
    while(strcmp(noPai->chaves[i], chavePromovida) < 0 && i < noPai->numChaves){ // busca posição onde inserir
        i++;
    }

    int j;
    for(j = noPai->numChaves; j > i; j --){ // reposiciona chaves do pai
        strcpy(noPai->chaves[j], noPai->chaves[j - 1]);
    }
    for(j = noPai->numChaves + 1; j > i; j --){ // reposiciona filhos do pai
        noPai->filhos[j] = noPai->filhos[j - 1];
    }

    if(i != noPai->numChaves && noNovo->eFolha){ //não inseriu na última posição
        noNovo->prox = noPai->filhos[i + 2]; // adequa ponteiro de próximo do novo nó
    }

    strcpy(noPai->chaves[i], chavePromovida); //adiciona chave promovida após a original
    noPai->filhos[i + 1] = noNovo->RRN;
    noPai->numChaves++;

    printf("\nNÓ PAI ");
    printf("RRN %d é folha %d chave0 %s chave1 %s chave2 %s chave3 %s quant chaves %d RRN0 %d RRNFILHO0 %d RRNFILHO1 %d prox %ld", noPai->RRN, noPai->eFolha, noPai->chaves[0], noPai->chaves[1], noPai->chaves[2], noPai->chaves[3], noPai->numChaves, noPai->dadosRRN[0], noPai->filhos[0], noPai->filhos[1], noPai->prox);


    if (noPai->numChaves == ORDEM) { //overflow no nó pai -> split
        No *irmaoPai = criaNo();

        irmaoPai->pai = noPai->pai;
        irmaoPai->eFolha = 0;

        int posicaoMedia = (int) ceil(ORDEM / 2.0) - 1;
        char novaChavePromovida[TAM_CHAVE + 1];
        strcpy(novaChavePromovida, noPai->chaves[posicaoMedia]);
        strcpy(noPai->chaves[posicaoMedia], "NULL0");

        int i, j;
        for (i = posicaoMedia + 1, j = 0; i < ORDEM; i++, j++) { 
            strcpy(irmaoPai->chaves[j], noPai->chaves[i]); //distribui chaves
            strcpy(noPai->chaves[i], "NULL0"); //"esvazia" pai "original"
        }
        for(i = posicaoMedia + 1, j = 0; i <= ORDEM; i ++, j ++){
            irmaoPai->filhos[j] = noPai->filhos[i]; 
            noPai->filhos[i] = -1;
        }

        irmaoPai->numChaves = ORDEM - posicaoMedia - 1;
        noPai->numChaves = posicaoMedia;

        armazenaNo(irmaoPai);

        for (int k = 0; k <= irmaoPai->numChaves; k++) {
            No *filho = carregaNo(irmaoPai->filhos[k]);
            filho->pai = irmaoPai->RRN; //altera pai
            printf("PAI MUDADADADO PARA FILHO %d: %d", irmaoPai->filhos[k], filho->pai);
            armazenaNo(filho);
        }
        
        insereNoPai(noPai, novaChavePromovida, irmaoPai);

        armazenaNo(irmaoPai);
        armazenaNo(noPai);
    } else{
        armazenaNo(noPai);
    }
}

void salvaRaiz(){
    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "rb+");

    if(! fp){
        perror("erro ao abrir o arquivo");
    }

    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%020ld", raiz->RRN); 
}

long verificaFinalArquivo(){
    FILE *fp = fopen(NOME_INDICE_PRIMARIO, "rb");

    if(! fp){
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long RRNFinal = ftell(fp);

    fclose(fp);

    return RRNFinal; 
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
