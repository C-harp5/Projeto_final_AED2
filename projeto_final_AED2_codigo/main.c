#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "destinos.c" 
#include "decolagens.c"
#include "historico.c"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_str(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}

char menu() {
    char opcao;
    
    printf("\n=== Menu de Controle ===");
    printf("\n1 - Cadastro de foguetes para decolagem");
    printf("\n2 - Fila de foguetes para decolagem");
    printf("\n3 - Remover foguete da fila de decolagens");
    printf("\n4 - Lista de destinos");
    printf("\n5 - Remover destino");
    printf("\n6 - Decolar foguete");
    printf("\n7 - Historico de decolagens");
    printf("\n8 - Adicionar destinos a lista");
    printf("\n9 - Sair");
    printf("\nEscolha: ");
    
    scanf(" %c", &opcao);
    limparBuffer();
    
    return opcao;
}

int main() {

    //Temporarios para a criação da fila de decolagens
    char tempDestino[40];
    char tempHorario[10];
    char tempID[10];
    char tempCapacidade[10];
    char tempModelo[50];
    //================================================

    char opcao;
    char destino[40];
    bool exit = false;
    Lista *listaDestinos = abrirDestinos();
    Fila *filaDecolagem = abrirDecolagens();
    Pilha *pilhaHistorico = abrirHistorico();

    // Verifica se a lista foi criada corretamente
    if(listaDestinos == NULL)
    {
        printf("Erro ao inicializar lista de destinos!\n");
        return 1;
    }

    if(filaDecolagem == NULL)
    {
        printf("Erro ao inicializar a fila de decolagens!\n");
        return 1;
    }

    if(pilhaHistorico == NULL)
    {
        printf("Erro ao inicializar histórico!\n");
        return 1;
    }


    ordenarDecolagens(filaDecolagem);

    do{
        opcao = menu();
        
        switch(opcao) {
            case '1':
                printf("\nCadastro de foguetes selecionado\n");
                bool cadastrofoguete = false;

                do
                {
                    printf("--- Modelo: ");
                    fgets(tempModelo, sizeof(tempModelo), stdin);
                    limpar_str(tempModelo);

                    printf("\n--- Quantidade de pessoas: ");
                    fgets(tempCapacidade, sizeof(tempCapacidade), stdin);
                    limpar_str(tempCapacidade);

                    switch(verificarModelo(tempModelo, tempCapacidade))
                    {
                        case '1':
                            printf("\nCapacidade excedida.");
                            printf("\nInsira novamente\n");
                            system("pause");
                            continue;

                        case '2':
                            printf("\nModelo não encontrado");
                            printf("\nInsira novamente\n");
                            system("pause");
                            continue;

                        case '0':
                            cadastrofoguete = true;
                            break;
                    }
                } while (!cadastrofoguete);
                
                

                printf("\n--- Horario formatação [00:00]: ");
                fgets(tempHorario, sizeof(tempHorario), stdin); 
                limpar_str(tempHorario);

                printf("\n--- Id: ");
                fgets(tempID, sizeof(tempID), stdin);
                limpar_str(tempID);

                do
                {
                    printf("\n--- Destino: ");
                    fgets(tempDestino, sizeof(tempDestino), stdin);
                    limpar_str(tempDestino);

                    if(verificarDestino(listaDestinos, tempDestino) == 0){
                        printf("Insira novamente\n");
                        system("pause");
                        continue;
                    }
                    break;
                } while (1);

                adicionarDecolagem(filaDecolagem, tempHorario, tempModelo, tempCapacidade, tempID, tempDestino);
                printf("\nDecolagem adicionada!");
                ordenarDecolagens(filaDecolagem);
                break;

            case '2':
                printf("\nLista de foguetes selecionado\n");
                imprimirDecolagem(filaDecolagem);
                break;

            case '3':
                printf("\nRemover foguete selecionado\n");

                printf("Insira o ID para a remoção: ");
                fgets(tempID, sizeof(tempID), stdin);
                limpar_str(tempID);

                removerDecolagem(filaDecolagem, tempID);
                break;

            case '4':
                printf("\n=== Destinos Disponíveis ===\n");
                imprimirDestinos(listaDestinos);
                break;

            case '5': 
                printf("\nDigite o destino a remover: ");
                fgets(destino, sizeof(destino), stdin);
                limpar_str(destino);

                removerDestino(listaDestinos, destino);
                break;

            case '6':
                printf("\nDecolagem selecionada\n");
                if(filaDecolagem != NULL){
                    adicionarHistorico(pilhaHistorico,filaDecolagem->inicio->ID, filaDecolagem->inicio->horario, filaDecolagem->inicio->localizacao, filaDecolagem->inicio->foguete.modelo, filaDecolagem->inicio->foguete.capacidade);
                    decolagem(filaDecolagem);
                } else
                {
                    printf("\nFila de decolagens vázia!\n");
                }
                break;

            case '7':
                printf("\nHistórico de decolagens\n");
                imprimirHistorico(pilhaHistorico);
                break;

            case '8': 
                char novoDestino[40];
                printf("\nNovo destino: ");
                fgets(novoDestino, sizeof(novoDestino), stdin);
                limpar_str(novoDestino);
                adicionarDestino(listaDestinos, novoDestino);
                break;

            case '9':
                printf("\nEncerrando sistema...\n");
                fecharLista(listaDestinos);
                fecharFila(filaDecolagem);
                fecharHistorico(pilhaHistorico);
                exit = true;
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                sleep(1);
                break;
        }
    }while(!exit);

    return 0;
}