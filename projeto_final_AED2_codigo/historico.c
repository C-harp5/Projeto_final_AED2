#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_gerais.h"

// Struct corrigida com ordem de campos igual ao CSV e tipo de ponteiro consistente
typedef struct Foguete {
    char horario[6];        // Campo 1 do CSV
    char ID[10];            // Campo 2 do CSV
    char localizacao[40];   // Campo 3 do CSV
    char modelo[20];        // Campo 4 do CSV
    char quantidade[6];     // Campo 5 do CSV
    struct Foguete* anterior;
} Foguete;

typedef struct Pilha {
    Foguete* topo;
} Pilha;

int historicoVazio(Pilha *h){
    return (h == NULL || h->topo == NULL);
}

Pilha* abrirHistorico() {
    FILE *fp = fopen("historico.csv", "r");
    if(fp == NULL) {
        printf("Erro ao abrir arquivo!\n"); 
        return NULL;
    }

    Pilha *novaPilha = malloc(sizeof(Pilha));
    if(novaPilha == NULL) {
        fclose(fp);
        return NULL;
    }
    novaPilha->topo = NULL;
    char linha[100];

    while(fgets(linha, sizeof(linha), fp) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        if(linha[0] == '\0')
        {
            continue;
        }
        
        Foguete *novoFoguete = malloc(sizeof(Foguete));
        if(!novoFoguete) {
            fclose(fp);
            free(novaPilha);
            return NULL;
        }

        // Extração de tokens na ordem correta do CSV
        char *token = strtok(linha, ";");
        if(!token) { 
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->horario, token, sizeof(novoFoguete->horario) - 1);
        novoFoguete->horario[sizeof(novoFoguete->horario) - 1] = '\0';
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->ID, token, sizeof(novoFoguete->ID) - 1);
        novoFoguete->ID[sizeof(novoFoguete->ID) - 1] = '\0';
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->localizacao, token, sizeof(novoFoguete->localizacao) - 1);
        novoFoguete->localizacao[sizeof(novoFoguete->localizacao) - 1] = '\0';
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->modelo, token, sizeof(novoFoguete->modelo) - 1);
        novoFoguete->modelo[sizeof(novoFoguete->modelo) - 1] = '\0';
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->quantidade, token, sizeof(novoFoguete->quantidade) - 1);
        novoFoguete->quantidade[sizeof(novoFoguete->quantidade) - 1] = '\0';

        // Empilhamento correto
        novoFoguete->anterior = novaPilha->topo;
        novaPilha->topo = novoFoguete;
    }
    fclose(fp);
    return novaPilha;
}

void imprimirHistorico(Pilha *h) {
    Foguete *atual = h->topo;
    while(atual != NULL) {
        printf("Horario: %s ", atual->horario);
        printf("ID: %s ", atual->ID);
        printf("Destino: %s ", atual->localizacao);
        printf("Modelo: %s ", atual->modelo);
        printf("Passageiros: %s\n", atual->quantidade);
        atual = atual->anterior;
    }
}

// Função simplificada sem retorno
void removerHistorico(Pilha *h) {
    if(h->topo == NULL) return;
    Foguete* removido = h->topo;
    h->topo = removido->anterior;
    free(removido);
}

// Adiciona um novo histórico e atualiza o CSV com o mais recente em primeiro lugar
void adicionarHistorico(Pilha *h, char id[], char horario[], char localizacao[], char modelo[], char quantidade[]) {
    if (!h) return;
    Foguete *novo = malloc(sizeof(Foguete));
    if (!novo) return;

    snprintf(novo->horario, 6, "%s", horario);
    snprintf(novo->ID, 10, "%s", id);
    snprintf(novo->localizacao, 40, "%s", localizacao);
    snprintf(novo->modelo, 20, "%s", modelo);
    snprintf(novo->quantidade, 6, "%s", quantidade);

    novo->anterior = h->topo;
    h->topo = novo;

    // Reescreve todo o CSV colocando o mais recente primeiro   
    salvarHistorico(h);
}

// Salva a pilha inteira no CSV do mais recente (linha 1) ao mais antigo
void salvarHistorico(Pilha *h) {
    if (!h) return;
    FILE *fp = fopen("historico.csv", "w");
    if (!fp) {
        printf("Erro ao salvar Historico!\n");
        return;
    }

    // Cria uma lista temporária para inverter a ordem
    Foguete *elementos[100];
    int count = 0;
    Foguete *atual = h->topo;

    // Coleta elementos do topo (mais recente) para a base (mais antigo)
    while (atual && count < 100) {
        elementos[count++] = atual;
        atual = atual->anterior;
    }

    // Grava do último para o primeiro (mais antigo → mais recente)
    for (int i = count - 1; i >= 0; i--) {
        fprintf(fp, "%s;%s;%s;%s;%s\n", 
            elementos[i]->horario,
            elementos[i]->ID,
            elementos[i]->localizacao,
            elementos[i]->modelo,
            elementos[i]->quantidade
        );
    }

    fclose(fp);
}

// Libera toda a pilha e fecha o histórico
void fecharHistorico(Pilha *h) {
    if (!h) return;
    salvarHistorico(h);
    Foguete *atual = h->topo;
    while (atual) {
        Foguete *next = atual->anterior;
        free(atual);
        atual = next;
    }
    free(h);
}