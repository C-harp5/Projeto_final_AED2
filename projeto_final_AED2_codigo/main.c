#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "destinos.c" 
#include "decolagens.c"
//#include "historico.c"

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
    printf("\n1 - Cadastro de foguetes");
    printf("\n2 - Lista de foguetes");
    printf("\n3 - Remover foguete");
    printf("\n4 - Lista de destinos");
    printf("\n5 - Remover destino");
    printf("\n6 - Decolagem");
    printf("\n7 - Historico de decolagens");
    printf("\n8 - Adicionar destinos");
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

    // Verifica se a lista foi criada corretamente
    if(listaDestinos == NULL) {
        printf("Erro ao inicializar lista de destinos!\n");
        return 1;
    }

    if(filaDecolagem == NULL)
    {
        printf("Erro ao inicializar a fila de decolagens!\n");
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

                printf("\n--- Destino: ");
                fgets(tempDestino, sizeof(tempDestino), stdin);
                limpar_str(tempDestino);

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
                decolagem(filaDecolagem);
                break;

            case '7':
                printf("\nHistórico de decolagens\n");
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