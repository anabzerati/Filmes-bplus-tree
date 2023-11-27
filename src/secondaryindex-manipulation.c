/* Implementação das funções definidas na biblioteca */

#include "../include/secondaryindex-manipulation.h"

extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Carrega índice secunsário do disco para RAM, retorna vetor de índices ordenado*/
void carregaSecundario(){
    char bufferNom[MAX_NOME + 1];
    int i = 0, flag;
    FILE *dadosp, *fp;

    if(verificaDados()){
        dadosp = fopen(NOME_ARQ_DADOS, "r+");
    } else{
        dadosp = fopen(NOME_ARQ_DADOS, "w+");
    }

    if(verificaSecundario()){
        fp = fopen(NOME_INDICE_TITULO, "r+");
    } else{
        fp = fopen(NOME_INDICE_TITULO, "w+");
    } 
    
    //leitura do header
    fscanf(fp, "%d %d", &flag, &numeroFilmes); //flag de consistência

    if(flag == -1){ //arquivo vazio
        numeroFilmes = 0;

        fprintf(fp, "%d %d\n", 0, 0); //flag e número de registros

        return;
    } else if(flag == 0){ //inconsistência entre índice e arquivo de dados
        criaSecundario(dadosp, fp);
    }

    rewind(fp);
    fscanf(fp, "%d %d\n", &flag, &numeroFilmes); //dados do header

    vetorTitulos = malloc(numeroFilmes * sizeof(IndiceSecundario)); //aloca vetor do índice secundário
    if(!vetorTitulos){
        perror("erro ao alocar vetor");
        return;
    } 

    for(i = 0; i < numeroFilmes; i ++){ //para cada registro
        //lê título e chave primária
        char linha[TAM_CHAVE + MAX_NOME + 1];
        fgets(linha, MAX_NOME + TAM_CHAVE + 1, fp); //lê linha

        if (sscanf(linha, "%[^@]@%s", vetorTitulos[i].titulo, vetorTitulos[i].chavePrimaria) != 2) { //lê campos
            perror("Erro ao ler os campos.\n");
            return;
        } 
    }

    ordenaSecundario(numeroFilmes);
}

/* Refaz indice secundário a partir do arquivo de dados*/
void criaSecundario(FILE *dadosp, FILE *secundariop){
    fseek(dadosp, 0, SEEK_END);
    long int size = ftell(dadosp) / TAM_REGISTRO; //quantidade total de registros

    rewind(dadosp);
    rewind(secundariop);

    fprintf(secundariop, "%d %d\n", 0, numeroFilmes); //flag

    for (int i = 0; i < size; i++){
        char linha[TAM_REGISTRO + 1];
        fgets(linha, TAM_REGISTRO + 1, dadosp); //lê linha
        
        if(linha[0] == '*'){ // significa que é apagada
            continue;
        }

        char *tokenChave = strtok(linha, "@"); // pega o primeiro campo

        if (tokenChave != NULL) {
            char *tokenTitulo = strtok(NULL, "@"); // pega o segundo campo
            if (tokenTitulo != NULL) {
                fprintf(secundariop, "%s@%s\n", tokenTitulo, tokenChave); // imprime campos
            }
        }
    }
}

int insereIndiceSecundario(Filme *novoFilme){ 
    IndiceSecundario * novovetorTitulos = realloc(vetorTitulos, numeroFilmes * sizeof(IndiceSecundario)); //realocando vetor de títulos

    if(!novovetorTitulos){ //verificando se a memória foi realocada
        printf("Erro ao alocar memória");
        
        return -1;
    }

    vetorTitulos = novovetorTitulos;

    strcpy(vetorTitulos[numeroFilmes - 1].titulo, novoFilme->tituloOriginal);
    strcpy(vetorTitulos[numeroFilmes - 1].chavePrimaria, novoFilme->chavePrimaria);

    ordenaSecundario(); //reordena vetor

    return 1; 
}

/* Ordena o vetor de índices secundários considerando o título. Insertion Sort*/
void ordenaSecundario(){
    int i, j;
    IndiceSecundario aux;

    for(i = 1; i < numeroFilmes; i ++){
        aux = vetorTitulos[i];
        j = i - 1;

        while(j >= 0 && strcmp(vetorTitulos[j].titulo, aux.titulo) > 0){ //utiliza título para ordenar
            vetorTitulos[j + 1] = vetorTitulos[j];
            j --;
        }

        vetorTitulos[j + 1] = aux;
    }
}


/* Busca filme pelo índice secundário a partir do título e retorna os indices da primeira e útlima aparição dessa chave secundária. Caso não haja registro com esse ID, retorna -1 em j */
void buscaSecundaria(char *titulo, int *i, int *j){
    int mid, val;

    int menor = 0, maior = numeroFilmes - 1;

    while(menor <= maior){ //enquanto ainda houver filmes para verificar
        mid = (menor + (maior - menor)/2);
        val = strcmp(vetorTitulos[mid].titulo, titulo);

        if(val == 0){ // encontrou um filme
            // buscar os demais filmes com o mesmo título
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

/* Função que verifica se o índice secundário existe. Retorna 0 se não existir e 1 se existir*/
int verificaSecundario(){
    FILE *fp;

    if((fp = fopen(NOME_INDICE_TITULO, "r+")) == NULL){ //não existe arquivo;
        return 0;
    }

    free(fp);
    return 1;
}

/* Remove posição do vetor de índice secundário a partir de sua posição, readequando as posições dos demais elementos. Retorna 1 se a operação teve sucesso e 0 caso não*/
int removePosicaoSecundario(int pos){
    int i;

    for(i = pos; i < numeroFilmes - 1; i ++){ //readequa posições do vetor a partir do elemento a ser removido
        vetorTitulos[i] = vetorTitulos[i+1];
    }

    IndiceSecundario *novoVetorTitulos = realloc(vetorTitulos, (numeroFilmes - 1) * sizeof(IndiceSecundario)); //realoca vetor

    if(!novoVetorTitulos){
        printf("erro no realloc");

        return 0;
    }

    vetorTitulos = novoVetorTitulos;
    
    return 1;
}