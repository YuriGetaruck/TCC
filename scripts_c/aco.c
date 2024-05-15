#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define N 100         // Tamanho da população de formigas
#define ALPHA 1.0     // Importância do feromônio
#define BETA 2.0      // Importância da visibilidade
#define RHO 0.5       // Taxa de evaporação do feromônio
#define Q 100         // Quantidade de feromônio depositado por formiga
#define MAX_ITER 1000 // Número máximo de iterações
#define INF 1e9       // Infinito

// Estrutura para representar um ponto 3D
typedef struct
{
    int id;
    double x;
    double y;
    double z;
} Point;

// Função para calcular a distância Euclidiana entre dois pontos 3D
double dist(Point a, Point b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

// Função para ler coordenadas de um arquivo
void ler_coordenadas(const char *nome_arquivo, Point points[])
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    int id;
    double x, y, z;
    for (int i = 0; i < N; i++)
    {
        fscanf(arquivo, "%lf %lf %lf", &x, &y, &z);
        points[i].id = i;
        points[i].x = x;
        points[i].y = y;
        points[i].z = z;
    }
    fclose(arquivo);
}

// Função para inicializar os feromônios entre os pontos
void init_pheromones(double **pheromones)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pheromones[i][j] = 1.0; // Valor inicial de feromônio
        }
    }
}

// Função para escolher o próximo ponto a ser visitado pela formiga
int choose_next_point(bool *visited, int current_point, double **pheromones, Point *points)
{
    double total_prob = 0.0;
    double probabilities[N];

    // Calcular a soma total das probabilidades para normalização
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            probabilities[i] = pow(pheromones[current_point][i], ALPHA) * pow(1.0 / dist(points[current_point], points[i]), BETA);
            total_prob += probabilities[i];
        }
        else
        {
            probabilities[i] = 0.0;
        }
    }

    // Escolher o próximo ponto com base nas probabilidades calculadas
    double r = ((double)rand() / RAND_MAX) * total_prob;
    double cumulative_prob = 0.0;
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            cumulative_prob += probabilities[i];
            if (cumulative_prob >= r)
            {
                return i;
            }
        }
    }

    // Se algo der errado, apenas retorne o próximo ponto não visitado
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            return i;
        }
    }

    // Se todos os pontos já foram visitados, retorne -1 (isso não deve acontecer)
    return -1;
}
// Função para calcular a distância total de um caminho
double calc_dist(Point *path, int n)
{
    double distance = 0.0;
    for (int i = 0; i < n - 1; i++)
    {
        distance += dist(path[i], path[i + 1]);
    }
    distance += dist(path[n - 1], path[0]); // Voltar ao ponto de origem
    return distance;
}

// Função principal para resolver o TSP usando colônia de formigas
void solve_tsp(Point *points)
{
    double **pheromones = malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++)
    {
        pheromones[i] = malloc(N * sizeof(double));
    }
    init_pheromones(pheromones);

    Point *best_path = malloc(N * sizeof(Point));
    double best_distance = INF;

    for (int iter = 0; iter < MAX_ITER; iter++)
    {
        // Implemente o movimento das formigas entre os pontos aqui
        for (int ant = 0; ant < N; ant++)
        {
            bool *visited = malloc(N * sizeof(bool));
            for (int i = 0; i < N; i++)
            {
                visited[i] = false;
            }

            int current_point = 0; // Começando do ponto 0
            visited[current_point] = true;

            for (int step = 1; step < N; step++)
            {
                int next_point = choose_next_point(visited, current_point, pheromones, points);
                // Atualize a trilha de feromônio entre current_point e next_point
                visited[next_point] = true;
                current_point = next_point;
            }
            free(visited);
        }
        // Atualize os feromônios na trilha

        // Encontre a melhor solução até agora
        double current_distance = calc_dist(points, N);
        if (current_distance < best_distance)
        {
            best_distance = current_distance;
            for (int i = 0; i < N; i++)
            {
                best_path[i] = points[i];
            }
        }

        printf("rodando");

        // Critério de parada
        if (best_distance < 2500)
        {
            break;
        }
    }

    // Imprima a melhor solução encontrada
    printf("Melhor caminho encontrado:\n");
    for (int i = 0; i < N; i++)
    {
        printf("ID: %d, X: %.6f, Y: %.6f, Z: %.6f\n", best_path[i].id, best_path[i].x, best_path[i].y, best_path[i].z);
    }
    printf("Distância total do caminho: %.6f\n", best_distance);

    // Libere a memória alocada
    free(best_path);
    for (int i = 0; i < N; i++)
    {
        free(pheromones[i]);
    }
    free(pheromones);
}

int main()
{
    Point points[N];
    ler_coordenadas("C:\\Users\\Getaruck\\Documents\\TCC\\coordenadas\\star1k.xyz.txt", points); // Substitua "pontos.txt" pelo nome do seu arquivo de pontos
    solve_tsp(points);
    return 0;
}
