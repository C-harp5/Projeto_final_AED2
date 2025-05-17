#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "destinos.c" 
#include "decolagens.c"
#include "historico.c"


//Definições para facilitar a impressão
#define LINHA "================================================"
#define SUBLINHA "--------------------------------"

void limparBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_str(char str[]){
    str[strcspn(str, "\n")] = '\0';
}

void cabecalho(const char* titulo){
    system("cls");
    printf("\n%s", LINHA);
    printf("\n%-40s", titulo);
    printf("\n%s\n", LINHA);
}

char menu(){
    char opcao;
    
    printf("\n%s", LINHA);
    printf("\n          CONTROLE DE DECOLAGENS ESPACIAIS");
    printf("\n%s", LINHA);
    
    printf("\n 1. Cadastrar foguete para decolagem");
    printf("\n 2. Exibir fila de decolagens");
    printf("\n 3. Remover foguete da fila");
    printf("\n 4. Listar destinos disponiveis");
    printf("\n 5. Remover destino");
    printf("\n 6. Realizar decolagem");
    printf("\n 7. Historico de decolagens");
    printf("\n 8. Adicionar novo destino");
    printf("\n 9. Limpar historico");
    printf("\n 0. Sair do sistema");
    printf("\n%s", LINHA);
    printf("\n Escolha: ");
    
    scanf(" %c", &opcao);
    limparBuffer();
    
    return opcao;
}

char retornarMenu(){
    char resultado;
    
    printf("\n%s", SUBLINHA);
    printf("\n [1] Retornar ao menu principal");
    printf("\n [0] Encerrar programa");
    printf("\n%s", SUBLINHA);
    printf("\n Opcao: ");
    
    do{
        scanf(" %c", &resultado);
        limparBuffer();
        
        if(resultado == '0' || resultado == '1'){
            return resultado;
        }
        printf("\nOpcao invalida! Digite 1 ou 0: ");
    }while(1);
}

int main() {
    // Variaveis temporarias para entrada de dados
    char tempDestino[40], tempHorario[10], tempID[10];
    char tempCapacidade[10], tempModelo[50];
    
    // Estruturas de dados principais
    Lista *listaDestinos = abrirDestinos();
    Fila *filaDecolagem = abrirDecolagens();
    Pilha *pilhaHistorico = abrirHistorico();

    // Verificacao inicial das estruturas
    if(!listaDestinos || !filaDecolagem || !pilhaHistorico){
        printf("\nERRO: Falha na inicializacao do sistema!");
        return 1;
    }

    ordenarDecolagens(filaDecolagem);
    bool sair = false;
    
    do{

        system("cls");
        char opcao = menu();
        
        switch(opcao){
            case '1': // Cadastro de novo foguete
            {
                system("cls");
                cabecalho("CADASTRO DE FOGUETE");
                bool dadosValidos = false;
                
                // Validacao do modelo e capacidade
                do{
                    printf("\nModelo do foguete: ");
                    fgets(tempModelo, sizeof(tempModelo), stdin);
                    limpar_str(tempModelo);

                    printf("Capacidade (passageiros): ");
                    fgets(tempCapacidade, sizeof(tempCapacidade), stdin);
                    limpar_str(tempCapacidade);

                    switch(verificarModelo(tempModelo, tempCapacidade)){
                        case '1':
                            printf("\nERRO: Capacidade invalida!");
                            break;
                        case '2':
                            printf("\nERRO: Modelo nao registrado!");
                            break;
                        case '0':
                            dadosValidos = true;
                            break;
                    }
                }while(!dadosValidos);

                // Coleta dos demais dados
                printf("\nHorario (formato HH:MM): ");
                fgets(tempHorario, sizeof(tempHorario), stdin);
                limpar_str(tempHorario);

                printf("ID unico do voo: ");
                fgets(tempID, sizeof(tempID), stdin);
                limpar_str(tempID);

                // Validacao do destino
                do{
                    printf("\nDestino: ");
                    fgets(tempDestino, sizeof(tempDestino), stdin);
                    limpar_str(tempDestino);
                }while(verificarDestino(listaDestinos, tempDestino) == 0);

                adicionarDecolagem(filaDecolagem, tempHorario, tempModelo, 
                                 tempCapacidade, tempID, tempDestino);
                printf("\nSUCESSO: Foguete cadastrado!");
                ordenarDecolagens(filaDecolagem);
                break;
            }

            case '2': // Listagem da fila
                system("cls");
                cabecalho("FILA DE DECOLAGENS");
                if(!filaVazia(filaDecolagem)){
                    imprimirDecolagem(filaDecolagem);
                }else{
                    printf("\nAVISO: Fila de decolagens vazia");
                }
                break;

            case '3': // Remocao por ID
                system("cls");
                cabecalho("REMOCAO DE FOGUETE");
                if(!filaVazia(filaDecolagem)){
                    printf("\nInforme o ID para remocao: ");
                    fgets(tempID, sizeof(tempID), stdin);
                    limpar_str(tempID);
                    removerDecolagem(filaDecolagem, tempID);
                }else{
                    printf("\nAVISO: Fila de decolagens vazia");
                }
                break;

            case '4': // Lista de destinos
                system("cls");
                cabecalho("DESTINOS DISPONIVEIS");
                if(!listaVazia(listaDestinos)){
                    imprimirDestinos(listaDestinos);
                }else{
                    printf("\nAVISO: Nenhum destino cadastrado");
                }
                break;

            case '5': // Remocao de destino
                system("cls");
                cabecalho("REMOCAO DE DESTINO");
                if(!listaVazia(listaDestinos)){
                    printf("\nDestino a remover: ");
                    fgets(tempDestino, sizeof(tempDestino), stdin);
                    limpar_str(tempDestino);
                    removerDestino(listaDestinos, tempDestino);
                }else{
                    printf("\nAVISO: Lista de destinos vazia");
                }
                break;

            case '6': // Executar decolagem
                system("cls");
                cabecalho("EXECUTAR DECOLAGEM");
                if(!filaVazia(filaDecolagem)){
                    adicionarHistorico(pilhaHistorico, filaDecolagem->inicio->ID, 
                                     filaDecolagem->inicio->horario, 
                                     filaDecolagem->inicio->localizacao, 
                                     filaDecolagem->inicio->foguete.modelo, 
                                     filaDecolagem->inicio->foguete.capacidade);
                    decolagem(filaDecolagem);
                }else{
                    printf("\nAVISO: Nenhum foguete na fila");
                }
                break;

            case '7': // Consulta historico
                system("cls");
                cabecalho("HISTORICO DE DECOLAGENS");
                if(!historicoVazio(pilhaHistorico)){
                    imprimirHistorico(pilhaHistorico);
                }
                else{
                    printf("\nAVISO: Historico vazio");
                }
                break;

            case '8': // Adicionar destino
                system("cls");
                cabecalho("NOVO DESTINO");
                printf("\nNovo destino: ");
                fgets(tempDestino, sizeof(tempDestino), stdin);
                limpar_str(tempDestino);
                adicionarDestino(listaDestinos, tempDestino);
                printf("\nSUCESSO: Destino adicionado!");
                break;

            case '9': // Limpar historico
                system("cls");
                cabecalho("LIMPEZA DE HISTORICO");
                if(!historicoVazio(pilhaHistorico)){
                    removerHistorico(pilhaHistorico);
                    printf("\nSUCESSO: Historico apagado!");
                }else{
                    printf("\nAVISO: Historico vazio");
                }
                break;

            case '0': // Encerrar sistema
                cabecalho("ENCERRAMENTO DO SISTEMA");
                printf("\nSalvando dados...");
                sleep(2);
                sair = true;
                break;

            default:
                printf("\nERRO: Opcao invalida!");
                sleep(1);
                break;
        }

        // Gerenciamento de retorno ao menu
        if(!sair && opcao != '0'){
            printf("\n%s", LINHA);
            if(retornarMenu() == '0') sair = true;
        }

    } while(!sair);

    // Liberacao de recursos
    fecharLista(listaDestinos);
    fecharFila(filaDecolagem);
    fecharHistorico(pilhaHistorico);
    
    printf("\nSistema encerrado com sucesso!\n");
    return 0;
}