/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)

    Classe cabecalho
        (Arquivo que compoe os atributos do cabecalho do sequenceSet)
*/
// cabecalho para facilitar a manipulação da estrutura em disco
class Cabecalho {
    friend class sequenceSet;
    private:
        // qual é o primeiro bloco a ser lido.
        int posInicio = 0;
        // quantos blocos que existem
        int qtdBlocos = 0;
        // proximo bloco a partir do meu bloco atual.
        int proximoBloco = 0;
};