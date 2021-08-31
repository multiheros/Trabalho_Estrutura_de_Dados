# Trabalho_Estrutura_de_Dados

O objetivo do trabalho é apresentar uma solução eficiente para armazenar e ordenar dados em arquivos binários, utilizando para isto, a estrutura de dados Sequence Set, juntamente com uma estrutura de dados para índices, a arvore b+.

O *sequenceSet* trabalha com a ordem 15/30, isto é, blocos de tamanho mínimo 15 e máximo 30, chave primária origem (source), além de uma chave secundária índice, a fim de manter os arquivos ordenados.

Cada nó da árvore é composto por vetor de registro chamado Chave, com no máximo 4 e no mínimo 2 valores, que contém o menor elemento de um bloco, sua chave primária e secundária, além do número do bloco a qual pertence, uma indicação se é folha e seu tamanho.

Para o funciomanento do programa é necessário ter um arquivo .csv com os pacotes de redes já coletados no seguinte formato:

"Number", "Time", "Source", "Destination", "Protocol", "Length", "Info".

# Restante em desenvolvimento