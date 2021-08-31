/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)

    Metodos de sobrecarga
        (Arquivo que contem as sobrecargas de metodo, usado nas comparacoes
        envolvendo chaves primarias e secundarias de pacotes e chaves)
*/
#include <iostream>
#include <cstring>

using namespace std;

// compara dois vetores de char e verifica se a primeira é maior que a segunda.
// caso a chave primaria(origem) seja igual, ele verifica uma chave secundaria(indice).
bool operator>(const Pacote& pacoteUm, const Pacote& pacoteDois) {
    if(strcmp(pacoteUm.origem, pacoteDois.origem) > 0) {
        return true;
    }
    else if(strcmp(pacoteUm.origem, pacoteDois.origem) < 0) {
        return false;
    }
    else {
        if(pacoteUm.indice > pacoteDois.indice) {
            return true;
        }
        else {
            return false;
        }
    }
}

// compara duas chaves, primaria e secundaria, e se ambas forem iguais retorna verdadeiro.
bool operator==(const Pacote& pacoteUm, const Pacote& pacoteDois) {
    if ((strcmp(pacoteUm.origem, pacoteDois.origem) == 0) and (pacoteUm.indice == pacoteDois.indice)) {
        return true;
    }
    else {
        return false;
    }
}

// Compara um tipo Pacote com um tipo Chave, retorna true se o primeira referência é menor que a segunda
bool operator<(const Pacote& umPacote, const Chave& umaChave) {
     if(strcmp(umPacote.origem, umaChave.origem) < 0) {
        return true;
    }
    else if(strcmp(umPacote.origem, umaChave.origem) > 0) {
        return false;
    }
    else {
        if(umPacote.indice < umaChave.indice) {
            return true;
        }
        else {
            return false;
        }
    }
}

// Compara um tipo Pacote com um tipo Chave, retorna true se o primeira referência é maior que a segunda
bool operator>(const Pacote& umPacote, const Chave& umaChave) {
    if(strcmp(umPacote.origem, umaChave.origem) > 0) {
        return true;
    }
    else if(strcmp(umPacote.origem, umaChave.origem) < 0) {
        return false;
    }
    else {
        if(umPacote.indice > umaChave.indice) {
            return true;
        }
        else {
            return false;
        }
    }
}

// Compara um tipo Pacote com um tipo Chave, retorna true se o ambas as chaves forem iguais
bool operator==(const Pacote& umPacote, const Chave& umaChave) {
    if ((strcmp(umPacote.origem, umaChave.origem) == 0) and (umPacote.indice == umaChave.indice)) {
        return true;
    }
    else {
        return false;
    }
}

// Compara dois tipos Chave, retorna true se o primeira referência é menor que a segunda
bool operator<(const Chave& chaveUm, const Chave& chaveDois) {
     if(strcmp(chaveUm.origem, chaveDois.origem) < 0) {
        return true;
    }
    else if(strcmp(chaveUm.origem, chaveDois.origem) > 0) {
        return false;
    }
    else {
        if(chaveUm.indice < chaveDois.indice) {
            return true;
        }
        else {
            return false;
        }
    }
}

// Compara dois tipos Chave, retorna true se o primeira referência é maior que a segunda
bool operator>(const Chave& chaveUm, const Chave& chaveDois) {
    if(strcmp(chaveUm.origem, chaveDois.origem) > 0) {
        return true;
    }
    else if(strcmp(chaveUm.origem, chaveDois.origem) < 0) {
        return false;
    }
    else {
        if(chaveUm.indice > chaveDois.indice) {
            return true;
        }
        else {
            return false;
        }
    }
}

// Compara dois tipos Chave, retorna true se as chaves são iguais
bool operator==(const Chave& chaveUm, const Chave& chaveDois) {
     if ((strcmp(chaveUm.origem, chaveDois.origem) == 0) and (chaveUm.indice == chaveDois.indice)) {
        return true;
    }
    else {
        return false;
    }
}
