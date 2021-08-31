/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)

    Classe sequenceSet
        (Arquivo com os atributos e metodos da classe para manipular
        os dados do arquivo)
*/
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "bloco.hpp"
#include "cabecalho.hpp"
#include "operator.hpp"
#include "Arvore B+.hpp"

using namespace std;

// tamanho de bytes do cabeçalho definido como uma constante global.
const int tamCabecalho  = sizeof(Cabecalho);
// tamanho de bytes do bloco definido como uma constante global.
const int tamBloco = sizeof(Bloco);

class sequenceSet {
    public:
        // construtor do sequenceSet.
        sequenceSet(bool reaproveitar);
        // destrutor do sequenceSet.
        ~sequenceSet();
        // cria toda a estrutura a partir de um arquivo binário já existente
        void criarPeloBinario();
        // insere um pacote em um bloco.
        void InserirPacote(Pacote& umPacote);
        // insere um pacote em um bloco.
        void InserirAux(Pacote& umPacote, Bloco& umBloco);
        // remove um pacote do bloco.
        void RemoverPacote(Pacote& umPacote);
        // altera um pacote no bloco.
        void AlterarPacote(Pacote& pacoteOriginal, Pacote& umPacoteAlterado);
        // quando o bloco está cheio ele divide o bloco pela metade e
        // cria um novo bloco com a divisão do bloco principal
        void DivideBloco(Bloco& umBloco, Bloco& novoBloco);
        // busca padrão
        bool Buscar(Pacote& umPacote);
        // procura o elemento dentro do bloco
        Bloco BuscarPacote(Pacote& umPacote, int& posicao);
        // lê um bloco no disco.
        Bloco LerBlocoArquivo(int bloco);
        // lê o cabecalho do disco
        void LerCabecalhoArquivo();
        // escreve o bloco em disco.
        void EscreverNoArquivo(Bloco& umBloco);
        // escreve o cabecalho no disco.
        void EscreverNoArquivo(Cabecalho& umCabecalho);
        // lê o arquivo.csv e manda inserir.
        bool LerArquivoCSV(int& inicio, int& fim);
        // retorna qual bloco é o primeiro a ser lido.
        int getPosInicio();
        // verifica se um bloco está vazio.
        bool Vazio(Bloco& umBloco);
        // verifica se um bloco está cheio.
        bool Cheio(Bloco& umBloco);
        // reseta a árvore completamente
        void Resetar();
        // imprime o cabecalho
        void ImprimirCabecalho();
        // imprime a estrutura da árvore
        void ImprimirArvore();
        // imprime os blocos
        void ImprimirBlocos();
    private:
        Pacote REMOVIDO;
        Cabecalho mCabecalho;
        BPArvore *umaArvore;
};

// inicializa o arquivo e faz a verificação se ira aproveitar um arquivo antigo
// se reaproveitar for verdadeiro, então o arquivo é mantido
// se não for verdadeiro, o arquivo e destruido e outro é inicializado no lugar
sequenceSet::sequenceSet(bool reaproveitar) : REMOVIDO(), umaArvore(new BPArvore()) {
    if(not reaproveitar) {
        FILE* arquivo;
        arquivo = fopen("arquivoUm.bin", "wb");

        if(not(arquivo)) {
            throw runtime_error("erro ao abrir arquivo");
        }

        mCabecalho.posInicio = 0;
        mCabecalho.proximoBloco = 0;
        mCabecalho.qtdBlocos = 0;

        fwrite(&mCabecalho, sizeof(Cabecalho), 1, arquivo);
        fclose(arquivo);
    }
}

// destrutor da sequenceSet, deleta a árvore
sequenceSet::~sequenceSet() {
    if(mCabecalho.qtdBlocos > 0) {
        delete umaArvore;
    }
}

// utilizado para criar a estrutura a partir de um arquivo binário já existente
// inicializa a sequenceSet e constroi a árvore a partir do arquivo binário
void sequenceSet::criarPeloBinario() {
    LerCabecalhoArquivo();
    Bloco umBloco;
    Chave umaChave;

    for (int i = 0; i < mCabecalho.qtdBlocos; i++) {
        if(i == 0) {
            umBloco = LerBlocoArquivo(mCabecalho.posInicio);
        }
        else {
            umBloco = LerBlocoArquivo(umBloco.mProximo);
        }
        umaChave = umBloco.getMenor();
        umaArvore->inserir(umaChave);
    }
}


// inicializa o primeiro bloco e insere o primeiro pacote, caso já exista um bloco chama a inserirAux.
void sequenceSet::InserirPacote(Pacote& umPacote) {
    if(mCabecalho.qtdBlocos == 0) { // arquivo sem bloco
        Bloco umBloco;
        umBloco.mPacote[0] = umPacote;
        umBloco.mTamanho++;
        // atualiza a menor chave do bloco
        umBloco.AtualizarChave();
        // salva a menor chave do bloco
        Chave umaChave = umBloco.getMenor();
        // insere a chave na árvore
        umaArvore->inserir(umaChave);

        mCabecalho.proximoBloco++;
        mCabecalho.qtdBlocos++;

        EscreverNoArquivo(umBloco);
        EscreverNoArquivo(mCabecalho);
    }
    else if(mCabecalho.qtdBlocos > 0) {  // arquivo com blocos já inseridos
        int posicao = 0;
        Bloco umBloco = BuscarPacote(umPacote, posicao);
        if(posicao == -1) { // verifica se o pacote a ser inserido já existe na sequenceSet.
            InserirAux(umPacote, umBloco);
        }
        else { // pacote com chave igual já inserido no sequenceSet
            throw runtime_error("Chave já existe, impossível inserir");
        }
    }
}

// insere um pacote no bloco
void sequenceSet::InserirAux(Pacote& umPacote, Bloco& umBloco) {
    bool controle = false;
    int posicao = 0;

    if(not Cheio(umBloco)) { // verifica se o bloco está cheio.
        posicao = umBloco.mTamanho;
        while((not controle) and (posicao >= 0)) { // laço para encontrar a posição de inserção no bloco.
            if(umPacote > umBloco.mPacote[posicao-1]) { // achou a posição para inserção, insere o pacote e sai do laço.
                // se o pacote alterado é o primeiro do bloco, devemos atualizar a chave na árvore.
                if(posicao == 0) {
                    // salva a menor chave do bloco antes da alteração
                    Chave chaveAntiga = umBloco.getMenor();
                    umBloco.mPacote[posicao] = umPacote;
                    umBloco.mTamanho++;
                    // após a inserção atualiza a chave do bloco
                    umBloco.AtualizarChave();
                    // salva a nova menor chave do bloco
                    Chave chaveNova = umBloco.getMenor();
                    // altera a chave na árvore
                    umaArvore->alterarChave(chaveAntiga, chaveNova);
                }
                // se não, insere normalmente no bloco sem alterar a chave na árvore.
                else {
                    umBloco.mPacote[posicao] = umPacote;
                    umBloco.mTamanho++;
                }
                EscreverNoArquivo(umBloco);
                controle = true;
            }
            else { // move os elementos até encontrar a posição de inserção do pacote.
                umBloco.mPacote[posicao] = umBloco.mPacote[posicao-1];
                posicao--;
            }
        }
    }
    else { // bloco cheio, cria um novo bloco e atualiza os atributos
        Bloco novoBloco;
        novoBloco.numBloco = mCabecalho.qtdBlocos;
        mCabecalho.qtdBlocos++;
        mCabecalho.proximoBloco++;

        if(umBloco.mProximo != -1) { // se o bloco a ser inserido é no meio da sequenceSet
            Bloco umBlocoPosterior;
            umBlocoPosterior = LerBlocoArquivo(umBloco.mProximo);
            novoBloco.mProximo = umBloco.mProximo;
            novoBloco.mAnterior = umBlocoPosterior.mAnterior;
            umBloco.mProximo = novoBloco.numBloco;
            umBlocoPosterior.mAnterior = novoBloco.numBloco;
            EscreverNoArquivo(umBlocoPosterior);  
        }
        else { // caso o bloco a ser inserido é no final
            umBloco.mProximo = novoBloco.numBloco;
            novoBloco.mAnterior = umBloco.numBloco;
        }

        DivideBloco(umBloco, novoBloco);

        // verifica em qual bloco vai ser inserido o pacote após a divisão
        if(umBloco.mPacote[umBloco.mTamanho-1] > umPacote) {
            InserirAux(umPacote, umBloco);
        }
        else if(umPacote > umBloco.mPacote[umBloco.mTamanho-1] && novoBloco.mPacote[0] > umPacote) {
            InserirAux(umPacote, umBloco);
        }
        else if(umPacote > novoBloco.mPacote[0]) {
            InserirAux(umPacote, novoBloco);
        }
    }
}

// remove um pacote
void sequenceSet::RemoverPacote(Pacote& umPacote){
    if(mCabecalho.qtdBlocos > 0) {
        int posicao = 0;
        bool controle = false;
        Bloco umBloco = BuscarPacote(umPacote, posicao);

        if(posicao != -1) { // Verifica se o pacote existe no bloco.
            if(umBloco.mTamanho > 1) { // quando bloco tiver mais de um pacote.
                Chave chaveAntiga, chaveNova;
                if (posicao == umBloco.mTamanho - 1){ // achou o pacote remove.
                    umBloco.mPacote[posicao] = REMOVIDO;
                }
                else{
                    // se a chave a ser excluída é a menor do bloco, salva ela para alteração na árvore
                    if(posicao == 0) {
                        chaveAntiga = umBloco.getMenor();
                        controle = true;
                    }
                    // movimenta todos os elementos para a esquerda a partir do elemento removido.
                    while (posicao < umBloco.mTamanho - 1){ 
                        umBloco.mPacote[posicao] = umBloco.mPacote[posicao+1];
                        posicao++;
                    }
                    umBloco.mPacote[posicao] = REMOVIDO;
                    // caso a chave removida seja a menor do bloco, atualiza a nova menor chave do bloco após remoção
                    // depois pega essa nova menor chave do bloco e altera ela na árvore
                    if(controle) {
                        umBloco.AtualizarChave();
                        chaveNova = umBloco.getMenor();
                    }
                }
                umBloco.mTamanho--;

                // se a remoção da é a menor chave do bloco, manda alterar a chave na árvore
                if(controle) {
                    umaArvore->alterarChave(chaveAntiga, chaveNova);
                }
            }

            else if (umBloco.mTamanho == 1) { // quando o bloco tiver apenas um pacote.
                // chave utilizada para procurar o bloco a ser retirado da árvore após a remoção do último pacote do bloco
                Chave remover = umBloco.getMenor();
                umBloco.mPacote[posicao] = REMOVIDO;
                umBloco.mTamanho--;
                if(mCabecalho.qtdBlocos > 1) { // verifica se não é o ultimo bloco
                    Bloco umBlocoAnterior;

                    if(umBloco.mProximo != -1) { // verifica se existe um proximo bloco.
                        Bloco umBlocoPosterior;
                    
                        if(umBloco.numBloco == mCabecalho.posInicio) { // verifica se o bloco removido é o inicio.
                            mCabecalho.posInicio = umBloco.mProximo;
                            umBlocoPosterior = LerBlocoArquivo(umBloco.mProximo);
                            umBlocoPosterior.mAnterior = -1;
                            umBloco.mProximo = -1;
                            EscreverNoArquivo(umBlocoPosterior);
                        }
                        else { // remoção do bloco no meio do sequenceSet.
                            umBlocoPosterior = LerBlocoArquivo(umBloco.mProximo);
                            umBlocoAnterior = LerBlocoArquivo(umBloco.mAnterior);
                            umBlocoAnterior.mProximo = umBloco.mProximo;
                            umBlocoPosterior.mAnterior = umBloco.mAnterior;
                            umBloco.mProximo = -1;
                            umBloco.mAnterior = -1;
                            EscreverNoArquivo(umBlocoPosterior);
                            EscreverNoArquivo(umBlocoAnterior);
                        }
                    }
                    else { // remoção do bloco no final do sequenceSet.
                        umBlocoAnterior = LerBlocoArquivo(umBloco.mAnterior);
                        umBlocoAnterior.mProximo = -1;
                        umBloco.mAnterior = -1;
                        EscreverNoArquivo(umBlocoAnterior);
                    }
                    mCabecalho.proximoBloco = umBloco.numBloco;
                }
                else { // quando tiver apenas um bloco no sequenceSet.
                    mCabecalho.posInicio = 0;
                    umBloco.mProximo = -1;
                    umBloco.mAnterior = -1;
                    mCabecalho.proximoBloco = 0;
                }
                mCabecalho.qtdBlocos--;
                EscreverNoArquivo(mCabecalho);
                // chamando método da árvore para remoção do bloco
                umaArvore->remover(remover);
            }
            EscreverNoArquivo(umBloco);
        }
        else {
            throw runtime_error("Pacote não encontrado");
        }
    }
    else {
        throw runtime_error("Impossível remover, estrura vazia");
    }
}

// altera um pacote
void sequenceSet::AlterarPacote(Pacote& pacoteOriginal, Pacote& umPacoteAlterado) {
    // se a chave secundaria não foi alterada, então só atualizamos o pacote na msm posição do pacote original
    if(pacoteOriginal == umPacoteAlterado) {
        int posicao = 0;
        Bloco umBloco = BuscarPacote(pacoteOriginal, posicao);
        umBloco.mPacote[posicao] = umPacoteAlterado;
        EscreverNoArquivo(umBloco);
    }
    // se não, removemos o pacote e inserimos para encontrar na nova posição correta depois da alteração da chave
    else {
        RemoverPacote(pacoteOriginal);
        InserirPacote(umPacoteAlterado);
    }
}

// pega metade dos elementos do umBloco e insere no novoBloco
// escreve as alterações em disco, incluindo a do cabeçalho
// insere a chave do novo bloco na árvore
void sequenceSet::DivideBloco(Bloco& umBloco, Bloco& novoBloco) {
    for (int i = capacidade/2; i < capacidade; i++) {
        novoBloco.mPacote[novoBloco.mTamanho] = umBloco.mPacote[i];
        umBloco.mPacote[i] = REMOVIDO;
        novoBloco.mTamanho++;
        umBloco.mTamanho--;
    }
    // atualiza a menor chave do bloco
    novoBloco.AtualizarChave();
    // salva a menor chave do bloco
    Chave umaChave = novoBloco.getMenor();
    // insere a chave na árvore
    umaArvore->inserir(umaChave);
    EscreverNoArquivo(umBloco);
    EscreverNoArquivo(novoBloco);
    EscreverNoArquivo(mCabecalho);
}

// busca padrão
bool sequenceSet::Buscar(Pacote& umPacote) {
    int posicao = 0;
    BuscarPacote(umPacote, posicao);

    if(posicao != -1) {
        return true;
    }
    else {
        return false;
    }
}

// busca um pacote no bloco.
Bloco sequenceSet::BuscarPacote(Pacote& umPacote, int& posicao) {
    if(mCabecalho.qtdBlocos > 0) { // verifica se a sequenceSet não está vazia.
        int bloco = umaArvore->buscar(umPacote);
        Bloco umBloco = LerBlocoArquivo(bloco);
        bool controle = false;
        posicao = umBloco.mTamanho - 1;

        while((not controle) and (posicao >= 0)) { // laço para encontrar o pacote
            if (umBloco.mPacote[posicao] == umPacote) { // pacote encontrado, sai do laço.
                umPacote = umBloco.mPacote[posicao];
                controle = true;
            }
            else { // decrementa a posição para verificar proximo pacote.
                posicao--;
            }
        }

        if(not controle) { // caso não encontre, atualiza a posicao com um valor invalido para verificação posterior.
            posicao = -1;
        }
        return umBloco;
    }
    else {
        throw runtime_error("Impossivel buscar, estrutura vazia");
    }
}

// lê um bloco do disco e grava na memoria primaria.
Bloco sequenceSet::LerBlocoArquivo(int bloco) {
    FILE* arquivo;
    arquivo = fopen("arquivoUm.bin", "rb");

    if(not(arquivo)) {
        throw runtime_error("erro ao abrir arquivo");
    }

    Bloco umBloco;
    int deslocamento = tamCabecalho + (tamBloco*bloco);

    fseek(arquivo, deslocamento, SEEK_SET);
    fread(&umBloco, sizeof(Bloco), 1, arquivo);

    fclose(arquivo);

    return umBloco;
}

// lê o cabecalho do disco e grava na memoria primaria.
void sequenceSet::LerCabecalhoArquivo() {
    FILE* arquivo;
    arquivo = fopen("arquivoUm.bin", "rb");

    if(not(arquivo)) {
        throw runtime_error("erro ao abrir arquivo");
    }

    fread(&mCabecalho, sizeof(Cabecalho), 1, arquivo);

    fclose(arquivo);
}

// escreve no disco um bloco da memoria primaria.
void sequenceSet::EscreverNoArquivo(Bloco& umBloco) {
    FILE* arquivo;
    arquivo = fopen("arquivoUm.bin", "r+b");

    if(not(arquivo)) {
        throw runtime_error("erro ao abrir arquivo");
    }

    int deslocamento = tamCabecalho + (tamBloco*umBloco.numBloco);
    fseek(arquivo, deslocamento, SEEK_SET);
    fwrite(&umBloco, sizeof(Bloco), 1, arquivo);

    fclose(arquivo);
}

// escreve no disco o cabeçalho da memoria primaria.
void sequenceSet::EscreverNoArquivo(Cabecalho& umCabecalho) {
    FILE* arquivo;
    arquivo = fopen("arquivoUm.bin", "r+b");

    if(not(arquivo)) {
        throw runtime_error("erro ao abrir arquivo");
    }

    fwrite(&mCabecalho, sizeof(Cabecalho), 1, arquivo);

    fclose(arquivo);
}

// lê os pacotes do arquivo captura_pacotes.csv e manda inserir no sequenceSet
bool sequenceSet::LerArquivoCSV(int& inicio, int& fim) {

    std::ifstream arquivo_csv("captura_pacotes.csv");
    if (not(arquivo_csv)) {
		throw runtime_error("captura_pacotes.csv não abriu");
    }

    int contador = 0;
    string linha;
    getline(arquivo_csv,linha); // descarta primeira linha do arquivo

    string campo; // variável para obter um campo de cada linha lida
    string delimitador = "\",\""; // delimitador entre os campos
    unsigned posFimCampo; // posição final do campo

    Pacote umPacote;

	while((getline(arquivo_csv,linha)) and (contador <= fim)) {
        campo = linha.erase(0,1); // remove primeiro caracter da linha (")

        // obtendo primeiro campo, um inteiro - No.
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.indice = stoul(campo);

        // obtendo segundo campo, um float - Time
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tempo = stof(campo);

        // obtendo terceiro campo, um texto - Source
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.origem, campo.c_str());

        // obtendo quarto campo, um texto - Destination
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.destino, campo.c_str());

        // obtendo quinto campo, um texto - Protocol
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.protocolo, campo.c_str());

        // obtendo sexto campo, um texto - Length
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tamanho = stoul(campo);

        // obtendo último campo, um texto - Info
        // nesse caso, procuro o último aspas, fazendo busca reversa
        posFimCampo = linha.rfind("\"");
        campo = linha.substr(0, posFimCampo);
        strcpy(umPacote.infomarcao, campo.c_str());

        if(contador >= inicio) {
            InserirPacote(umPacote);
        }
        contador++;
    }
    return true;
}

// retorna qual é o primeiro bloco da sequenceSet
int sequenceSet::getPosInicio() {
    int posInicio = mCabecalho.posInicio;
    return posInicio;
}

// verifica se o bloco está vazio.
bool sequenceSet::Vazio(Bloco& umBloco) {
    if(umBloco.getTamanho() == 0) {
        return true;
    }
    else {
        return false;
    }
}

// verifica se o bloco está cheio.
bool sequenceSet::Cheio(Bloco& umBloco) {
    if(umBloco.getTamanho() == capacidade) {
        return true;
    }
    else {
        return false; 
    }
}

// para resetar a árvore para uma nova aplicação
void sequenceSet::Resetar() {
    if(mCabecalho.qtdBlocos > 0) {
        delete umaArvore;
    }
    else {
        throw runtime_error("Impossivel resetar a árvore, estrutura vazia");
    }
}

// imprime o cabeçalho, depuração.
void sequenceSet::ImprimirCabecalho() {
    cout << "------------------------>" << "Cabeçalho" << "<------------------------" << endl;
    cout << "Posição do inicio: " << mCabecalho.posInicio << endl;
    cout << "Proximo Pedaço: " << mCabecalho.proximoBloco << endl;
    cout << "Quantidade de pedaços: " << mCabecalho.qtdBlocos << endl;
    cout << "-----------------------------------------------------------" << endl << endl;
}

// imprime o número dos blocos contidos nas chaves que estão na árvore
// monstra a estrutura da árvore, os índices e as folhas
void sequenceSet::ImprimirArvore() {
    if(mCabecalho.qtdBlocos > 0) {
        umaArvore->imprimir(umaArvore->getRaiz());
    }
    else {
        throw runtime_error("Impossível imprimir, estrutura vazia");
    }
}

// a partir do menor noh da árvore, faz a impressão sequencial de todos os blocos do menor ao maior
void sequenceSet::ImprimirBlocos() {
    if(mCabecalho.qtdBlocos > 0) {
        Noh* percorrer = umaArvore->buscarMenorNoh();
        Bloco umBloco;

        // percorre até o final da árvore
        while(percorrer != nullptr) {
            // percorre o vetor com o número dos blocos
            for (int i = 0; i < percorrer->tamanho; i++) {
                umBloco = LerBlocoArquivo(percorrer->chave[i].numBloco);
                cout << "---------------------" << "Imprimir Bloco(" << umBloco.numBloco << ")" << "---------------------" << endl;
                for (int i = 0; i < umBloco.mTamanho; i++) {
                    cout << "Indice: " << umBloco.mPacote[i].indice << endl;
                    cout << "tempo: " << umBloco.mPacote[i].tempo << endl;
                    cout << "Origem: " << umBloco.mPacote[i].origem << endl;
                    cout << "Destino: " << umBloco.mPacote[i].destino << endl;
                    cout << "Protocolo: " << umBloco.mPacote[i].protocolo << endl;
                    cout << "Tamanho: " << umBloco.mPacote[i].tamanho << endl;
                    cout << "Informacao: " << umBloco.mPacote[i].infomarcao << endl;
                    cout << "-----------------------------------------------------------" << endl;
                }
                cout << "-----------------------------------------------------------" << endl;
                cout << "Número do Bloco: " << umBloco.numBloco << endl;
                cout << "Tamanho do Bloco: " << umBloco.mTamanho << endl;
                cout << "Proximo Bloco: " << umBloco.mProximo << endl;
                cout << "Bloco Anterior: " << umBloco.mAnterior << endl;
                cout << "-----------------------------------------------------------" << endl << endl;
            }
            // vai para o próximo vetor com os blocos
            percorrer = percorrer->ptr[percorrer->tamanho];
        }
    }
    else {
        throw runtime_error("Impossível imprimir, estrutura vazia");
    }
}