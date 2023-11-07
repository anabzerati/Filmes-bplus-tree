/* Implementação das funções definidas na biblioteca */

#include "../include/arquivos.h"

extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Carrega índice secunsário do disco para RAM, retorna vetor de índices ordenado*/
IndiceSecundario *carregaSecundario(){
    char bufferNom[MAX_NOME + 1];
    int i = 0, flag;

    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");
    FILE *fp = fopen(NOME_INDICE_TITULO, "r+");

    if(numeroFilmes == 0){
        return NULL;
    }

    //leitura do header
    fscanf(fp, "%d", &flag);

    if(flag == 0){ //inconsistência entre índice e arquivo de dados
        criaSecundario(dadosp, fp);
    }

    rewind(fp);
    fscanf(fp, "%d\n", &flag); //dado do header
    
    vetorTitulos = malloc(numeroFilmes * sizeof(IndiceSecundario)); //aloca vetor do índice secundário

    for(i = 0; i < numeroFilmes; i ++){ //para cada registro
        //lê título e chave primária
        char linha[TAM_CHAVE + MAX_NOME + 1];
        fgets(linha, MAX_NOME + TAM_CHAVE + 1, fp); //lê linha

        if (sscanf(linha, "%[^@]@%s", vetorTitulos[i].titulo, vetorTitulos[i].chavePrimaria) != 2) { //lê campos
            printf("Erro ao ler os campos.\n");
        } 
    }

    ordenaSecundario(numeroFilmes);

    rewind(fp);
    fprintf(fp, "%d", 0);

    fclose(fp);
    fclose(dadosp);

    return vetorTitulos;
}

/* Refaz indice secundário a partir do arquivo de dados*/
void criaSecundario(FILE *dadosp, FILE *secundariop){
    fseek(dadosp, 0, SEEK_END);
    long int size = ftell(dadosp) / TAM_REGISTRO; //quantidade total de registros

    rewind(dadosp);
    rewind(secundariop);

    fprintf(secundariop, "%d\n", 0); //flag

    for (int i = 0; i < size; i++){
        char linha[TAM_REGISTRO + 1];
        fgets(linha, TAM_REGISTRO + 1, dadosp); //lê linha
        
        if(linha[0] == '*') // significa que é apagada
        {
            continue;
        }

        char *tokenChave = strtok(linha, "@"); // pega o primeiro campo

        if (tokenChave != NULL) {
            char *tokenTitulo = strtok(NULL, "@"); // pega o segundo campo
            if (tokenTitulo != NULL) {
                printf("Chave e título lidos %s %s\n", tokenChave, tokenTitulo);
                fprintf(secundariop, "%s@%s\n", tokenTitulo, tokenChave); // imprime campos
            }
        }
    }
}

/* Insere registro no arquivo de dados e nos vetores de índices. Retorna 1 se a operação teve sucesso e 0 caso não*/
int insereRegistro(Filme *novo){
    //TODO inserir árvore
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

/* Ordena o vetor de índices secundários considerando o título. Insertion Sort
void ordenaSecundario(){
    int i, j;
    IndiceSecundario aux;

    for(i = 1; i < numeroFilmes; i ++){
        aux = vetorTitulos[i];
        j = i - 1;

        while( j >= 0 && strcmp(vetorTitulos[j].titulo, aux.titulo) > 0){ //utiliza título para ordenar
            vetorTitulos[j + 1] = vetorTitulos[j];
            j --;
        }

        vetorTitulos[j + 1] = aux;
    }
}

/* Busca filme pelo índice primário e retorna sua posição no vetor. Caso não haja registro com esse ID, retorna -1. Busca binária
int buscaPrimaria(char *chavePrimaria, int i, int j){
    int mid = (i + j) / 2;
    int val = strcmp(vetorPrimario[mid].chavePrimaria, chavePrimaria);

    if(i > j){ //não há filme no vetor com essa chave primária
        return -1;
    } else if(val == 0){ //encontrou o filme
        return mid; //retorna posição
    } else{ //continuar buscando
        if(val > 0){ //mid > chave buscada -> chave está na parte inferior do vetor
            return buscaPrimaria(chavePrimaria, i, mid - 1);
        } else{ //chave está na metade superior do vetor
            return buscaPrimaria(chavePrimaria, mid + 1, j);
        }
    }
}

/* Busca filme pelo índice secundário a partir do título e retorna os indices da primeira e útlima aparição dessa chave secundária. Caso não haja registro com esse ID, retorna -1 em j
void buscaSecundaria(char *titulo, int *i, int *j){
    int mid, val;

    int menor = 0, maior = numeroFilmes - 1;

    while(menor <= maior){ //enquanto ainda houver filmes para verificar
        mid = (menor + (maior - menor)/2);
        val = strcmp(vetorTitulos[mid].titulo, titulo);

        if(val == 0){ //encontrou um filme
            //agora, é necessário buscar os demais filmes com o mesmo título
            *i = *j = mid;
                   
            while(*i > 0 && strcmp(vetorTitulos[(*i) - 1].titulo, titulo) == 0){ //buscamos a primeira aparição no vetor
                *i = *i - 1;
            }

            while(*j < numeroFilmes - 1 && strcmp(vetorTitulos[(*j) + 1].titulo, titulo) == 0){ //buscamos última aparição
                *j = *j + 1;
            }

            return;
        } else if(val > 0){ //mid > chave buscada -> chave está na parte inferior do vetor
            maior = mid-1;
        } else{ //chave está na metade superior do vetor
            menor = mid+1;
        }
    }

    *j = -1; //se não encontrou filme 
    
}

/* Atualiza dados dos índices da RAM para o disco. Atualiza flag da header
void atualizaIndices(){
    int i;

    //arquivo de índice primário primeiro
    FILE *primario =  fopen(NOME_INDICE_PRIMARIO, "w");
    fprintf(primario, "%d %d\n", 1, numeroFilmes); //flag e quantidade de registros 

    for(i = 0; i < numeroFilmes; i++){ //para cada registro, imprime os campos de chave primária e RRN
        fprintf(primario, "%s %ld ", vetorPrimario[i].chavePrimaria, vetorPrimario[i].RRN);
    }  

    fclose(primario);

   //arquivo de índice secundário
    FILE *secundario = fopen(NOME_INDICE_TITULO, "w");
    fprintf(secundario, "%d\n", 1); //flag

    for(i = 0; i < numeroFilmes; i++){ //para cada registro, imprime os campos de título e chave primária
        fprintf(secundario, "%s@%s\n", vetorTitulos[i].titulo, vetorTitulos[i].chavePrimaria);
    }  
    
    fclose(secundario);
}

/* Chama função de atualizar índices e libera as memórias alocadas
void sair(){
    atualizaIndices();

    free(vetorPrimario);
    free(vetorTitulos);
}
*/