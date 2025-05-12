#ifndef FUNC_GERAIS_H
#define FUNC_GERAIS_H

typedef struct Lista Lista; 
typedef struct Fila Fila;
typedef struct Pilha Pilha;

// Parte dos destinos
Lista* abrirDestinos(); //Abre o arquivo com a lista de destino
char verificarModelo(const char modelo[], const char capacidade_fornecida_str[]);//Verifica o modelo utilizado
int verificarDestino(Lista *d, const char destinoExistente[]);//checa se o destino existe no arquivo destino.txt
void imprimirDestinos(Lista *d); //Imprime o arquivo com a lista de destino
void removerDestino(Lista *d, const char destinoExistente[]); // Remove o destino de dentro do arquivo
void adicionarDestino(Lista *d, const char destinoNovo[]); // Adiciona o destino novo dentro do arquivo
void fecharLista(Lista *d); //Uma função para salvar o arquivo novo e dar um fclose()
void salvarDestinos(Lista *d);
//------------------------------------------------------------------
// Parte de Decolagens
Fila* abrirDecolagens(); //Abre o arquivo com a fila de decolagem;
void ordenarDecolagens(Fila *decolagem); //Algoritmo de ordenação para dar preferencia por horario
void imprimirDecolagem(Fila *decolagem); //IMprime o arquivo de fila de decolagem
//Adiciona um novo voo com todos os parmetros
void adicionarDecolagem(Fila *decolagemNova, const char horario[], const char modelo[], const char quantidadePessoas[], const char idAeronave[], const char localDestino[]);
void removerDecolagem(Fila *decolagemExistente, const char id[]);//Remove um voo a partir do ID
void fecharFila(Fila *d);
//------------------------------------------------------------------
//Parte do histórico
Pilha* abrirHistorico(); //Abre o arquivo de histórico
void imprimirHistorico(Pilha *h); //Imprime o arquivo de histórico
void removerHistorico(Pilha *h);//Remove o histórico
void adicionarHistorico(Pilha *h, char id[], char horario[], char localizacao[], char modelo[]);//Põe no historico
void fecharHistorico(Pilha *h); //fecha o arquivo
void salvarHistorico(Pilha *p);
//------------------------------------------------------------------

#endif