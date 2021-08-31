/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)
        
    Arquivo Principal
        (Arquivo onde comeca a execucao do programa,
         apresenta um menu com as opcoes que podem
         serem executadas e primeiros passos de 
         funcoes principais)
*/
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "sequenceSet.hpp"

using namespace std;

Pacote InicilizarPacote() { //  função para inicializar o pacote e evitar lixo.
    Pacote umPacote;
    umPacote.indice = 0;
    umPacote.tempo = 0;
    umPacote.origem[40] = ' ';
    umPacote.destino[40] = ' ';
    umPacote.protocolo[18] = ' ';
    umPacote.tamanho = 0;
    umPacote.infomarcao[1650] = ' ';
    return umPacote;   
}

bool VerificarInteiro(string umaString) {
    int contador = 0;
    while (umaString[contador] != '\0') {
        if((umaString[contador] >= 'a' && umaString[contador] <= 'z') ||
          (umaString[contador] >= 'A' && umaString[contador] <= 'Z')) {
            return false;
        }
        contador++;
    }
    return true;
}

Pacote CriarPacote(Pacote& umPacote, bool criar) { // função auxiliar para criação de um pacote.
    string umaString;
    char converter[20];
    char confirmacao;
    bool controle = false;

    while (not controle) {
        cout << "Deseja alterar o indicie? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite um número inteiro: ";
            while(not controle) {
                cin >> umaString;
                if(VerificarInteiro(umaString)){
                    strcpy(converter, umaString.c_str());
                    umPacote.indice = atoi(converter);
                    controle = true;
                }
                else {
                    cout << "Porfavor digite um número inteiro: ";
                }
            }
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    while (not controle) {
        cout << "Deseja alterar o tempo? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite um número decimal: ";
            while(not controle) {
                cin >> umaString;
                if(VerificarInteiro(umaString)){
                    strcpy(converter, umaString.c_str());
                    umPacote.tempo = atof(converter);
                    controle = true;
                }
                else {
                    cout << "Porfavor digite um número decimal: ";
                }
            }
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    if(criar) {
        while (not controle) {
            cout << "Deseja alterar a origem? s/n: ";
            cin >> confirmacao;
            if(confirmacao == 's') {
                cout << "Porfavor digite a origem: ";
                cin >> umaString;
                strcpy(umPacote.origem, umaString.c_str());
                controle = true;
            }
            else if (confirmacao == 'n') {
                controle = true;
            }
            system("clear");
        }
        controle = false;
    }

    while (not controle) {
        cout << "Deseja alterar o destino? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite o destino: ";
            cin >> umaString;
            strcpy(umPacote.destino, umaString.c_str());
            controle = true;
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    while (not controle) {
        cout << "Deseja alterar o protocolo? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite o protocolo: ";
            cin >> umaString;
            strcpy(umPacote.protocolo, umaString.c_str());
            controle = true;
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    while (not controle) {
        cout << "Deseja alterar o tamanho? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite um número inteiro: ";
            while(not controle) {
                cin >> umaString;
                if(VerificarInteiro(umaString)){
                    strcpy(converter, umaString.c_str());
                    umPacote.tamanho = atoi(converter);
                    controle = true;
                }
                else {
                    cout << "Porfavor digite um número inteiro: ";
                }
            }
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    while (not controle) {
        cout << "Deseja alterar a informação? s/n: ";
        cin >> confirmacao;
        if(confirmacao == 's') {
            cout << "Porfavor digite a informação: ";
            cin >> umaString;
            strcpy(umPacote.infomarcao, umaString.c_str());
            controle = true;
        }
        else if (confirmacao == 'n') {
            controle = true;
        }
        system("clear");
    }
    controle = false;

    return umPacote;
}

void ImprimirPacote(Pacote& umPacote) { // função para impressão de um pacote.
    cout << "\n----------------" << "PACOTE"  << "------------------------------------" << endl;
    cout << "|| Indice: " << umPacote.indice << endl;
    cout << "|| Tempo: " << umPacote.tempo << endl;
    cout << "|| Origem: " << umPacote.origem << endl;
    cout << "|| Destino: " << umPacote.destino << endl;
    cout << "|| Protocolo: " << umPacote.protocolo << endl;
    cout << "|| Tamanho: " << umPacote.tamanho << endl;
    cout << "|| Informacao: " << umPacote.infomarcao << endl;
    cout << "-----------------------------------------------------------" << endl << endl;
}

sequenceSet* criarEstrutura() {
    sequenceSet* umSet = new sequenceSet(true);
    cout << "Processando..." << endl;
    umSet->criarPeloBinario();

    return umSet;
}

sequenceSet* InserirCSV() { // função auxiliar para instanciar a sequenceSet e inserir o arquivo.csv.
    int inicio, fim;
    string umaString;
    char converter[20];
    bool controle = false;
    sequenceSet* umSet = new sequenceSet(false);
    while(not controle) {
        cout << "Digite o começo da leitura(número inteiro): ";
        cin >> umaString;
        if(VerificarInteiro(umaString)) {
            strcpy(converter, umaString.c_str());
            inicio = atoi(converter);
            controle = true;
        }
    }
    controle = false;

    while(not controle) {
        cout << "Digite o fim da leitura(número inteiro): ";
        cin >> umaString;
        if(VerificarInteiro(umaString)) {
            strcpy(converter, umaString.c_str());
            fim = atoi(converter);
            controle = true;
        }
    }
    system("clear");
    cout << "Processando..." << endl;
    umSet->LerArquivoCSV(inicio,fim);

    return umSet;
}

void Inserir(sequenceSet* umSet) { // função auxiliar para inserção de um pacote.
    Pacote umPacote = InicilizarPacote();
    umPacote = CriarPacote(umPacote, true);
    umSet->InserirPacote(umPacote);
}

void Buscar(sequenceSet* umSet) { // função auxiliar para buscar um pacote na sequenceSet.
    Pacote busca = InicilizarPacote();
    string umaString;
    char indice[20];
    bool controle = false;
    int posicao = 0;

    cout << "Por favor insira a chave primaria(origem): ";
    cin >> umaString;
    strcpy(busca.origem, umaString.c_str());
    while(not controle) {
        cout << "Por favor insira a chave secundaria(indice): ";
        cin >> umaString;
        if(VerificarInteiro(umaString)) {
            strcpy(indice, umaString.c_str());
            busca.indice = atoi(indice);
            controle = true;
        }
    }

    if(umSet->Buscar(busca)) {
        ImprimirPacote(busca);
    }
    else {
        throw runtime_error("Pacote não encontrado");
    }
}

void Alterar(sequenceSet* umSet) { // função auxiliar para alteração de um pacote.
    Pacote busca = InicilizarPacote();
    Pacote original;
    string umaString;
    char indice[20];
    bool controle = false;
    int posicao;

    cout << "Por favor insira a chave primaria(origem): ";
    cin >> umaString;
    strcpy(busca.origem, umaString.c_str());
    while(not controle) {
        cout << "Por favor insira a chave secundaria(indice): ";
        cin >> umaString;
        if(VerificarInteiro(umaString)) {
            strcpy(indice, umaString.c_str());
            busca.indice = atoi(indice);
            controle = true;
        }
    }

    if(umSet->Buscar(busca)) {
        original = busca;
        CriarPacote(busca, false);
        umSet->AlterarPacote(original, busca);
    }
    else {
        throw runtime_error("Pacote não encontrado");
    }
}

void Remover(sequenceSet* umSet) { // função auxiliar para remover um pacote da sequenceSet.
    Pacote remove = InicilizarPacote();
    string umaString;
    char indice[20];
    bool controle = false;

    cout << "Por favor insira a chave primaria(origem): ";
    cin >> umaString;
    strcpy(remove.origem, umaString.c_str()); 
    while(not controle) {
        cout << "Por favor insira a chave secundaria(indice): ";
        cin >> umaString;
        if(VerificarInteiro(umaString)) {
            strcpy(indice, umaString.c_str());
            remove.indice = atoi(indice);
            controle = true;
        }
    }
    umSet->RemoverPacote(remove);
}


void Menu() { // função para criar o a interface no terminal.
    cout << "============================================" << endl;
    cout << "|| [0]-> Reaproveitar arquivo já criado   ||" << endl;
    cout << "|| [1]-> Para ler o arquivo.csv e inserir ||" << endl;
    cout << "|| [2]-> Para inserir um pacote           ||" << endl;
    cout << "|| [3]-> Para buscar um pacote            ||" << endl;
    cout << "|| [4]-> Para alterar um pacote           ||" << endl;
    cout << "|| [5]-> Para remover um pacote           ||" << endl;
    cout << "|| [6]-> Para imprimir todos os blocos    ||" << endl;
    cout << "|| [7]-> Para imprimir o cabeçalho        ||" << endl;
    cout << "|| [8]-> Para imprimir a arvore           ||" << endl;
    cout << "|| [9]-> Para sair do programa            ||" << endl;
    cout << "============================================" << endl << endl;
    cout << "--> Sua opção? -> ";
}

int main() {
    system("clear");
    char opcao;
    char verifica;
    bool controle = false;
    sequenceSet* umSet;

    do {
        try {
            Menu();
            cin >> opcao;
            system("clear");
            switch (opcao) {
//===============================================================================================
                case '0' : // cria toda a estrutura em cima de um arquivo binario já existente
                    if(not controle) {
                        umSet = criarEstrutura();
                        system("clear");
                        cout << "Processo completado com sucesso!" << endl;
                        controle = true;
                    }
                    else {
                        throw runtime_error("Estratura já criada");
                    }
                break;
//===============================================================================================
                case '1' : // ler arquivo.csv e inserir
                    if(not controle) {
                        umSet = InserirCSV();
                        system("clear");
                        cout << "Processo completado com sucesso!" << endl;
                        controle = true;
                    }
                    else {
                        do {
                            if(controle) {
                                cout << "Se você continuar a estrutura antiga vai ser destruida, deseja continuar?" << endl;
                                cout << "Digite [1] para continuar" << endl;
                                cout << "Digite [2] para não alterar" << endl;
                                cout << "Sua opção? -> ";
                                cin >> verifica;
                            }
                            switch(verifica) {
                                case '1' : // destroi estrutura anterior e faz uma nova
                                    umSet->Resetar();
                                    umSet = InserirCSV();
                                    system("clear");
                                    cout << "Processo completado com sucesso!" << endl;
                                    controle = false;
                                    verifica = '2';
                                    break;
                                case '2' : // sai sem alterar nada da estrutura
                                // testa depois
                                break;
                            }
                        } while(verifica != '2');
                        controle = true;
                    }
                break;
//===============================================================================================
                case '2' : // inserir um pacote
                    Inserir(umSet);
                break;
//===============================================================================================
                case '3' : // buscar um pacote
                    Buscar(umSet);
                break;
//===============================================================================================
                case '4' : // alterar um pacote
                    Alterar(umSet);
                break;
//===============================================================================================
                case '5' : // remove um pacote
                    Remover(umSet);
                break;
//===============================================================================================
                case '6' : // imprime os blocos
                    umSet->ImprimirBlocos();
                break;   
//===============================================================================================
                case '7' : // imprime o cabeçalho
                    umSet->ImprimirCabecalho();
                break;
//===============================================================================================  
                case '8' : // imprimir a arvore
                    umSet->ImprimirArvore();
                break;
//===============================================================================================      
                case '9' : // sair do programa
                // testa depois;
                break;
//===============================================================================================    
                default:
                    cerr << "operação inválida\n";
                }
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
    } while (opcao != '9');

    if(controle) {
        delete umSet;
    }

    return 0;
}