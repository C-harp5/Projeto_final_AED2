#include <stdio.h>
#include <string.h>
#include "func_gerais.h"

typedef struct Foguete{
    char quantidade[6];
    char ID[10];
    char modelo[20];
    char horario[6];
    char localizacao[40];
    struct Elemento* anterior;
}Foguete;

typedef struct Pilha{
    Foguete* topo;
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

    while(fgets(linha, sizeof(linha), fp) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';
        Foguete *novoFoguete = malloc(sizeof(Foguete));
        
        char *token = strtok(linha, ";");

        if (token)
        {
            strncpy(novoFoguete->horario, token, sizeof(novoFoguete->horario) - 1);
            novoFoguete->horario[sizeof(novoFoguete->horario) - 1] = '\0';
            
            token = strtok(NULL, ";");
            strncpy(novoFoguete->ID, token, sizeof(novoFoguete->ID) - 1);
            novoFoguete->ID[sizeof(novoFoguete->ID) - 1] = '\0';

            token = strtok(NULL, ";");
            strncpy(novoFoguete->localizacao, token, sizeof(novoFoguete->localizacao) - 1);
            novoFoguete->localizacao[sizeof(novoFoguete->localizacao) - 1] = '\0';

            token = strtok(NULL, ";");
            strncpy(novoFoguete->modelo, token, sizeof(novoFoguete->modelo) - 1);
            novoFoguete->modelo[sizeof(novoFoguete->modelo) - 1] = '\0';
        }
        novoFoguete->anterior = novaPilha->topo; //Empilhamento correto da pilha
        novaPilha->topo = novoFoguete;
    }
    fclose(fp);
    return novaPilha;
}

void imprimirHistorico(Pilha *h){
    Foguete *atual = h->topo;

    while(atual != NULL) {
        printf("Modelo: %s ", atual->modelo);
        printf("ID: %s ", atual->ID);
        printf("Destino: %s ", atual->localizacao);
        printf("Horario: %s ", atual->horario);
        printf("Quantidade de passageiros: %s", atual->quantidade);
        atual = atual->anterior;
    }
}
//em ajuste
void removerHistorico(Pilha *h){
    if(h->topo == NULL) return -1;

    Foguete* removido = h->topo;
    int valor = removido->horario;
    h->topo = removido->anterior;
    free(removido);
    return valor;
}

void adicionarHistorico(Pilha *h, char id[], char horario[], char localizacao[], char modelo[], char quantidade[]){
    Foguete* novo = (Foguete*)malloc(sizeof(Foguete));

    strcpy(novo->ID,id);
    strcpy(novo->horario,horario);
    strcpy(novo->localizacao, localizacao);
    strcpy(novo->modelo, modelo);
    strcpy(novo->quantidade, quantidade);
 
    novo->anterior = h->topo;
    h->topo = novo;
}

void fecharHistorico(Pilha *h){
    salvarHistorico(h);  // Salva alterações antes de liberar memória
    
    Foguete *atual = h->topo;
    while(atual != NULL) {
        Foguete *temp = atual;
        atual = atual->anterior;
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

    Foguete *atual = p->topo;
    while(atual != NULL) {
        fprintf(fp, "%s; ", atual->modelo);
        fprintf(fp, "%s; ", atual->ID);
        fprintf(fp, "%s; ", atual->localizacao);
        fprintf(fp, "%s; ", atual->horario);
        atual = atual->anterior;
    }
    
    fclose(fp);
}