/* Implementação das funções definidas na biblioteca */

#include "../include/user-interaction.h"

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

    //inserir no arq de dados (pegar byteoffset), na indice sec e na árvore
    insereNo(aux->chavePrimaria, 0);

    /*if(insereArquivoDados(aux) == -1 || insereIndiceSecundario(aux) == -1){
        printf("\nNão foi possível inserir");
    } else{
        printf("\nFilme '%s' inserido com sucesso!", aux->tituloOriginal);
    }*/
}

/* Lê dados para a busca, chama a função para realizar a busca binária e, por fim, caso seja encontrado um filme, lê seus dados do arquivo de dados e imprime na tela. Caso não encontre, imrpime mensagem de erro
void busca(){
    int op;
    char tituloaux[MAX_NOME + 1];
    char idaux[TAM_CHAVE + 1];
    IndicePrimario * auxP;
    Filme *auxF;

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
                int auxIndice = buscaPrimaria(vetorTitulos[i].chavePrimaria, 0, numeroFilmes); //busca pela chave primária
                Filme* auxFilme = leFilmeIndicePrimario(auxIndice); //carrega em RAM registro do arquivo de dados
                imprimeFilme(auxFilme); //imprime infos na tela
            }
            
            break;

        case 2:
            printf("\nInsira o ID do filme: ");
            scanf(" %s", idaux);

            int auxIndice = buscaPrimaria(idaux, 0, numeroFilmes); //busca filme com o índice

            if(auxIndice == -1){
                printf("Não foi encontrado o filme de ID %s\n", idaux);
                break;
            }

            Filme* auxFilme = leFilmeIndicePrimario(auxIndice); //carrega em RAM o registro
            imprimeFilme(auxFilme); //imprime na tela informações

            break; 
            
        default:
            printf("Opção inválida!\n");
    }

}

/* Lê dados necessários e edita nota de um filme, no arquivo de dados, a partir de sua chave primária
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

/* Percorre todo o arquivo de dados (seguindo a ordem alfabética de diretores), lendo o registro para RAM e imprimindo na tela suas informações
void listarFilmes(){
    int i;

    FILE *dadosp = fopen(NOME_ARQ_DADOS, "r+");

    printf("\n*LISTA DE FILMES CADASTRADOS*\n");
    for(i = 0; i < numeroFilmes; i ++){
        Filme* auxFilme = leFilmeIndicePrimario(i); //carrega em RAM o filme do índice i do índice primário
        imprimeFilme(auxFilme); //imprime informações na tela
    }

    fclose(dadosp);
}
*/