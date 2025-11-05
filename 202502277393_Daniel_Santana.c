#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <time.h>   

#define MAX_COMP 20   // Número máximo de componentes possíveis
#define MAX_NOME 30   // Tamanho máximo do campo 'nome' na struct
#define MAX_TIPO 20   // Tamanho máximo do campo 'tipo' na struct

/* Declaração da struct do item conforme solicitado */
typedef struct {
    char nome[MAX_NOME];      // Campo que armazena o nome do componente
    char tipo[MAX_TIPO];      // Campo que armazena o tipo do componente
    int prioridade;           // Número que define a prioridade do componente (1 a 10)
} Componente;

/* Protótipos das funções obrigatórias/auxiliares */
void bubbleSortNome(Componente arr[], int n, long *comparisons);            // Ordena array por nome (Bubble Sort)
void insertionSortTipo(Componente arr[], int n, long *comparisons);         // Ordena array por tipo (Insertion Sort)
void selectionSortPrioridade(Componente arr[], int n, long *comparisons);   // Ordena array por prioridade (Selection Sort)
int buscaBinariaPorNome(Componente arr[], int n, const char key[], long *comparisons); // Busca binária por nome
void mostrarComponentes(Componente arr[], int n);                           // Exibe a lista de componentes
void medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparisons, double *elapsed_seconds); // Mede tempo de execução de um algoritmo

/* Utilitários */
void trim_newline(char *s);                               // Remove '\n' ao fim da string
void copiarArray(Componente src[], Componente dest[], int n);  // Copia conteúdo de um array para outro
void trocar(Componente *a, Componente *b);                     // Troca dois itens do tipo Componente

/* ---------------------------
   Implementação das funções
   --------------------------- */

/* Remove '\n' no final de strings lidas com fgets */
void trim_newline(char *s) {
    size_t len = strlen(s);            // Calcula o comprimento da string
    if (len == 0) return;              // Evita erro se string estiver vazia
    if (s[len-1] == '\n') s[len-1] = '\0';   // Troca '\n' por '\0' (fim de string)
}

/* Copia o conteúdo de um array de componentes para outro */
void copiarArray(Componente src[], Componente dest[], int n) {
    for (int i = 0; i < n; ++i) {
        dest[i] = src[i];             // Copia cada componente individualmente
    }
}

/* Troca segura de dois componentes */
void trocar(Componente *a, Componente *b) {
    Componente tmp = *a;   // Guarda o valor de 'a'
    *a = *b;               // Atribui o valor de 'b' a 'a'
    *b = tmp;              // Finaliza a troca
}

/*
  Bubble Sort por nome (string).
  Conta comparações realizadas e atualiza *comparisons.
  Ordena em ordem crescente por nome (strcmp).
*/
void bubbleSortNome(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;      // Inicializa contagem de comparações
    int troca;
    for (int i = 0; i < n-1; ++i) {
        troca = 0;
        for (int j = 0; j < n-1-i; ++j) {
            (*comparisons)++;  // Conta comparação entre dois nomes
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) { // Compara strings
                trocar(&arr[j], &arr[j+1]);               // Troca se estiver fora de ordem
                troca = 1;    // Marca que houve troca
            }
        }
        if (!troca) break; // Se nenhuma troca foi feita, o array já está ordenado
    }
}

/*
  Insertion Sort por tipo (string).
  Conta comparações e atualiza *comparisons.
  Ordenação crescente por tipo com strcmp.
*/
void insertionSortTipo(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 1; i < n; ++i) {
        Componente key = arr[i];   // Guarda o item atual
        int j = i - 1;
        // Enquanto houver elementos maiores à esquerda, continue deslocando
        while (j >= 0) {
            (*comparisons)++;  // Conta comparação realizada
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j+1] = arr[j]; // Move item para a direita
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = key;       // Insere item na posição correta
    }
}

/*
  Selection Sort por prioridade (int).
  Ordena por prioridade em ordem crescente.
  Conta comparações numéricas realizadas.
*/
void selectionSortPrioridade(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n-1; ++i) {
        int min_idx = i;         // Assume que o menor está na posição i
        for (int j = i+1; j < n; ++j) {
            (*comparisons)++;    // Conta comparação numérica
            if (arr[j].prioridade < arr[min_idx].prioridade) {
                min_idx = j;     // Atualiza índice do menor
            }
        }
        if (min_idx != i) {
            trocar(&arr[i], &arr[min_idx]); // Troca elemento atual com o menor encontrado
        }
    }
}

/*
  Busca binária por nome.
  - O array deve estar ordenado por nome.
  - Retorna o índice encontrado ou -1 se não existir.
  - Conta comparações feitas.
*/
int buscaBinariaPorNome(Componente arr[], int n, const char key[], long *comparisons) {
    int left = 0, right = n - 1;
    *comparisons = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2; // Calcula o meio
        (*comparisons)++;                    // Conta comparação
        int cmp = strcmp(arr[mid].nome, key); // Compara o nome do meio com a chave buscada
        if (cmp == 0) {
            return mid;     // Nome encontrado
        } else if (cmp < 0) {
            left = mid + 1; // Busca na metade direita
        } else {
            right = mid - 1; // Busca na metade esquerda
        }
    }
    return -1;   // Não encontrado
}

/* Exibe os componentes em formato tabular */
void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\nLista de componentes (%d):\n", n);
    printf("+----+------------------------------+--------------------+-----------+\n");
    printf("| ID | Nome                         | Tipo               | Prioridade|\n");
    printf("+----+------------------------------+--------------------+-----------+\n");
    for (int i = 0; i < n; ++i) {
        printf("| %2d | %-28s | %-18s | %9d |\n", i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("+----+------------------------------+--------------------+-----------+\n");
}

/*
  Mede tempo de execução de um algoritmo de ordenação
  e também atualiza a contagem de comparações.
*/
void medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparisons, double *elapsed_seconds) {
    clock_t start = clock();  // Marca tempo inicial
    algoritmo(arr, n, comparisons); // Executa algoritmo recebido por ponteiro
    clock_t end = clock();    // Tempo final
    *elapsed_seconds = (double)(end - start) / (double)CLOCKS_PER_SEC; // Calcula tempo em segundos
}

/* ---------------------------
   Programa principal com menu
   --------------------------- */
int main(void) {
    Componente componentes[MAX_COMP];  // Array principal de componentes
    Componente trabalho[MAX_COMP];     // Array auxiliar para ordenação temporária
    int n = 0;                         // Número de componentes cadastrados
    char buffer[128];                  // Buffer auxiliar para leitura de strings

    /* Entrada de dados do usuário */
    printf("Cadastro de componentes (máx %d). Para parar, deixe o nome vazio.\n", MAX_COMP);
    for (int i = 0; i < MAX_COMP; ++i) {
        printf("\nComponente %d:\n", i+1);

        /* Nome */
        printf("  Nome (ex: chip central): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {  // Lê a entrada
            printf("Erro de leitura. Encerrando cadastro.\n");
            break;
        }
        trim_newline(buffer);                        // Remove '\n' do fim
        if (strlen(buffer) == 0) {                   // Se o nome for vazio, para o cadastro
            printf("Cadastro interrompido pelo usuário.\n");
            break;
        }
        strncpy(componentes[i].nome, buffer, MAX_NOME-1); // Copia o nome para a struct
        componentes[i].nome[MAX_NOME-1] = '\0';          // Garante que termina em '\0'

        /* Tipo */
        printf("  Tipo (ex: controle, suporte, propulsao): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro de leitura. Encerrando cadastro.\n");
            n = i+1; break;
        }
        trim_newline(buffer);
        if (strlen(buffer) == 0) {
            strcpy(componentes[i].tipo, "desconhecido"); // Se tipo vazio, define como padrão
        } else {
            strncpy(componentes[i].tipo, buffer, MAX_TIPO-1);
            componentes[i].tipo[MAX_TIPO-1] = '\0';
        }

        /* Prioridade */
        int prio = 0;
        while (1) {
            printf("  Prioridade (1 a 10): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) { // Lê valor
                printf("Erro de leitura. Prioridade = 1\n");
                prio = 1; break;
            }
            trim_newline(buffer);
            if (strlen(buffer)==0) { prio = 1; break; } // Se vazio, define prioridade 1
            prio = atoi(buffer);                        // Converte para int
            if (prio >= 1 && prio <= 10) break;         // Aceita apenas entre 1 e 10
            printf("  Valor inválido. Informe um inteiro entre 1 e 10.\n");
        }
        componentes[i].prioridade = prio; // Armazena prioridade na struct
        n = i + 1;                        // Atualiza número de componentes
        if (n >= MAX_COMP) {              // Interrompe se atingir o máximo
            printf("Número máximo de componentes (%d) atingido.\n", MAX_COMP);
            break;
        }
    }

    if (n == 0) {  // Se nenhum componente cadastrado, encerra
        printf("Nenhum componente cadastrado. Encerrando.\n");
        return 0;
    }

    /* Menu principal interativo */
    Componente working[MAX_COMP];        // Array de trabalho para ordenações
    int ordenado_por_nome = 0;           // Flag indica se array está ordenado por nome
    while (1) {
        printf("\n--- MENU ---\n");
        printf("1) Mostrar componentes cadastrados\n");
        printf("2) Ordenar por nome (Bubble Sort)\n");
        printf("3) Ordenar por tipo (Insertion Sort)\n");
        printf("4) Ordenar por prioridade (Selection Sort)\n");
        printf("5) Buscar componente-chave por nome (Busca binária) [Requer ordenação por nome]\n");
        printf("6) Mostrar montagem final (lista atual)\n");
        printf("0) Sair\n");
        printf("Escolha uma opção: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break; // Lê opção
        trim_newline(buffer);
        int opc = atoi(buffer);

        if (opc == 0) {
            printf("Saindo. Boa sorte na fuga!\n");
            break;           // Sai do loop e encerra programa
        } else if (opc == 1) {
            mostrarComponentes(componentes, n);        // Exibe lista de componentes
        } else if (opc == 2) {
            /* Ordena por nome usando Bubble Sort */
            copiarArray(componentes, working, n);      // Copia array original para o de trabalho
            long comparisons = 0;                      // Contador de comparações
            double elapsed = 0.0;                      // Tempo decorrido
            medirTempo(bubbleSortNome, working, n, &comparisons, &elapsed); // Executa e mede
            printf("\nResultado: Bubble Sort por NOME (ordem alfabética crescente)\n");
            mostrarComponentes(working, n);            // Mostra lista já ordenada
            printf("Comparações realizadas: %ld\n", comparisons);
            printf("Tempo (clock): %.6f s\n", elapsed);
            ordenado_por_nome = 1;                     // Marca que está ordenado por nome
            copiarArray(working, componentes, n);      // Copia lista ordenada de volta
        } else if (opc == 3) {
            /* Ordena por tipo usando Insertion Sort */
            copiarArray(componentes, working, n);
            long comparisons = 0;
            double elapsed = 0.0;
            medirTempo(insertionSortTipo, working, n, &comparisons, &elapsed);
            printf("\nResultado: Insertion Sort por TIPO (ordem alfabética crescente)\n");
            mostrarComponentes(working, n);
            printf("Comparações realizadas: %ld\n", comparisons);
            printf("Tempo (clock): %.6f s\n", elapsed);
            ordenado_por_nome = 0;                     // Agora não está ordenado por nome
            copiarArray(working, componentes, n);
        } else if (opc == 4) {
            /* Ordena por prioridade usando Selection Sort */
            copiarArray(componentes, working, n);
            long comparisons = 0;
            double elapsed = 0.0;
            medirTempo(selectionSortPrioridade, working, n, &comparisons, &elapsed);
            printf("\nResultado: Selection Sort por PRIORIDADE (crescente)\n");
            mostrarComponentes(working, n);
            printf("Comparações realizadas: %ld\n", comparisons);
            printf("Tempo (clock): %.6f s\n", elapsed);
            ordenado_por_nome = 0;
            copiarArray(working, componentes, n);
        } else if (opc == 5) {
            /* Busca binária por nome */
            if (!ordenado_por_nome) {     // Verifica se o array está ordenado por nome
                printf("Atenção: a busca binária requer que a lista esteja ordenada por NOME.\n");
                printf("Deseja ordenar agora por nome usando Bubble Sort? (s/n): ");
                if (!fgets(buffer, sizeof(buffer), stdin)) break;
                trim_newline(buffer);
                if (buffer[0] == 's' || buffer[0] == 'S') {
                    copiarArray(componentes, working, n);
                    long comp_sort = 0;
                    double elapsed_sort = 0.0;
                    medirTempo(bubbleSortNome, working, n, &comp_sort, &elapsed_sort);
                    printf("Ordenação realizada. Comparações: %ld, Tempo: %.6f s\n", comp_sort, elapsed_sort);
                    copiarArray(working, componentes, n);
                    ordenado_por_nome = 1;
                } else {
                    printf("Busca cancelada. Ordene por nome antes de usar busca binária.\n");
                    continue;
                }
            }

            printf("Digite o NOME do componente-chave a ser buscado: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            trim_newline(buffer);
            if (strlen(buffer) == 0) {
                printf("Nome vazio. Busca cancelada.\n");
                continue;
            }
            char key[MAX_NOME];
            strncpy(key, buffer, MAX_NOME-1);
            key[MAX_NOME-1] = '\0';

            long comp_search = 0;
            int idx = buscaBinariaPorNome(componentes, n, key, &comp_search);
            printf("Comparações na busca binária: %ld\n", comp_search);
            if (idx >= 0) {
                printf("Componente-chave encontrado no índice %d:\n", idx);
                printf("  Nome: %s\n  Tipo: %s\n  Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                printf("\n>>> Confirmação visual: componente-chave PRESENTE. Você pode ativar a torre.\n");
            } else {
                printf("Componente-chave '%s' NÃO encontrado na lista atual.\n", key);
            }
        } else if (opc == 6) {
            /* Mostra a lista de componentes atual */
            printf("\n=== Montagem final (lista atual de componentes) ===\n");
            mostrarComponentes(componentes, n);
            printf("Se quiser, busque o componente-chave com a opção 5 para confirmar sua presença.\n");
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
