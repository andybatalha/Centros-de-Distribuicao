#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Declaração de constantes
#define MAX_CITIES 20

// Estrutura para armazenar os dados do problema
typedef struct {
    int N;  // Número de cidades
    int E;  // Número de conexões
    int P;  // Número de hubs
    int **distancia;  // Matriz de distâncias entre cidades
} Problema;

// Função para alocar a matriz de distâncias
void alocar_matriz_distancias(Problema *prob) {
    prob->distancia = (int **)malloc(prob->N * sizeof(int *));
    for (int i = 0; i < prob->N; i++) {
        prob->distancia[i] = (int *)malloc(prob->N * sizeof(int));
    }
}

// Função para liberar a matriz de distâncias
void liberar_matriz_distancias(Problema *prob) {
    for (int i = 0; i < prob->N; i++) {
        free(prob->distancia[i]);
    }
    free(prob->distancia);
}

// Função para ler informações das cidades de um arquivo
void read_input(Problema *prob, const char *cenario) {
    FILE *arquivo = fopen(cenario, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(arquivo, "%d %d %d", &prob->N, &prob->E, &prob->P);

    if (prob->N < 1 || prob->E < 0 || prob->P < 1 || prob->P > prob->N) {
        printf("Valores invalidos de N, E ou P no arquivo de entrada.\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    alocar_matriz_distancias(prob);

    for (int i = 0; i < prob->N; i++) {
        for (int j = 0; j < prob->N; j++) {
            fscanf(arquivo, "%d", &prob->distancia[i][j]);
            if (prob->distancia[i][j] == -1) {
                prob->distancia[i][j] = INT_MAX;
            }
        }
    }

    fclose(arquivo);
}

// Função para calcular a menor distância utilizando o algoritmo de Floyd-Warshall
void floyd_warshall(Problema *prob) {
    for (int k = 0; k < prob->N; k++) {
        for (int i = 0; i < prob->N; i++) {
            for (int j = 0; j < prob->N; j++) {
                if (prob->distancia[i][k] != INT_MAX && prob->distancia[k][j] != INT_MAX &&
                    prob->distancia[i][k] + prob->distancia[k][j] < prob->distancia[i][j]) {
                    prob->distancia[i][j] = prob->distancia[i][k] + prob->distancia[k][j];
                }
            }
        }
    }
}

// Função para verificar se uma combinação de hubs é válida
int combinacao_valida(int *hubs, int hubs_custo) {
    // Implementação para verificar se os hubs escolhidos são válidos
    return 1;  // No exemplo atual, assume que qualquer combinação de hubs é válida
}

// Função para calcular o custo com base nos hubs escolhidos
int calcular_custo(Problema *prob, int *hubs, int hubs_custo) {
    int total_custo = 0;
    for (int i = 0; i < prob->N; i++) {
        int min_distance = INT_MAX;
        for (int j = 0; j < hubs_custo; j++) {
            if (prob->distancia[i][hubs[j]] < min_distance) {
                min_distance = prob->distancia[i][hubs[j]];
            }
        }
        if (min_distance != INT_MAX) {
            total_custo += min_distance;
        }
    }
    return total_custo;
}

// Função para encontrar os melhores locais para os hubs de distribuição
void achar_melhor_local(Problema *prob) {
    int hubs[prob->P];
    int min_custo = INT_MAX;
    int melhor_hubs[prob->P];

    for (int i = 0; i < (1 << prob->N); i++) {
        int count = 0;
        for (int j = 0; j < prob->N; j++) {
            if (i & (1 << j)) {
                if (count < prob->P) {
                    hubs[count++] = j;
                }
            }
        }
        if (count == prob->P && combinacao_valida(hubs, prob->P)) {
            int custo = calcular_custo(prob, hubs, prob->P);
            if (custo < min_custo) {
                min_custo = custo;
                for (int k = 0; k < prob->P; k++) {
                    melhor_hubs[k] = hubs[k];
                }
            }
        }
    }

    if (min_custo == INT_MAX) {
        printf("Nao foi possível encontrar uma combinacao valida de hubs.\n");
    } else {
        printf("Melhores locais para os centros de distribuicao:\n");
        for (int i = 0; i < prob->P; i++) {
            printf("Cidade %d\n", melhor_hubs[i] + 1);
        }
        printf("Custo total minimo: %d\n", min_custo);
    }
}

// Função principal
int main() {
    Problema prob;
    char cenario[100];

    printf("Digite o nome do arquivo de entrada (por exemplo: sc1.txt): ");
    scanf("%s", cenario);

    read_input(&prob, cenario);

    floyd_warshall(&prob);

    achar_melhor_local(&prob);

    liberar_matriz_distancias(&prob);

    return EXIT_SUCCESS;
}