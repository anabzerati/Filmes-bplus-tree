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

    carregaSecundario(); //carrega vetor com dados do índice secundário de título
    printf("CARREGOU");
    carregaRaiz();

    printf("numero de filmes %d", numeroFilmes);
    for(int i = 0; i < numeroFilmes; i ++){
        printf("filme %s", vetorTitulos[i].titulo);
    }

    printf("\n raiz rrn %d", raiz->RRN);

    int op = 1;
    while(op){
        printf("\n*MENU DE OPÇÕES*\n");
        printf("1 - Inserir novo filme\n2 - Buscar filme\n3 - Editar dados\n4 - Listar todos os filmes\n0 - Sair\n");
        scanf("%d", &op);
     
        switch (op) {
        case 1: //inserir novo filme
            insercao();
            break;

        case 2: //buscar filme por chave primária
            busca();
            break;

        case 3: //editar nota do filme
            editarNota();
            break;

        case 4: //listar filmes
            listarFilmes();
            break;

        case 0: //sair
            sair(); 
            exit(0);
            break;
            
        default:
            break;
        }
        
    }

    printf("saiu do while");
    
    exit(1);
    //return 0;
}

