/*
    Projeto Pratico Estruturas de Dados em Memoria Secundaria e Arvore B+
        (GCC 216 Estrutura de Dados)
		
    Classe Arvore B+
        (Arquivo com os atributos e metodos da classe, afim de 
		otimizar os processos de busca utilizando indices)
*/
#include <iostream>

using namespace std;

const int MAX = 4; // Tamanho máximo de cada noh
const int MIN = MAX/2; // Tamanho mínimo de cada noh

class Noh {
    friend class BPArvore;
	friend class sequenceSet;
    public:
	    Noh();
    private:
		// verifica se o noh é folha
        bool eFolha;
		// vetor de chaves que contem o número e a menor chave do bloco
		Chave *chave;
		// tamanho do vetor
        int tamanho;
		// vetor de ponteiros para ligar os vetores de chaves
        Noh** ptr;
};

class BPArvore {
    public:
		// construtor da árvore B+
        BPArvore();
		// destrutor da árvore B+
        ~BPArvore();
		// busca o bloco com possibilidade de o pacote estar
        int buscar(Pacote&);
		// busca o menor noh da árvore
		Noh* buscarMenorNoh();
		// insere a chave na árvore
        void inserir(Chave&);
		// divide os nohs folhas quando está cheio
		Noh* dividir(Chave&, Noh*);
		// altera uma chave na árvore
		void alterarChave(Chave&, Chave&);
		// remove uma chave da árvore
        bool remover(Chave&);
		// imprime a estrutura da árvore
        void imprimir(Noh*);
		// retorna a raiz
        Noh* getRaiz();
		// método para desalocar a árvore, usado no destrutor
        void limpar(Noh*);
    private:
        Noh *raiz;
		// insere uma chave em um noh não folha
        void inserirInterno(Chave&, Noh*, Noh*);
		// divide um noh não folha
		Noh* dividirInterno(Chave&, Noh*, Noh*);
		// encontra o pai de um noh
        Noh* encontrarPai(Noh*, Noh*);
		Noh* removerAux(Chave&, int&);
		Noh* encontrarAntecessor(Noh*, Noh*);
		Noh* encontrarSucessor(Noh*, Noh*);
};

// alocação dinamica na mémoria
Noh::Noh() {
	chave = new Chave[MAX];
	ptr = new Noh*[MAX+1];

	for(int i = 0; i < MAX+1; i++) {
		ptr[i] = NULL;
	}

	eFolha = true;
	tamanho = 0;
}
// inicializa a árvore
BPArvore::BPArvore() {
	raiz = NULL;
}
// destrutor da árvore, usa o método limpar para desalocar todos os nohs
BPArvore::~BPArvore() {
	//limpar(raiz);
}


// primeiramente ele percorre a árvore até chegar no noh com possibilidade de encontrar a chave
// depois percorre o vetor de chaves até encontrar o bloco com possibilidade de encontrar o pacote
// após encontrar o bloco, retorna o número do bloco para encontrar no arquivo binário na sequenceSet
int BPArvore::buscar(Pacote& umPacote) {
	// Busca Lógica
	if(raiz == NULL) {
		throw runtime_error("Árvore vazia");
	}
	else {
		Noh* nohPossivel = raiz;
		bool controle = false;
		int i;
		// vai percorrendo a arvore até encontrar o noh com possibilidade de encontrar o valor
		while(nohPossivel->eFolha == false) {
			i = 0;
			while(not controle && i < nohPossivel->tamanho) {
				if(umPacote < nohPossivel->chave[i]) {
					nohPossivel = nohPossivel->ptr[i];
					controle = true;
				}
				else if(i == nohPossivel->tamanho - 1) {
					nohPossivel = nohPossivel->ptr[i+1];
					controle = true;
				}
				i++;
			}
			controle = false;
		}
		// ao encontrar o noh com possibilidade de encontrar o bloco, buscar por ele no noh
		for(int i = 0; i < nohPossivel->tamanho; i++) {
			if(umPacote < nohPossivel->chave[i]) {
				return nohPossivel->chave[i-1].numBloco;
			}
			else if(umPacote > nohPossivel->chave[i] && i == nohPossivel->tamanho-1) {
				return nohPossivel->chave[i].numBloco;
			}
			else if(umPacote == nohPossivel->chave[i]) {
				return nohPossivel->chave[i].numBloco;
			}
		}
		throw runtime_error("Problema ao encontrar chave");
	}
}

// busca o noh com as menores chaves da árvore
Noh* BPArvore::buscarMenorNoh() {
	Noh* menorNoh = raiz;

	while(menorNoh->eFolha == false) {
		menorNoh = menorNoh->ptr[0];
	}

	return menorNoh;
}

void BPArvore::inserir(Chave& umaChave) {
	// inserção lógica
	if(raiz == NULL) {
		raiz = new Noh;
		raiz->chave[0] = umaChave;
		raiz->eFolha = true;
		raiz->tamanho = 1;
	}
	else {
		bool controle = false;
		int i;
		Noh* nohPossivel = raiz;
		Noh* nohPai;
		// vai percorrendo a arvore até encontrar o noh com possibilidade de encontrar a chave, salva o noh anterior também
		while(nohPossivel->eFolha == false) {
			i = 0;
			nohPai = nohPossivel;
			while(not controle && i < nohPossivel->tamanho) {
				if(umaChave < nohPossivel->chave[i]) {
					nohPossivel = nohPossivel->ptr[i];
					controle = true;
				}
				else if(i == nohPossivel->tamanho - 1) {
					nohPossivel = nohPossivel->ptr[i+1];
					controle = true;
				}
				i++;
			}
			controle = false;
		}
		// agora estamos no noh, vamos inserir o novo valor
		if(nohPossivel->tamanho < MAX) { // Se o noh não está cheio
			i = 0;
			// achando a posição para inserção da nova chave
			while(umaChave > nohPossivel->chave[i] && i < nohPossivel->tamanho) i++;	
			// criando espaço para a nova chave
			for(int j = nohPossivel->tamanho; j > i ; j--) {
				nohPossivel->chave[j] = nohPossivel->chave[j-1];
			}
			nohPossivel->chave[i] = umaChave;
			nohPossivel->tamanho++;
			nohPossivel->ptr[nohPossivel->tamanho] = nohPossivel->ptr[nohPossivel->tamanho-1];
			nohPossivel->ptr[nohPossivel->tamanho-1] = NULL;
		}
		else { // se o noh está cheio
			Noh* novoNohFolha = dividir(umaChave, nohPossivel);

			// altera o pai
			if(nohPossivel == raiz) { // se o nohPossivel é a raiz, criamos uma nova raiz
				Noh* novoNohRaiz = new Noh;
				novoNohRaiz->chave[0] = novoNohFolha->chave[0];
				novoNohRaiz->ptr[0] = nohPossivel;
				novoNohRaiz->ptr[1] = novoNohFolha;
				novoNohRaiz->eFolha = false;
				novoNohRaiz->tamanho = 1;
				raiz = novoNohRaiz;
			}
			else { // insere o novo valor no nohPai
				inserirInterno(novoNohFolha->chave[0], nohPai, novoNohFolha);
			}
		}
	}
}

Noh* BPArvore::dividir(Chave& umaChave, Noh* nohPossivel) {
	// criando nova folha
	Noh* novoNohFolha = new Noh;
	// criando noh virtual e inserimos as chaves nele
	Chave nohVirtual[MAX+1];
	for(int i = 0; i < MAX; i++) {
		nohVirtual[i] = nohPossivel->chave[i];
	}
	int i = 0, j = 0;
	// achando a posição para inserção da nova chave
	while(umaChave > nohVirtual[i] && i < MAX) i++;
	// criando espaço para a nova chave
	for(int j = (MAX); j > i; j--) {
		nohVirtual[j] = nohVirtual[j-1];
	}
	nohVirtual[i] = umaChave; 
	novoNohFolha->eFolha = true;
	// divide o nohPossivel em dois nohs folhas
	nohPossivel->tamanho = (MAX+1)/2;
	novoNohFolha->tamanho = (MAX+1)-(MAX+1)/2;
	// faça o ponteiro de nohPossivel apontar para o novoNohFolha
	nohPossivel->ptr[nohPossivel->tamanho] = novoNohFolha;
	// faça o ponteiro do novoNohFolha aponatar para o próximo noh folha
	if(nohPossivel->ptr[MAX] != NULL) {
		novoNohFolha->ptr[novoNohFolha->tamanho] = nohPossivel->ptr[MAX];
	}
	nohPossivel->ptr[MAX] = NULL;
	// agora passamos os elementos para os novos nohs folha
	for(i = 0; i < nohPossivel->tamanho; i++) {
		nohPossivel->chave[i] = nohVirtual[i];
	}

	i = 0, j = nohPossivel->tamanho;

	while(i < novoNohFolha->tamanho) {
		novoNohFolha->chave[i] = nohVirtual[j];
		i++, j++;
	}
	return novoNohFolha;
}

// altera uma chave na árvore, utilizada quando a menor chave de um bloco é alterada
void BPArvore::alterarChave(Chave& chaveAntiga, Chave& chaveNova) {
	Noh* nohPossivel = raiz;
	bool controle = false;
	int i;
	// vai percorrendo a arvore até encontrar o noh com possibilidade de encontrar o valor
	while(nohPossivel->eFolha == false) {
		i = 0;
		while(not controle && i < nohPossivel->tamanho) {
			if(chaveAntiga < nohPossivel->chave[i]) {
				nohPossivel = nohPossivel->ptr[i];
				controle = true;
			}
			else if(i == nohPossivel->tamanho - 1) {
				nohPossivel = nohPossivel->ptr[i+1];
				controle = true;
			}
			i++;
		}
		controle = false;
	}
	i = 0;
	// ao encontrar o noh com possibilidade de encontrar a chave, buscar por ele no noh e faz a alteração
	while(not controle && i < nohPossivel->tamanho) {
		if(chaveAntiga == nohPossivel->chave[i]) {
			nohPossivel->chave[i] = chaveNova;
			controle = true;
		}
		i++;
	}
}

void BPArvore::inserirInterno(Chave& umaChave, Noh* nohPai, Noh* nohFilho) {
	if(nohPai->tamanho < MAX) { // se o nohPai não está cheio
		int i = 0;
		// achando a posição para inserção da nova chave
		while(umaChave > nohPai->chave[i] && i < nohPai->tamanho) i++;
		// criando espaço para o novo valor
		for(int j = nohPai->tamanho;j > i; j--) {
			nohPai->chave[j] = nohPai->chave[j-1];
		}
		// criando espaço para o novo ponteiro
		for(int j = nohPai->tamanho+1; j > i+1; j--) {
			nohPai->ptr[j] = nohPai->ptr[j-1];
		}
		nohPai->chave[i] = umaChave;
		nohPai->tamanho++;
		nohPai->ptr[i+1] = nohFilho;
	}
	else { // se o noh interno está cheio
		Noh* novoNohInterno = dividirInterno(umaChave, nohPai, nohFilho);

		if(nohPai == raiz) { // se o nohPai é a raiz, criamos uma nova raiz
			Noh* novoNohRaiz = new Noh;
			novoNohRaiz->chave[0] = nohPai->chave[nohPai->tamanho];
			novoNohRaiz->ptr[0] = nohPai;
			novoNohRaiz->ptr[1] = novoNohInterno;
			novoNohRaiz->eFolha = false;
			novoNohRaiz->tamanho = 1;
			raiz = novoNohRaiz;
		}
		else { // recursão
			// busca em profundidade para encontrar o pai do nohPai
			inserirInterno(nohPai->chave[nohPai->tamanho], encontrarPai(raiz,nohPai), novoNohInterno);
		}
	}
}

// divide o nohs internos, ou seja, que não são folhas
Noh* BPArvore::dividirInterno(Chave& umaChave, Noh* nohPai, Noh* nohFilho) {
	// criando novo noh interno
	Noh* novoNohInterno = new Noh;
	// crindo um novo noh interno virtual;
	Chave chaveVirtual[MAX+1];
	Noh* ptrVirtual[MAX+2];
	for(int i = 0; i < MAX; i++) {
		chaveVirtual[i] = nohPai->chave[i];
	}
	for(int i = 0; i < MAX+1; i++) {
		ptrVirtual[i] = nohPai->ptr[i];
	}
	int i = 0, j = 0;
	// achando a posição para inserção da nova chave
	while(umaChave > chaveVirtual[i] && i < MAX) i++;
	// criando espaço para o novo valor
	for(int j = (MAX+1)-1;j > i; j--) {
		chaveVirtual[j] = chaveVirtual[j-1];
	}
	chaveVirtual[i] = umaChave; 
	// criando espaço para o novo ponteiro
	for(int j = MAX+2;j > i+1; j--) {
		ptrVirtual[j] = ptrVirtual[j-1];
	}
	ptrVirtual[i+1] = nohFilho; 
	novoNohInterno->eFolha = false;
	// Dividir o nohPai em dois nohs
	nohPai->tamanho = (MAX+1)/2;
	novoNohInterno->tamanho = (MAX)-(MAX+1)/2;
	// Passando os elementos para novo noh
	i = 0, j = nohPai->tamanho+1;
	while(i < novoNohInterno->tamanho) {
		novoNohInterno->chave[i] = chaveVirtual[j];
		i++, j++;
	}
	// Passando os ponteiros para os novo noh
	i = 0, j = nohPai->tamanho+1;
	while(i < novoNohInterno->tamanho+1) {
		novoNohInterno->ptr[i] = ptrVirtual[j];
		i++, j++;
	}
	return novoNohInterno;
}

// Encontra o pai usando busca em profundidade e ignora o nohs folha, pois eles não podem ser pais
// Também ignora o segundo último nível, porque nunca encontraremos o pai de um noh folha dura a inserção usando esse método
Noh* BPArvore::encontrarPai(Noh* nohAux, Noh* nohFilho) {
	Noh* nohPai;

	if (nohAux->eFolha){
		return nullptr;
	}
	for(int i = 0; i < nohAux->tamanho+1; i++) {
		if(nohAux->ptr[i] == nohFilho) {
			nohPai = nohAux;
			return nohPai;
		}
		else {
			nohPai = encontrarPai(nohAux->ptr[i], nohFilho);
			if(nohPai != NULL) {
                return nohPai;
            }
		}
	}
	return nohPai;
}

// remove um bloco
bool BPArvore::remover(Chave& umaChave){
	if (raiz == NULL){
        throw runtime_error("Impossível remover, árvore vazia");
	}

	int p = 0;
	Noh* nohPossivel = removerAux(umaChave, p);
	Noh* pai = encontrarPai(raiz, nohPossivel);

	// remove a chave
    for (int i=p; i<nohPossivel->tamanho; i++){
		nohPossivel->chave[i] = nohPossivel->chave[i+1];
	}
	nohPossivel->tamanho--;

	// atualizar o ponteiro do próximo noh folha
	nohPossivel->ptr[nohPossivel->tamanho] = nohPossivel->ptr[nohPossivel->tamanho+1];
	nohPossivel->ptr[nohPossivel->tamanho+1] = NULL;
	
	// verifica se o nohPossivel nao e a raiz para poder atualizar o pai
	if (pai != NULL){
		for (int i=0; i<pai->tamanho; i++){
			if (pai->chave[i] == umaChave){
				pai->chave[i] = nohPossivel->chave[0];
			}
		}
	}

	// verifica se o nohPossivel e a raiz
	if (nohPossivel == raiz){
		if (nohPossivel->tamanho == 0){
			raiz = NULL;
		}
		return true;
	}

	// se após a remoção o tamanho do nohPossivel continua sendo maior que a metade
    if (nohPossivel->tamanho >= (MAX+1)/2){
        return true;
    }
	
	// se após a remoção o tamanho do nohPossivel for menor que o minino a arvore precisa de tratamento
    else{
        Noh* antecessor = encontrarAntecessor(pai, nohPossivel);
        Noh* sucessor = encontrarSucessor(pai, nohPossivel);

		/* Verifica se o nohPossivel tem um antecessor
		se tiver, pega a maior chave do antecessor e atualiza o pai */
        if ((antecessor != NULL) && (antecessor->tamanho > (MIN))){
			// move as chaves do nohPossivel para poder inserir a chave recebida pelo antecessor
            for (int i=nohPossivel->tamanho; i>0; i--){
                nohPossivel->chave[i] = nohPossivel->chave[i-1];
            }
            nohPossivel->chave[0] = antecessor->chave[antecessor->tamanho-1];
			// atualiza o tamanho e ponteiros dos nohs
            nohPossivel->tamanho++;
            antecessor->tamanho--;
            antecessor->ptr[antecessor->tamanho] = nohPossivel;
            nohPossivel->ptr[nohPossivel->tamanho] = sucessor;
            

            int i = 0;
			// procura a posicao no pai que vai ser atualizada
            while (pai->chave[i] < nohPossivel->chave[1]) i++;

			// atualiza pai
			if(i == 0) {
            	pai->chave[i] = nohPossivel->chave[0];
			}
			else {
				pai->chave[i-1] = nohPossivel->chave[0];
			}

        }
		/* O nohPossivel nao tem um antecessor ou o antecessor nao pode dar a chave
		verifica se o nohPossivel tem um sucessor
		se tiver, pega a menor chave do antecessor e atualiza o pai */
        else if ((sucessor != NULL) && (sucessor->tamanho > (MIN))){
			// o nohPossivel recebe a menor chave do sucessor
            nohPossivel->chave[1] = sucessor->chave[0];
			// arruma o sucessor após a remoção da menor chave
            for (int i=0; i<sucessor->tamanho; i++){
                sucessor->chave[i] = sucessor->chave[i+1];
            }
			// ataliza o tamanho e os ponteiros dos nohs
			nohPossivel->tamanho++;
            sucessor->tamanho--;
            nohPossivel->ptr[nohPossivel->tamanho] = sucessor;
            sucessor->ptr[sucessor->tamanho] = sucessor->ptr[sucessor->tamanho+1];

            int i = 0;
			// procura a posicao no pai que vai ser atualizada
            while (pai->chave[i] < nohPossivel->chave[1]) i++;
			// atualiza pai
            pai->chave[i] = sucessor->chave[0];

        }
		// verifica se o nohPossivel tem um antecessor para fazer a fusão dos nohs
        else if (antecessor != NULL){
            // antecessor recebe todas as chaves de nohPossivel
			antecessor->chave[antecessor->tamanho] = nohPossivel->chave[0];
			antecessor->tamanho++;
			// se o tamanho de pai for 1 entao antecessor se torna o novo pai e pai se torna folha e deleta o nohPossivel
			if (pai->tamanho == 1){
				// se o pai for a raiz, então a raiz se torna o antecessor
				if (pai == raiz){
					raiz = antecessor;
				}
				pai = antecessor;
				pai->eFolha;
				delete[] nohPossivel->chave;
				delete[] nohPossivel->ptr;
				delete nohPossivel;
				
				return true;
			}
			// se nohPossivel tiver um sucessor precisa atualiza as chaves do pai
			if (sucessor != NULL){

				int j = 0;
				// procura a posicao no pai que vai ser atualizada
				while (pai->chave[j] < umaChave) j++;
				// atualiza as chaves e os ponteiros do pai
				for (int i=j; i<pai->tamanho; i++){
					pai->chave[i] = pai->chave[i+1];
				}
				for (int i=j+1; i<pai->tamanho+1; i++){
					pai->ptr[i] = pai->ptr[i+1];
				}
			}
			pai->tamanho--;
			// sucessor do nohPossivel se torna sucessor do antecessor
			antecessor->ptr[antecessor->tamanho] = sucessor;
			//deleta o nohPossivel
            delete[] nohPossivel->chave;
            delete[] nohPossivel->ptr;
            delete nohPossivel;
        }
        /* nohPossivel nao tem um antecessor, entao faz a junçao com o sucessor dele */
        else{
			int j = 0;
            while (pai->chave[j] < sucessor->chave[0]) j++;
			
            for (int i=0; i<sucessor->tamanho; i++){
                nohPossivel->chave[nohPossivel->tamanho] = sucessor->chave[i];
                nohPossivel->tamanho++;
            }
			// se o tamanho de pai for 1 entao nohPossivel se torna o novo pai e pai se torna folha e deleta o sucessor
			if (pai->tamanho == 1){
				// se o pai for a raiz, então a raiz se torna o nohPossivel
				if (pai == raiz){
					raiz = nohPossivel;
				}
				pai = nohPossivel;
				pai->eFolha;

				delete[] sucessor->chave;
				delete[] sucessor->ptr;
				delete sucessor;
				return true;
			}
			// atualiza as chaves e os ponteiros do pai
			for (int i=j; i<pai->tamanho; i++){
                pai->chave[i] = pai->chave[i+1];
            }

            for (int i=j+1; i<pai->tamanho+1; i++){
                pai->ptr[i] = pai->ptr[i+1];
            }
			// sucessor do sucessor do nohPossivel se torna sucessor do nohPossivel
			nohPossivel->ptr[nohPossivel->tamanho] = sucessor->ptr[sucessor->tamanho];

			pai->tamanho--;
			//deleta o sucessor
            delete[] sucessor->chave;
            delete[] sucessor->ptr;
            delete sucessor;
        }
    }
	return true;
}
// percorre a árvore até chegar no noh com possibilidade de encontrar a chave
// após encontrar, retorna o noh e atualiza a posicao da chave buscada
Noh* BPArvore::removerAux(Chave& umaChave, int& pos) {
	if(raiz == nullptr) {
		throw runtime_error("Árvore vazia");
	}
	else {
		Noh* nohPossivel = raiz;
		bool controle = false;
		int i;
		// vai percorrendo a arvore até encontrar o noh com possibilidade de encontrar o valor
		while(nohPossivel->eFolha == false) {
			i = 0;
			while(not controle && i < nohPossivel->tamanho) {
				if(umaChave < nohPossivel->chave[i]) {
					nohPossivel = nohPossivel->ptr[i];
					controle = true;
				}
				else if(i == nohPossivel->tamanho - 1) {
					nohPossivel = nohPossivel->ptr[i+1];
					controle = true;
				}
				i++;
			}
			controle = false;
		}
		// ao encontrar o noh com possibilidade de encontrar o bloco, buscar por ele no noh
		for(int i = 0; i < nohPossivel->tamanho; i++) {
			if(umaChave < nohPossivel->chave[i]) {
				pos = i-1;
				return nohPossivel;
			}
			else if(umaChave > nohPossivel->chave[i] && i == nohPossivel->tamanho-1) {
				pos = i;
				return nohPossivel;
			}
			else if(umaChave == nohPossivel->chave[i]) {
				pos = i;
				return nohPossivel;
			}
		}
		throw runtime_error("Problema ao encontrar chave");
	}
}

Noh* BPArvore::encontrarSucessor(Noh* pai, Noh* nohPossivel){
	for(int i = 0; i < pai->tamanho+1; i++) {
		if(pai->ptr[i] == nohPossivel) {
			if (i == pai->tamanho){
				return nullptr;
			}
			return pai->ptr[i+1];	
		}
	}

	return nullptr;
}

Noh* BPArvore::encontrarAntecessor(Noh* pai, Noh* nohPossivel){
	for(int i = 0; i < pai->tamanho+1; i++) {
		if(pai->ptr[i] == nohPossivel) {
			if (i == 0){
				return nullptr;
			}
			return pai->ptr[i-1];	
		}
	}

	return nullptr;
}

// faz a impressão da estrutura da árvore, os valores impridos são os números dos blocos
void BPArvore::imprimir(Noh* percorrer) {
    bool controle = false;
	if(percorrer != NULL) {
		for(int i = 0; i < percorrer->tamanho; i++) {
            if(not controle && not percorrer->eFolha) {
                cout << "Indice-> ";
                controle = true;
            }
            else if(not controle && percorrer->eFolha) {
                cout << "Folha-> ";
                controle = true;
            }
			    cout << percorrer->chave[i].numBloco << " ";
		}
		cout << endl;
		if(percorrer->eFolha != true) {
			for(int i = 0; i < percorrer->tamanho+1; i++) {
				imprimir(percorrer->ptr[i]);
			}
		}
	}
}

// retorna a raiz
Noh* BPArvore::getRaiz() {
	return raiz;
}

/*
// utilizado no destrutor, vai desalocando os nohs da árvore
void BPArvore::limpar(Noh* nohPercorrer) {
	if(nohPercorrer != NULL) {
		if(nohPercorrer->eFolha != true) {
			for(int i = 0; i < nohPercorrer->tamanho+1; i++) {
				limpar(nohPercorrer->ptr[i]);
			}
		}
		for(int i = 0; i < nohPercorrer->tamanho; i++) {
			cout << "Valor deletado da mémoria: " << nohPercorrer->chave[i].numBloco << endl;
		}
		delete[] nohPercorrer->chave;
		delete[] nohPercorrer->ptr;
		delete nohPercorrer;
	}
}
*/