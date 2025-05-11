#include <stdio.h>
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
   
}

void imprimirHistorico(Pilha *h){

}
 
void removerHistorico(Pilha *h){

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