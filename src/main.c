#include "../include/structs.h"
#include "../include/arquivos.h"
#include "../include/interacao.h"

/* variáveis globais */
IndiceSecundario *vetorTitulos = NULL; //índice secundário em RAM
int numeroFilmes = 0; //quantidade de registros dos índices

int main(){
    int i;

    vetorTitulos = carregaSecundario(); //carrega vetor com dados do índice secundário de título
    
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
            busca();
            break;

        case 4: //editar filme
            editarNota();
            break;

        case 5: //listar filmes
            listarFilmes();
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

    return 0;
}
