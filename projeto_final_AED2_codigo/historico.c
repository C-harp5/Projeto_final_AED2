#include <stdio.h>
#include <string.h>
#include "func_gerais.h"

typedef struct foguete{
    char ID[10];
    char modelo[20];
    char horario[6];
    char localizacao[40];
    struct Elemento* proximo;
}foguete;

typedef struct Pilha{
    foguete* topo;
}Pilha;

Pilha* abrirHistorico(){
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
        limpar_str(linha);
        
        foguete *novoFoguete = malloc(sizeof(foguete));
        if(novoFoguete == NULL) {
            fclose(fp);
            free(novaPilha);
            return NULL;
        }

        char *token = strtok(linha, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->ID, token, sizeof(novoFoguete->ID) - 1);
        limpar_str(novoFoguete->ID);

        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->horario, token, sizeof(novoFoguete->horario) - 1);
        limpar_str(novoFoguete->horario);
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->localizacao, token, sizeof(novoFoguete->localizacao) - 1);
        limpar_str(novoFoguete->localizacao);
        
        token = strtok(NULL, ";");
        if(!token) {
            free(novoFoguete);
            continue;
        }
        strncpy(novoFoguete->modelo, token, sizeof(novoFoguete->modelo) - 1);
        limpar_str(novoFoguete->modelo);
        
        novoFoguete->proximo = novaPilha->topo;
        novaPilha->topo = novoFoguete;
    }

    fclose(fp);
    return novaPilha;
}

void imprimirHistorico(Pilha *h){
    foguete *atual = h->topo;

    while(atual != NULL) {
        printf("Modelo: %s ", atual->modelo);
        printf("ID: %s ", atual->ID);
        printf("Destino: %s ", atual->localizacao);
        printf("Horario: %s ", atual->horario);
        atual = atual->proximo;
    }
}
//em ajuste
void removerHistorico(Pilha *h){
    if(h->topo == NULL) return -1;

    foguete* removido = h->topo;
    int valor = removido->horario;
    h->topo = removido->proximo;
    free(removido);
    return valor;
}

void adicionarHistorico(Pilha *h, char id[], char horario[], char localizacao[], char modelo[]){
    foguete* novo = (foguete*)malloc(sizeof(foguete));

    strcpy(novo->ID,id);
    strcpy(novo->horario,horario);
    strcpy(novo->localizacao, localizacao);
    strcpy(novo->modelo, modelo);
 
    novo->proximo = h->topo;
    h->topo = novo;
}

void fecharHistorico(Pilha *h){
    salvarHistorico(h);  // Salva alterações antes de liberar memória
    
    foguete *atual = h->topo;
    while(atual != NULL) {
        foguete *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(h);
}

void salvarHistorico(Pilha *p){
    FILE *fp = fopen("historico.csv", "a");
    if(fp == NULL) {
        printf("Erro ao salvar Historico!\n");
        return;
    }

    foguete *atual = p->topo;
    while(atual != NULL) {
        fprintf(fp, "%s ", atual->modelo);
        fprintf(fp, "%s ", atual->ID);
        fprintf(fp, "%s ", atual->localizacao);
        fprintf(fp, "%s ", atual->horario);
        atual = atual->proximo;
    }
    
    fclose(fp);
}