#include "../include/structs.h"
#include "../include/bplus-tree-manipulation.h"
#include "../include/secondaryindex-manipulation.h"
#include "../include/datafile-manipulation.h"
#include "../include/user-interaction.h"

/* variáveis globais */
No *raiz = NULL;
IndiceSecundario *vetorTitulos = NULL; //índice secundário em RAM
int numeroFilmes = 0; //quantidade de registros dos índices

int main(){
    int i;

    /*vetorTitulos = carregaSecundario(); //carrega vetor com dados do índice secundário de título*/
    carregaRaiz();

    int op = 1;
    while(op){
        printf("\n*MENU DE OPÇÕES*\n");
        printf("1 - Inserir novo filme\n2 - Remover filme\n3 - Buscar filme\n4 - Editar dados\n5 - Listar todos os filmes\n6 - Compactar arquivo\n0 - Sair\n");
        scanf("%d", &op);
     
        switch (op) {
        case 1: //inserir novo filme
            insercao();
            break;

        //case 2: //remover filme
        //    remocao(&ponteirosArquivos);
        //    break;

        case 3: //buscar filme por chave primária
            //busca();
            break;

        case 4: //editar filme
            //editarNota();
            break;

        case 5: //listar filmes
            //listarFilmes();
            break;

        //case 6: //compactar arquivo de dados
        //    compactarDados();
        //    break;

        case 0: //sair
            sair(); 
            break;
            
        default:
            break;
        }
        
    }

    /*typedef struct{
    int RRN;
    int eFolha;
    char chaves[ORDEM][TAM_CHAVE+1]; //array de chaves
    int dadosRRN[ORDEM];
    int filhos[ORDEM];
    int numChaves;
    int pai;
    int prox;
} No;*/

    printf("RRN %d é folha %d chave0 %s chave1 %s chave2 %s chave3 %s quant chaves %d filho0 %d", raiz->RRN, raiz->eFolha, raiz->chaves[0], raiz->chaves[1], raiz->chaves[2], raiz->chaves[3], raiz->numChaves, raiz->filhos[0]);

    return 0;
}
