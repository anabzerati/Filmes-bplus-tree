/* Implementação das funções definidas na biblioteca */

#include "../include/user-interaction.h"
#include "../include/bplus-tree-manipulation.h"

extern IndiceSecundario *vetorTitulos;
extern int numeroFilmes;

/* Lê dados do filme inseridos pelo usuário e retorna uma struct preenchida */
Filme *leDadosFilme(){
    Filme *aux = malloc(sizeof(Filme));

    printf("\n*PREENCHIMENTO DE DADOS*\n");

    printf("Título original: ");
    scanf(" %[^\n]s", aux->tituloOriginal); //lê até quebra de linha

    printf("Título em português: ");
    scanf(" %[^\n]s", aux->tituloPortugues);

    char auxNome[MAX_NOME/2], auxSobrenome[MAX_NOME/2];
    printf("Diretor: ");

    printf("Primeiro nome: ");
    scanf(" %[^\n]s", auxNome);

    printf("Sobrenome: ");
    scanf(" %[^\n]s", auxSobrenome);

    strcat(auxSobrenome, ", ");
    strcat(auxSobrenome, auxNome);
    strcpy(aux->diretor, auxSobrenome);

    printf("Ano de lançamento: ");
    scanf(" %[^\n]s", aux->anoLancamento);

    printf("País: ");
    scanf(" %[^\n]s", aux->pais);

    int auxNota;

    //validação da nota inserida
    do{
        printf("Nota: ");
        scanf(" %d", &auxNota);

        if(auxNota > 9 || auxNota < 0){
            printf("\nValor inválido para nota. Insira um número entre 0 e 9.");
        }
    } while(auxNota > 9 || auxNota < 0);

    aux->nota = auxNota + '0'; //convertendo inteiro para caracter

    //determinação da chave primária
    char auxId[6];
    strncpy(auxId, auxSobrenome, 3);
    strncpy(auxId + 3, aux->anoLancamento + 2, 2);
    auxId[5] = '\0';
    
    // converter para maiusculo
    for (int i = 0; auxId[i]; i++) {
        auxId[i] = toupper(auxId[i]);
    }

    // salvando a chave determinada
    strcpy(aux->chavePrimaria, auxId);

    return aux;
}

/* Recebe uma struct com dados do filme e os imprime na tela*/
void imprimeFilme(Filme *aux){
    printf("\n*Filme %s*", aux->chavePrimaria);
    printf("\nTítulo Original: %s\nTítulo em português: %s", aux->tituloOriginal, aux->tituloPortugues);
    printf("\nDiretor: %s", aux->diretor);
    printf("\nAno de lançamento: %s\nPaís: %s\nNota:%c\n", aux->anoLancamento, aux->pais, aux->nota);

    free(aux);
}

/* Faz o intermédio entre ler os dados do filme e ser inserido e chamar a função de inserir. Verifica sucesso e imprime uma mensagem na tela*/
void insercao(){
    Filme *aux = leDadosFilme(); //pega info com usuário

    long rrnDados = insereArquivoDados(aux); // insere no arq de dados

    if(rrnDados == -1){
        perror("Erro ao inserir");
    } else{
        insereNo(aux->chavePrimaria, rrnDados); // insere na árvore
        insereIndiceSecundario(aux); // insere no índice secundário
    }
}

/* Lê dados para a busca, chama a função para realizar a busca binária e, por fim, caso seja encontrado um filme, lê seus dados do arquivo de dados e imprime na tela. Caso não encontre, imrpime mensagem de erro */
void busca(){
    int op;
    char tituloaux[MAX_NOME + 1];
    char idaux[TAM_CHAVE + 1];

    printf("*BUSCA*\n");
    printf("\n1 - Busca por título\n2 - Busca por ID\n");
    scanf("%d", &op);

    switch(op){
        case 1:
            printf("\nInsira o título do filme: ");
            scanf(" %[^\n]s", tituloaux);

            int comeco = 0, final = numeroFilmes - 1;
            buscaSecundaria(tituloaux, &comeco, &final); //busca filmes de mesmo título 

            if(final == -1){
                printf("Não foi encontrado um filme de título %s\n", tituloaux);
                break;
            }

            for(int i = comeco; i <= final; i ++){ //imprime todos os filmes encontrados
                No *noAux = buscaNo(vetorTitulos[i].chavePrimaria);

                int j;
                for(j = 0; j < noAux->numChaves; j ++){
                    if(! strcmp(noAux->chaves[j], vetorTitulos[i].chavePrimaria)){
                        break;
                    }
                }

                Filme *auxF = leFilmeChavePrimaria(noAux->dadosRRN[j]);
                imprimeFilme(auxF);
            }
            
            break;

        case 2:
            printf("\nInsira o ID do filme: ");
            scanf(" %s", idaux);

            No *encontrado = buscaNo(idaux);
            printf("\nno encontrado na busca %d %s\n", encontrado->RRN, encontrado->chaves[0]);

            int i;
            for(i = 0; i < encontrado->numChaves; i ++){
                if(!strcmp(encontrado->chaves[i], idaux)){
                    break;
                }
            }

            if(i == encontrado->numChaves){ // não achou no nó
                printf("Não foi encontrado um filme de ID %s\n", idaux);
                break;
            }

            Filme *auxF = leFilmeChavePrimaria(encontrado->dadosRRN[i]);
            imprimeFilme(auxF);

            break; 
            
        default:
            printf("Opção inválida!\n");
    }

}

/* Lê dados necessários e edita nota de um filme, no arquivo de dados, a partir de sua chave primária */
void editarNota(){
    char idaux[TAM_CHAVE + 1], novaNota;

    //leitura de dados
    printf("\n*EDIÇÃO DE NOTA*\n");
    printf("Insira o ID do filme: ");
    scanf(" %s", idaux);

    int auxNota;
    do{
        printf("Insira a nova nota: ");
        scanf(" %d", &auxNota);

        if(auxNota > 9 || auxNota < 0){
            printf("\nValor inválido para nota. Insira um número entre 0 e 9.");
        }
    } while(auxNota > 9 || auxNota < 0);

    novaNota = auxNota + '0'; //convertendo inteiro para caracter

    int flag = alteraRegistro(novaNota, idaux);
    if(flag == 0){
        printf("Não foi possível alterar a nota.");
    }

}

/* Percorre todo o arquivo de dados (seguindo a ordem alfabética de diretores), lendo o registro para RAM e imprimindo na tela suas informações */
void listarFilmes(){
    int i, op;

    printf("*LISTAGEM DE FILMES*\n");
    printf("\n1 - Listagem em range\n2 - Listagem de todos os filmes cadastrados\n");
    scanf("%d", &op);

    switch (op){
    case 1:
        char idInicial[TAM_CHAVE + 1], idFinal[TAM_CHAVE + 1];

        printf("\nInsira o ID do filme inicial: ");
        scanf(" %s", idInicial);

        printf("\nInsira o ID do filme final: ");
        scanf(" %s", idFinal);

        buscaRange(idInicial, idFinal);

        break;
    
    case 2:
        buscaRange("AAA00", "ZZZ99");

        break;

    default:
        printf("Opção inválida!");
        break;
    }

    /*
    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");

    printf("\n*LISTA DE FILMES CADASTRADOS*\n");
    for(i = 0; i < numeroFilmes; i ++){
        Filme* auxFilme = leFilmeIndicePrimario(i); //carrega em RAM o filme do índice i do índice primário
        imprimeFilme(auxFilme); //imprime informações na tela} 
    */

}