#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_gerais.h"

typedef struct Destinos {
    char localizacao[40];
    struct Destinos *proximo;
} Destinos;

typedef struct Lista {
    Destinos *inicio;
} Lista;

Lista* abrirDestinos() { 
    FILE *fp = fopen("destinos.csv", "r");
    if(fp == NULL) {
        printf("Erro ao abrir arquivo!\n"); 
        return NULL;
    }

    Lista *novaLista = malloc(sizeof(Lista));
    novaLista->inicio = NULL;
    Destinos *ultimo = NULL;
    char linha[100];

    while(fgets(linha, sizeof(linha), fp) != NULL) {
        // Remove a quebra de linha
        linha[strcspn(linha, "\n")] = '\0';
        if(linha[0] == '\0')
        {
            continue;
        }
        
        Destinos *novoDestino = malloc(sizeof(Destinos));
        char *token = strtok(linha, ";");
        
        if(token) {
            strncpy(novoDestino->localizacao, token, sizeof(novoDestino->localizacao));
            novoDestino->localizacao[sizeof(novoDestino->localizacao) - 1] = '\0';
        }
        
        novoDestino->proximo = NULL;

        if(novaLista->inicio == NULL) {
            novaLista->inicio = novoDestino;
            ultimo = novoDestino;
        } else {
            ultimo->proximo = novoDestino;
            ultimo = novoDestino;
        }
    }

    fclose(fp);
    return novaLista;
}

void salvarDestinos(Lista *d) {
    FILE *fp = fopen("destinos.csv", "w");
    if(fp == NULL) {
        printf("Erro ao salvar destinos!\n");
        return;
    }

    Destinos *atual = d->inicio;
    while(atual != NULL) {
        fprintf(fp, "%s;\n", atual->localizacao);
        atual = atual->proximo;
    }
    
    fclose(fp);
}

void imprimirDestinos(Lista *d) {
    Destinos *atual = d->inicio;
    while(atual != NULL) {
        printf("%s\n", atual->localizacao); 
        atual = atual->proximo;
    }
}

void removerDestino(Lista *d, const char destinoExistente[]) {
    Destinos *atual = d->inicio;
    Destinos *anterior = NULL;

    while(atual != NULL) {
        if(strcmp(atual->localizacao, destinoExistente) == 0) {
            if(anterior == NULL) {
                d->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    salvarDestinos(d);
}

void adicionarDestino(Lista *d, const char destinoNovo[]) {
    Destinos *novo = malloc(sizeof(Destinos));
    strncpy(novo->localizacao, destinoNovo, sizeof(novo->localizacao));
    novo->localizacao[sizeof(novo->localizacao) - 1] = '\0'; // Garante terminação nula
    novo->proximo = d->inicio;
    d->inicio = novo;

    salvarDestinos(d);
}

void fecharLista(Lista *d) {
    salvarDestinos(d);  // Salva alterações antes de liberar memória
    
    Destinos *atual = d->inicio;
    while(atual != NULL) {
        Destinos *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(d);
}

int verificarDestino(Lista *d, const char destinoExistente[]) {
    Destinos *atual = d->inicio;
    while(atual != NULL) {
        if(strcasecmp(atual->localizacao, destinoExistente) == 0) {
            return 1; // Destino encontrado
        }
        atual = atual->proximo;
    }
    return 0; // Destino não existe
}