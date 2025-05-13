#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "func_gerais.h"

struct foguetes{
    char modelo[30];
    char capacidade[10];
};

typedef struct Decolagens {
    char ID[10];
    char horario[6];
    char localizacao[40];
    struct foguetes foguete;
    struct Decolagens *proximo;
} Decolagens;

typedef struct Fila {
    Decolagens *inicio;
    Decolagens *fim;
} Fila;

//Função para verificar o modelo
char verificarModelo(const char modelo[], const char capacidade_fornecida_str[]) {
    FILE *arquivo = fopen("modelos.txt", "r");
    if (arquivo == NULL) return '2'; // Arquivo não encontrado

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove a quebra de linha (caso exista)
        linha[strcspn(linha, "\n")] = '\0';

        char modeloArquivo[50], capacidadeArquivo[50];
        // Divide a linha em modelo e capacidade
        if (sscanf(linha, "%49[^;];%49s", modeloArquivo, capacidadeArquivo) != 2) {
            continue; // Ignora linhas mal formatadas
        }

        // Compara o modelo
        if (strcasecmp(modeloArquivo, modelo) == 0) {
            fclose(arquivo);

            // Converte as capacidades para números
            char *endptr;
            long capacidade_fornecida = strtol(capacidade_fornecida_str, &endptr, 10);
            long capacidade_modelo = strtol(capacidadeArquivo, &endptr, 10);

            // Verifica erros de conversão
            if (errno != 0 || *endptr != '\0') {
                return '1'; // Capacidade inválida (não é número)
            }

            // Comparação numérica
            if (capacidade_fornecida <= capacidade_modelo && capacidade_fornecida >= 0) {
                return '0'; // Capacidade aceitável
            } else {
                return '1'; // Capacidade excedida
            }
        }
    }

    fclose(arquivo);
    return '2'; // Modelo não encontrado
}

//Função para conversão do horario
int converterHorarioParaMinutos(const char *horario) {
    int h, m;
    if (sscanf(horario, "%d:%d", &h, &m) != 2) {
        return -1;
    }
    return h * 60 + m;
}

Fila* abrirDecolagens() {
    FILE *fp = fopen("decolagens.csv", "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return NULL;
    }

    Fila *novaFila = malloc(sizeof(Fila));
    novaFila->inicio = NULL;
    novaFila->fim = NULL; // Inicializa o fim da fila
    char linha[100];

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        // Remove a quebra de linha
        linha[strcspn(linha, "\n")] = '\0';
        
        if(linha[0] == '\0')
        {
            continue;
        }

        Decolagens *novaDecolagem = malloc(sizeof(Decolagens));
        char *token = strtok(linha, ";");
        
        // Leitura dos campos (mantido igual)
        if (token) {
            strncpy(novaDecolagem->localizacao, token, sizeof(novaDecolagem->localizacao) - 1);
            novaDecolagem->localizacao[sizeof(novaDecolagem->localizacao) - 1] = '\0';
            
            token = strtok(NULL, ";");
            strncpy(novaDecolagem->horario, token, sizeof(novaDecolagem->horario) - 1);
            novaDecolagem->horario[sizeof(novaDecolagem->horario) - 1] = '\0';
            
            token = strtok(NULL, ";");
            strncpy(novaDecolagem->ID, token, sizeof(novaDecolagem->ID) - 1);
            novaDecolagem->ID[sizeof(novaDecolagem->ID) - 1] = '\0';
            
            token = strtok(NULL, ";");
            strncpy(novaDecolagem->foguete.capacidade, token, sizeof(novaDecolagem->foguete.capacidade) - 1);
            novaDecolagem->foguete.capacidade[sizeof(novaDecolagem->foguete.capacidade) - 1] = '\0';
            
            token = strtok(NULL, ";");
            strncpy(novaDecolagem->foguete.modelo, token, sizeof(novaDecolagem->foguete.modelo) - 1);
            novaDecolagem->foguete.modelo[sizeof(novaDecolagem->foguete.modelo) - 1] = '\0';
        }

        novaDecolagem->proximo = NULL;

        // CORREÇÃO: Vincular os nós corretamente
        if (novaFila->inicio == NULL) {
            novaFila->inicio = novaDecolagem;
            novaFila->fim = novaDecolagem;
        } else {
            novaFila->fim->proximo = novaDecolagem; // Liga o último nó ao novo
            novaFila->fim = novaDecolagem; // Atualiza o fim da fila
        }
    }

    fclose(fp);
    return novaFila;
}

// Função de ordenação por horario
void ordenarDecolagens(Fila *decolagem) {
    if (decolagem == NULL || decolagem->inicio == NULL || decolagem->inicio->proximo == NULL) {
        return; // Nada a ordenar
    }

    Decolagens *ordenada = NULL;
    Decolagens *atual = decolagem->inicio;

    while (atual != NULL) {
        Decolagens *proximo = atual->proximo; // Salva o próximo nó

        // Caso 1: Insere no início da lista ordenada
        if (ordenada == NULL || 
            converterHorarioParaMinutos(atual->horario) < 
            converterHorarioParaMinutos(ordenada->horario)) {
            atual->proximo = ordenada;
            ordenada = atual;
        } 
        // Caso 2: Procura a posição correta
        else {
            Decolagens *temp = ordenada;
            while (temp->proximo != NULL && 
                   converterHorarioParaMinutos(temp->proximo->horario) < 
                   converterHorarioParaMinutos(atual->horario)) {
                temp = temp->proximo;
            }
            atual->proximo = temp->proximo;
            temp->proximo = atual;
        }

        atual = proximo; // Avança para o próximo nó
    }

    decolagem->inicio = ordenada; // Atualiza o início da fila
}

void imprimirDecolagem(Fila *decolagem){
    Decolagens *impressaoDecolagens = decolagem->inicio;
    while(impressaoDecolagens != NULL)
    {
        printf("Modelo: %s - ", impressaoDecolagens->foguete.modelo);
        printf("Capacidade: %s - ", impressaoDecolagens->foguete.capacidade);
        printf("Destino: %s - ", impressaoDecolagens->localizacao);
        printf("Horario: %s - ", impressaoDecolagens->horario);
        printf("ID: %s\n", impressaoDecolagens->ID);
        impressaoDecolagens = impressaoDecolagens->proximo;
    }
    printf("\n");
}

void adicionarDecolagem(Fila *decolagemNova, const char horario[], const char modelo[], const char capacidade[], const char idAeronave[], const char localDestino[]){
       // 1. Alocar memória para a nova decolagem
    Decolagens *novaDecolagem = (Decolagens*)malloc(sizeof(Decolagens));
    if (novaDecolagem == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    // 2. Preencher os dados da nova decolagem
    strncpy(novaDecolagem->horario, horario, sizeof(novaDecolagem->horario) - 1);
    strncpy(novaDecolagem->foguete.modelo, modelo, sizeof(novaDecolagem->foguete.modelo) - 1);
    strncpy(novaDecolagem->ID, idAeronave, sizeof(novaDecolagem->ID) - 1);
    strncpy(novaDecolagem->localizacao, localDestino, sizeof(novaDecolagem->localizacao) - 1);
    strncpy(novaDecolagem->foguete.capacidade, capacidade, sizeof(novaDecolagem->foguete.capacidade) - 1);

    // Garantir terminador nulo em todas as strings
    novaDecolagem->horario[sizeof(novaDecolagem->horario) - 1] = '\0';
    novaDecolagem->foguete.modelo[sizeof(novaDecolagem->foguete.modelo) - 1] = '\0';
    novaDecolagem->ID[sizeof(novaDecolagem->ID) - 1] = '\0';
    novaDecolagem->localizacao[sizeof(novaDecolagem->localizacao) - 1] = '\0';
    novaDecolagem->foguete.capacidade[sizeof(novaDecolagem->foguete.capacidade) - 1] = '\0';

    novaDecolagem->proximo = NULL;

    // 3. Adicionar à fila
    if (decolagemNova->inicio == NULL) {
        decolagemNova->inicio = novaDecolagem;
        decolagemNova->fim = novaDecolagem;
    } else {
        decolagemNova->fim->proximo = novaDecolagem;
        decolagemNova->fim = novaDecolagem;
    }

    // 4. Escrever no arquivo CSV (modo append)
    FILE *fp = fopen("decolagens.csv", "a");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    // Adiciona \n antes do novo registro (exceto se o arquivo estiver vazio)
    if (ftell(fp) != 0) {
        fprintf(fp, "\n");
    }

    fprintf(fp, "%s;%s;%s;%s;%s\n",
        localDestino,    // Localização
        horario,         // Horário
        idAeronave,      // ID
        capacidade,      // Capacidade
        modelo           // Modelo
    );
}

void removerDecolagem(Fila *fila, const char id[]) {
    if (fila == NULL || fila->inicio == NULL || id == NULL) {
        printf("Fila vazia ou ID inválido!\n");
        return;
    }

    Decolagens *atual = fila->inicio;
    Decolagens *anterior = NULL;
    int encontrou = 0; // Flag para indicar se o ID foi encontrado

    // Passo 1: Remove da memória
    while (atual != NULL) {
        if (strcmp(atual->ID, id) == 0) {
            if (anterior == NULL) {
                fila->inicio = atual->proximo;
                if (fila->fim == atual) {
                    fila->fim = NULL;
                }
            } else {
                anterior->proximo = atual->proximo;
                if (fila->fim == atual) {
                    fila->fim = anterior;
                }
            }

            free(atual);
            encontrou = 1;
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (!encontrou) {
        printf("ID %s não encontrado!\n", id);
        return;
    }

    // Passo 2: Remove do arquivo CSV
    FILE *fp = fopen("decolagens.csv", "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo para leitura!\n");
        return;
    }

    // Cria um arquivo temporário
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(fp);
        return;
    }

    char linha[200];
    int primeiraLinha = 1;

    // Lê linha por linha e copia, exceto a linha com o ID
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        char idAtual[10];
        sscanf(linha, "%*[^;];%*[^;];%9[^;]", idAtual); // Extrai o ID da linha

        if (strcmp(idAtual, id) != 0) {
            if (primeiraLinha) {
                primeiraLinha = 0;
            } else {
                fprintf(temp, "\n");
            }
            fprintf(temp, "%s", linha);
        }
    }

    fclose(fp);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("decolagens.csv");
    rename("temp.csv", "decolagens.csv");

    printf("Decolagem com ID %s removida com sucesso.\n", id);
}

void decolagem(Fila *fila) {
    if (fila == NULL || fila->inicio == NULL) {
        printf("Fila vazia! Nenhuma decolagem pendente.\n");
        return;
    }

    // Passo 1: Remove da memória
    Decolagens *primeiro = fila->inicio;
    char idRemovido[10];
    strncpy(idRemovido, primeiro->ID, sizeof(idRemovido));
    fila->inicio = fila->inicio->proximo;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    // Passo 2: Remove do arquivo CSV
    FILE *arquivo = fopen("decolagens.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        free(primeiro);
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivo);
        free(primeiro);
        return;
    }

    char linha[200];
    int primeiraLinha = 1;

    // Copia todas as linhas, exceto a que tem o ID removido
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char idAtual[10];
        sscanf(linha, "%*[^;];%*[^;];%9[^;]", idAtual);

        if (strcmp(idAtual, idRemovido) != 0) {
            if (primeiraLinha) {
                primeiraLinha = 0;
            } else {
                fprintf(temp, "\n");
            }
            fprintf(temp, "%s", linha);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("decolagens.csv");
    rename("temp.csv", "decolagens.csv");

    free(primeiro);
    for(int i = 5; i >= 0; i--) {
        sleep(1);
        printf("%d segundo(s)\n", i);
    }
    printf("\nDecolagem realizada! Um grande passo para o futuro!\n");
}

void fecharFila(Fila *fila) {
    if (fila == NULL) return;
    //Liberar memória
    Decolagens *atual = fila->inicio;
    while (atual != NULL) {
        Decolagens *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
}