# Filmes-bplus-tree
Trabalho de Estrutura de Dados II para o gerenciamento de um banco de dados de filmes utilizando Árvore-B+.

## Especificações
O objetivo do trabalho é gerenciar uma coleção de filmes armazenada em arquivo, em memória secundária. 

Estão disponíveis as seguintes operações: inserção de um novo filme, busca de um filme a partir do ID ou do nome, busca sequêncial de filmes em um range de IDs, e edição de registro (alteração da nota do filme).

Para isso, foram aplicadas estruturas de indexação primária e secundária. Para o índice primário, é utilizada a Árvore-B+ simulada em disco, e para o índice secundário, um vetor armazenado em RAM. Suas implementações estão no diretório src/.

## Como rodar
Para rodar, basta acessas o diretório src/ e executar o script rodar.sh.
