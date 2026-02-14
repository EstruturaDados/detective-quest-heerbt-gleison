#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

        printf("Atualmente em: %s", atual->nomeSala);

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

    // Inicializa exploração
    explorarSalasComPistas(hall, &raizPistas);

    // Mostrar pistas ao final
    printf("\n=== Pistas Encontradas ===\n");

    if (raizPistas == NULL) 
        printf("Nenhuma pista encontrada.\n");
    else
        imprimirPistas(raizPistas);

    // Liberar memória
    liberarPistas(raizPistas);

    free(hall->nomeSala);
    free(hall);
    free(biblioteca->nomeSala);
    free(biblioteca);
    free(cozinha->nomeSala);
    free(cozinha);

    return 0;
}
