#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum COR
{
    Vermelho,
    Preto
};

typedef struct estrutura
{
    int valor;
    struct estrutura *dir;
    struct estrutura *esq;
    struct estrutura *pai;
    enum COR cor;
} No;

typedef struct arvore
{
    No *raiz;
    No *NIL;
} ARVORE_RUBRO_NEGRA;

No *criarNoARB(int valor)
{
    No *n = malloc(sizeof(No));
    n->esq = NULL;
    n->dir = NULL;
    n->pai = NULL;
    n->valor = valor;
    n->cor = Vermelho;

    return n;
}

ARVORE_RUBRO_NEGRA *criaARB()
{
    ARVORE_RUBRO_NEGRA *t = malloc(sizeof(ARVORE_RUBRO_NEGRA));
    No *nil_no = malloc(sizeof(No));
    nil_no->esq = NULL;
    nil_no->dir = NULL;
    nil_no->pai = NULL;
    nil_no->cor = Preto;
    nil_no->valor = 0;
    t->NIL = nil_no;
    t->raiz = t->NIL;

    return t;
}

No* buscaARB(ARVORE_RUBRO_NEGRA* arv, int valor) {
    // Percorre a árvore a partir da raiz.
    while (arv->raiz != arv->NIL && arv->raiz->valor != valor) {
        if (valor < arv->raiz->valor)
            arv->raiz = arv->raiz->esq;
        else
            arv->raiz = arv->raiz->dir;
    }
    
    // Verifica se o valor foi encontrado ou não.
    if (arv->raiz == arv->NIL) {
        printf("\nValor nao encontrado na arvore.\n");
        return NULL;
    } else {
        printf("Valor encontrado na arvore.\n");
        return arv->raiz;
    }
}

bool ehFilhoEsquerdo(No *no) {
    return no == no->pai->esq;
}

void rotacaoEsquerda(ARVORE_RUBRO_NEGRA *arv, No *no)
{   
    No *dir = no->dir;
    no->dir = dir->esq;

    if (dir->esq != arv->NIL)
        dir->esq->pai = no;
    dir->pai = no->pai;

    if (no->pai == arv->NIL) // Nó é raiz.
        arv->raiz = dir;
    else if (ehFilhoEsquerdo(no)) // Nó é filho esquerdo.
        no->pai->esq = dir;
    else // Nó é filho direito.
        no->pai->dir = dir;

    dir->esq = no;
    no->pai = dir;
}

void rotacaoDireita(ARVORE_RUBRO_NEGRA *arv, No *no)
{
    No *esq = no->esq;
    no->esq = esq->dir;

    if (esq->dir != arv->NIL)
        esq->dir->pai = no;
    esq->pai = no->pai;

    if (no->pai == arv->NIL) // Nó é raiz.
        arv->raiz = esq;
    else if (no == no->pai->dir) // Nó é filho direito.
        no->pai->dir = esq;
    else // Nó é filho esquerdo.
        no->pai->esq = esq;

    esq->dir = no;
    no->pai = esq;
}

void balancearInsercao(ARVORE_RUBRO_NEGRA *arv, No *no) {
    bool foiBalanceado = false;
    while (no->pai->cor == Vermelho) {   
        No * pai = no->pai;
        No * avo = no->pai->pai;
        if (ehFilhoEsquerdo(pai)) // Nó pai é filho esquerdo do avô.
        {
            No *tio = avo->dir;
            if (tio->cor == Vermelho) // (Caso 1) Tio também é vermelho.
            {
                printf("-> Foi balanceado com o Caso 1.\n\n");
                pai->cor = Preto;
                tio->cor = Preto;
                avo->cor = Vermelho;
                no = avo;
            }
            else { // (Caso 2) Tio é preto.
                bool ehRotacaoDupla = false;
                printf("-> Foi balanceado com o Caso 2");
                if (!ehFilhoEsquerdo(no)) // (Rotação dupla) Nó é filho direito do pai.
                { 
                    ehRotacaoDupla = true;
                    printf(" (Rotacao dupla - Esq -> Dir)");
                    no = pai;
                    rotacaoEsquerda(arv, no);
                }
                printf(!ehRotacaoDupla ? " (Rotacao Simples).\n\n" : ".\n\n");
                pai = no->pai;
                // (Rotação Simples) Nó é filho esquerdo do pai.
                pai->cor = Preto;       
                avo->cor = Vermelho;
                rotacaoDireita(arv, avo);
            }
        }
        else // Nó pai é filho direito do avô.
        {
            No *tio = avo->esq;
            if (tio->cor == Vermelho) // (Caso 1) Tio também é vermelho.
            {
                printf("-> Foi balanceado com o Caso 1.\n\n");
                pai->cor = Preto;
                tio->cor = Preto;
                avo->cor = Vermelho;
                no = avo;
            }
            else // (Caso 2) Tio é preto.
            {
                bool ehRotacaoDupla = false;
                printf("-> Foi balanceado com o Caso 2");
                if (ehFilhoEsquerdo(no)) // (Rotação dupla) Nó é filho esquerdo do pai.
                {   
                    ehRotacaoDupla = true;
                    printf(" (Rotacao dupla - Dir -> Esq)");
                    no = pai;
                    rotacaoDireita(arv, no);
                }
                printf(!ehRotacaoDupla ? " (Rotacao Simples).\n\n" : ".\n\n");
                pai = no->pai;
                // (Rotação Simples) Nó é filho direito do pai.
                pai->cor = Preto;
                avo->cor = Vermelho;
                rotacaoEsquerda(arv, avo);
            }
        }
        foiBalanceado = true;
    }
    arv->raiz->cor = Preto;
    printf(!foiBalanceado ? "-> Sem balanceamento.\n\n": "");
}

void inserir(ARVORE_RUBRO_NEGRA *arv, No *NoNovo) 
{
    No *paiNovoNo = arv->NIL;
    No *NoAtual = arv->raiz;

    // Encontra o pai do novo nó.
    while (NoAtual != arv->NIL) {
        paiNovoNo = NoAtual;
        if (NoNovo->valor < NoAtual->valor)
            NoAtual = NoAtual->esq;
        else
            NoAtual = NoAtual->dir;
    }

    NoNovo->pai = paiNovoNo;

    // Se árvore vazia, o novo nó é a raiz.
    if (paiNovoNo == arv->NIL)
        arv->raiz = NoNovo;
    else if (NoNovo->valor < paiNovoNo->valor) // Se for menor que seu pai adiciona a esquerda.
        paiNovoNo->esq = NoNovo;
    else // Se for maior que seu pai adiciona a direita.
        paiNovoNo->dir = NoNovo;

    NoNovo->dir = arv->NIL;
    NoNovo->esq = arv->NIL;

    printf("Valor %d inserido!\n", NoNovo->valor);

    balancearInsercao(arv, NoNovo);
}

void transplantarSubArvore(ARVORE_RUBRO_NEGRA *arv, No *no, No *filho) 
{
    if (no->pai == arv->NIL) // Se o pai do nó for nulo.
        arv->raiz = filho;
    else if (ehFilhoEsquerdo(no))
        no->pai->esq = filho;
    else
        no->pai->dir = filho;
    filho->pai = no->pai;
}

No *obterSucessor(ARVORE_RUBRO_NEGRA *arv, No *no)
{
    while (no->esq != arv->NIL)
        no = no->esq;
    return no;
}

bool filhosPretos(No *no) {
    return no->esq->cor == Preto && no->dir->cor == Preto;
}

void balancearRemocao(ARVORE_RUBRO_NEGRA *arv, No *no)
{
    while (no != arv->raiz && no->cor == Preto) // Nó diferente da raiz e nó é preto.
    { 
        if (ehFilhoEsquerdo(no)) // Nó é filho esquerdo do pai.
        {
            No *irmao = no->pai->dir;
            if (irmao->cor == Vermelho) // (Caso 1) Irmão é vermelho
            {
                printf("-> Foi removido com o Caso 1.\n\n");
                no->pai->cor = Vermelho; // Pai fica vermelho.
                irmao->cor = Preto; // Irmão fica preto.

                rotacaoEsquerda(arv, no->pai);

                irmao = no->pai->dir; // Atualiza o irmão.
            }
            // (Caso 2) O irmão e seus filhos são pretos.
            if (filhosPretos(irmao))
            {
                printf("-> Foi removido com o Caso 2.\n\n");
                irmao->cor = Vermelho; // Irmão fica vermelho.
                no = no->pai; // Atualiza o nó.
            }
            else // (Caso 3) Filho esquerdo é vermelho e direito é preto.
            { 
                printf("-> Foi removido com o Caso ");
                bool ehCasoTresQuatro = false;
                if (irmao->dir->cor == Preto)
                {
                    ehCasoTresQuatro = true;
                    irmao->esq->cor = Preto; // Filho esquerdo do irmão fica preto.
                    irmao->cor = Vermelho; // Irmão fica vermelho.

                    rotacaoDireita(arv, irmao);

                    irmao = no->pai->dir; // Atualiza o irmão.
                }
                // (Caso 4) Filho direito é vermelho.
                printf(!ehCasoTresQuatro ? "4.\n\n": "3 e 4.\n\n");
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->dir->cor = Preto;

                rotacaoEsquerda(arv, no->pai);
                no = arv->raiz;
            }
        }
        else // Nó é filho direito do pai.
        {
            No *irmao = no->pai->esq;
            if (irmao->cor == Vermelho) // (Caso 1) Irmão é vermelho
            {
                printf("-> Foi removido com o Caso 1.\n\n");
                no->pai->cor = Vermelho; // Pai fica vermelho.
                irmao->cor = Preto; // Irmão fica preto.

                rotacaoDireita(arv, no->pai);
                irmao = no->pai->esq; // Atualiza o irmão.
            }
            // (Caso 2) O irmão e seus filhos são pretos.
            if (filhosPretos(irmao))
            {
                printf("-> Foi removido com o Caso 2.\n\n");
                irmao->cor = Vermelho; // Irmão fica vermelho.
                no = no->pai; // Atualiza o nó.
            }
            else // (Caso 3) Filho esquerdo é vermelho e direito é preto.
            {   
                printf("-> Foi removido com o Caso ");
                bool ehCasoTresQuatro = false;
                if (irmao->esq->cor == Preto)
                {
                    ehCasoTresQuatro = true;
                    irmao->dir->cor = Preto; // / Filho direito do irmão fica preto.
                    irmao->cor = Vermelho; // Irmão fica vermelho.

                    rotacaoEsquerda(arv, irmao);

                    irmao = no->pai->esq; // Atualiza o irmão.
                }
                // (Caso 4) Filho direito é vermelho.
                printf(!ehCasoTresQuatro ? "4.\n\n": "3 e 4.\n\n");
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esq->cor = Preto;

                rotacaoDireita(arv, no->pai);
                no = arv->raiz;
            }
        }
    }
    no->cor = Preto;
}

void remover(ARVORE_RUBRO_NEGRA *arv, No *no)
{
    No *sucessor = no;
    No *filho;
    enum COR cor_original_sucessor = sucessor->cor;
    /* Caso alguma subárvore seja nula, basta transplatar o lado não nulo
       para o nó que deseja remover. */
    if (no->esq == arv->NIL) {
        filho = no->dir;
        transplantarSubArvore(arv, no, no->dir);
    }
    else if (no->dir == arv->NIL) {
        filho = no->esq;
        transplantarSubArvore(arv, no, no->esq);
    }
    /* Caso o nó tenha filhos em ambos os lados. */
    else {
        sucessor = obterSucessor(arv, no->dir);
        filho = sucessor->dir;
        cor_original_sucessor = sucessor->cor; // Guardar a cor para saber se precisa do balanceamento.
        // Caso o sucessor seja filho imediato, garantimos que o filho dele está apontando para o sucessor. 
        if (sucessor->pai == no)
            filho->pai = sucessor;
        else {
            transplantarSubArvore(arv, sucessor, sucessor->dir);
            sucessor->dir = no->dir;
            sucessor->dir->pai = sucessor;
        }
        transplantarSubArvore(arv, no, sucessor);   
        sucessor->esq = no->esq;
        sucessor->esq->pai = sucessor;
        sucessor->cor = no->cor;
    }
    // Se o sucessor for preto, balanceamos.
    printf("Valor %d removido!\n", no->valor);
    free(no);
    if (cor_original_sucessor == Preto){
        balancearRemocao(arv, filho);}
}

void inorder(ARVORE_RUBRO_NEGRA *arv, No *no)
{
    if (no != arv->NIL)
    {
        inorder(arv, no->esq);
        printf(arv->raiz == no ? "\nRaiz: " : "\n");
        printf("%d ", no->valor);
        printf(no->cor == Vermelho ? "Vermelho\n" : "Preto\n");
        inorder(arv, no->dir);
    }
}

int main()
{
    ARVORE_RUBRO_NEGRA *t = criaARB();

    No *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n;
    a = criarNoARB(75);  
    b = criarNoARB(45); 
    c = criarNoARB(11);
    d = criarNoARB(50);
    e = criarNoARB(90);
    f = criarNoARB(78);
    g = criarNoARB(94); 
    h = criarNoARB(77);
    i = criarNoARB(80);
    j = criarNoARB(85);
    k = criarNoARB(92);
    l = criarNoARB(96);
    m = criarNoARB(95);
    n = criarNoARB(98);

    inserir(t, a);
    inserir(t, b);
    inserir(t, c);
    inserir(t, d);
    inserir(t, e);
    inserir(t, f);
    inserir(t, g);
    inserir(t, h);
    inserir(t, i);
    inserir(t, j);
    inserir(t, k);
    inserir(t, l);
    inserir(t, m);
    inserir(t, n);

    remover(t, e);

    inorder(t, t->raiz);

    buscaARB(t, 11);

    return 0;
}