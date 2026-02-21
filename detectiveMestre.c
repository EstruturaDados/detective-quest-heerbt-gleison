#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_HASH 10

typedef struct noPista {
    char *textoPista;
    struct noPista *esq;
    struct noPista *dir;
} noPista;

typedef struct salaMansao {
    char *nomeSala;
    char *pista;
    struct salaMansao *esq;
    struct salaMansao *dir;
} salaMansao;

typedef struct noHash {
    char *pista;
    char *suspeito;
    struct noHash *prox;
} noHash;

noHash* tabelaHash[TAM_HASH];

// ============= FUNÇÕES HASH ===================

int funcaohash(char *str) {
    int soma = 0;

    for(int i = 0; str[i] != '\0'; i++)
        soma += str[i];

    return soma % TAM_HASH;
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaohash(pista);

    noHash *novo = malloc(sizeof(noHash));

    if (novo == NULL)
        return;

    novo->pista = malloc(strlen(pista)+1);
    novo->suspeito = malloc(strlen(suspeito)+1);

    if (novo->pista == NULL || novo->suspeito == NULL) {
        free(novo->pista);
        free(novo->suspeito);
        free(novo);
        return;
    }

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// ================ BST pistas ==================

noPista* criarNoPista(char *texto) {
    noPista *novo = malloc(sizeof (noPista));

    if (novo == NULL) {
        return NULL;
    }

    novo->textoPista = malloc(strlen(texto) + 1);
    if (novo->textoPista == NULL) {
        free(novo);
        return NULL;
    }

    strcpy(novo->textoPista, texto);
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

noPista* inserirPista(noPista *raiz, char *texto) {
    if (raiz == NULL) 
        return criarNoPista(texto);

    if (strcmp (texto, raiz->textoPista) < 0) {
        raiz->esq = inserirPista(raiz->esq, texto);
    }
    else if (strcmp (texto, raiz->textoPista) > 0) {
        raiz->dir = inserirPista(raiz->dir, texto);
    }

    return raiz;
}

void imprimirPistas(noPista *raiz) {
    if (raiz != NULL) {
        imprimirPistas(raiz->esq);
        printf("%s\n", raiz->textoPista);
        imprimirPistas(raiz->dir);
    }
}

void liberarPistas(noPista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esq);
        liberarPistas(raiz->dir);
        free(raiz->textoPista);
        free(raiz);
    }
}

// =============   Salas   =====================

salaMansao* criarSala(char *nome, char *pista) {
    
    salaMansao *nova = malloc(sizeof(salaMansao));
    if (nova == NULL) return NULL;

    nova->nomeSala = malloc(strlen(nome) + 1);
    if (nova->nomeSala == NULL) {
        free(nova); 
        return NULL;
    }
    strcpy(nova->nomeSala, nome);

    if (pista != NULL){
        nova->pista = malloc(strlen(pista) + 1);
        if (nova->pista == NULL) {
            free (nova->nomeSala);
            free(nova);
            return NULL;
        }
        strcpy(nova->pista, pista);
    } else {
        nova->pista = NULL;
    }

    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}

void explorarSalasComPistas(salaMansao *atual, noPista **raizPistas) {

    char opcao;

    while (1) {

        printf("Atualmente em: %s\n", atual->nomeSala);

        if (atual->pista != NULL) {
            printf("Pista: %s\n", atual->pista);
            
            *raizPistas = inserirPista(*raizPistas, atual->pista);
            
            free (atual->pista);
            
            atual->pista = NULL;
        }

        printf("Escolha uma opção: \n");
        printf("(e) esquerda\n");
        printf("(d) direita\n");
        printf("(s) sair\n");
        scanf(" %c", &opcao);

        switch (opcao) {
        case 'e':
            if (atual->esq != NULL) {
                atual = atual->esq;
            } else {
                printf("Não há sala a esquerda!\n");
            }
            break;

        case 'd':
            if (atual->dir != NULL) {
                atual = atual->dir;
            } else {
                printf("Não há sala a direita!\n");
            }
            break;

        case 's': 
            printf("Saindo...\n");
            return;
        
        default: printf("Opção invalida\n");
            break;
        }
    }
}

// ======= ENCONTRAR SUSPEITO ============

char* encontrarSuspeito(char *pista){
    int indice = funcaohash(pista);

    noHash *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        } 

        atual = atual->prox;
    }

    return NULL;
}

void exibirSuspeitosUnicos() {
    printf("\n=== Lista de Suspeitos ===\n");

    char exibidos[100][100];
    int totalExibidos = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        noHash *atual = tabelaHash[i];

        while (atual != NULL) {

            int jaExibido = 0;

            for (int j = 0; j < totalExibidos; j++) {
                if (strcmp(exibidos[j], atual->suspeito) == 0) {
                    jaExibido = 1;
                    break;
                }
            }

            if (!jaExibido) {
                strcpy(exibidos[totalExibidos], atual->suspeito);
                printf("- %s\n", atual->suspeito);
                totalExibidos++;
            }

            atual = atual->prox;
        }
    }
}

int contarPistasSuspeito(noPista *raiz, char *suspeito){
    if (raiz == NULL)
        return 0;

    int contador = 0;

    char *suspeitoEncontrado = encontrarSuspeito(raiz->textoPista);

    if (suspeitoEncontrado != NULL &&
       strcmp(suspeitoEncontrado, suspeito) == 0)
       contador++;

    contador += contarPistasSuspeito(raiz->esq, suspeito);
    contador += contarPistasSuspeito(raiz->dir, suspeito);

    return contador; 
}

void verificarSuspeitoFinal(noPista *raizPistas) {
    if (raizPistas == NULL) {
        printf("\nNunhuma pista foi coletada. não há como acusar alguém.\n");
        return;
    }

    char suspeito[100];

    printf("\nDigite o nome do suspeito: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(suspeito, sizeof(suspeito), stdin);
    suspeito[strcspn(suspeito, "\n")] = '\0';

    int total = contarPistasSuspeito(raizPistas, suspeito);

    printf("\nTotal de pistas contra %s: %d\n", suspeito, total);

    if (total >= 2) {
        printf("\nCaso resolvido! %s é o culpado!\n", suspeito);
    } else {
        printf("\nEvidências insuficientes. %s não pode ser condenado.\n", suspeito);
    }
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++){
        noHash *atual = tabelaHash[i];

        while (atual != NULL){
            noHash *temp = atual;

            atual = atual->prox;

            free(temp->pista);
            free(temp->suspeito);
            free(temp);
        }
    }
}
// ============ MAIN ====================

int main() {

    noPista *raizPistas = NULL;

    // Criando salas
    salaMansao *hall = criarSala("Hall", "Pegadas no chão");
    salaMansao *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    salaMansao *cozinha = criarSala("Cozinha", "Faca ensanguentada");

    // Conectando salas
    hall->esq = biblioteca;
    hall->dir = cozinha;

    // Associar pistas->suspeitos
    inicializarHash();

    inserirNaHash("Pegadas no chão", "mordomo");
    inserirNaHash("Livro fora do lugar", "Professor");
    inserirNaHash("Faca ensanguentada", "Cozinheiro");

    // Inicializa exploração
    explorarSalasComPistas(hall, &raizPistas);

    // Mostrar pistas ao final
    printf("\n=== Pistas Encontradas ===\n");

    if (raizPistas == NULL) 
        printf("Nenhuma pista encontrada.\n");
    else
        imprimirPistas(raizPistas);
        
    printf("\nSuspeitos disponíveis:\n");
    exibirSuspeitosUnicos();
    
    verificarSuspeitoFinal(raizPistas);

    // Liberar memória
    liberarPistas(raizPistas);

    free(hall->nomeSala);
    free(hall);
    free(biblioteca->nomeSala);
    free(biblioteca);
    free(cozinha->nomeSala);
    free(cozinha);

    liberarHash();

    return 0;
}
