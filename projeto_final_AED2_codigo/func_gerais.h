#ifndef FUNC_GERAIS_H
#define FUNC_GERAIS_H


typedef struct Destinos Destinos;
typedef struct Historico Historico;
typedef struct Decolagens Decolagens;
typedef struct Lista Lista; 
typedef struct Fila Fila;

// Parte dos destinos
Lista* abrirDestinos(); //Abre o arquivo com a lista de destino
void imprimirDestinos(Lista *d); //Imprime o arquivo com a lista de destino
void removerDestino(Lista *d, const char destinoExistente[]); // Remove o destino de dentro do arquivo
void adicionarDestino(Lista *d, const char destinoNovo[]); // Adiciona o destino novo dentro do arquivo
void fecharLista(Lista *d); //Uma função para salvar o arquivo novo e dar um fclose()
void salvarDestinos(Lista *d);
int verificarDestino(Lista *d, const char destinoExistente[]);
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
Historico* abrirHistorico(); //Abre o arquivo de histórico
void imprimirHistorico(Historico *historico); //Imprime o arquivo de histórico
void removerHistorico(Historico *historico);//Remove o histórico
void adicionarHistorico(Historico *historico);//Põe no historico
void fecharHistorico(); //fecha o arquivo
//------------------------------------------------------------------

#endif