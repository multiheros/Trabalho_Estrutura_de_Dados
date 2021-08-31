/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)

    Classe bloco
        (Arquivo com os atributos e metodos da classe, alem
        de declaracoes essenciais para os dados e indices da arvore
        Chave e Pacote)
*/
#include <iostream>

using namespace std;

// para saber o tamanho de cada campo, fiz um primeiro teste para
// pegar o tamanho máximo de cada campo do tipo texto
// adicionei mais uma posição para armazenar o término de string '\0'
struct Pacote {
    unsigned indice = 0;
    double tempo = 0;
    char origem[40] = "";
    char destino[40] = "";
    char protocolo[18] = "";
    unsigned tamanho = 0;
    char infomarcao[1650] = "";
};

// registro utilizado para armazenar a menor chave do bloco
struct Chave {
	unsigned indice = -1;
	char origem[40] = "";
    int numBloco = -1;
};

// quantidade de pacotes possiveis no bloco.
const int capacidade = 30;

// classe responsavel por armazenar os pacotes em blocos de tamanho 30
class Bloco {
    friend class sequenceSet;
    public:
        // construtor do bloco.
        Bloco();
        // destrutor do bloco
        ~Bloco(){}
        // atualiza os atributos maior e menor
        void AtualizarChave();
        // retorna a menor chave do bloco
        Chave getMenor();
        // retorna o numero do bloco
        int getNumBloco();
        // retorna o tamanho do bloco.
        int getTamanho();
    private:
        // vetor de pacotes, para armazenar os pacotes no bloco.
        Pacote mPacote[capacidade];
        // variavel para armazenar proximo bloco na cadeia.
        int mProximo;
        // variavel para armazenar o bloco anterior na cadeia.
        int mAnterior;
        // quantidade de pacotes no bloco.
        int mTamanho;
        // variavel para identificar qual bloco é.
        int numBloco;
        // menor chave do bloco
        Chave menor;
};

Bloco::Bloco() : mProximo(-1), mAnterior(-1), mTamanho(0), numBloco(0) {}

// atualiza o atributo com o menor elemento do bloco
void Bloco::AtualizarChave() {
    if(mTamanho != 0) {
        string umaString;

        umaString = mPacote[0].origem;
        strcpy(menor.origem, umaString.c_str());
        menor.indice = mPacote[0].indice;
        menor.numBloco = numBloco;
    }
    else {
        throw runtime_error("Bloco vazio, impossível atualizar as chaves");
    }
}

Chave Bloco::getMenor() {
    return menor;
}

int Bloco::getNumBloco() {
    return numBloco;
}

int Bloco::getTamanho() {
    return mTamanho;
}