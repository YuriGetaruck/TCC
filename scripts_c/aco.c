#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define N 10           // número de pontos 3D
#define ALPHA 1.0      // parâmetro de influência de feromônio
#define BETA 2.0       // parâmetro de influência de visibilidade
#define RHO 0.5        // taxa de evaporação de feromônio
#define Q 100          // quantidade de feromônio depositada
#define ITERATIONS 100 // número de iterações
#define ANTS 10        // número de formigas

// Estrutura para representar um ponto 3D
typedef struct
{
    double x;
    double y;
    double z;
} Point;

// Calcula a distância euclidiana entre dois pontos 3D
double distance(Point p1, Point p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

// Inicializa os feromônios entre os pontos
void init_pheromones(double pheromones[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pheromones[i][j] = 1.0; // inicializa com um valor arbitrário
        }
    }
}

// Atualiza os feromônios baseado na trilha deixada pelas formigas
void update_pheromones(double pheromones[N][N], double delta_pheromones[N][N], double (*distances)[N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pheromones[i][j] *= (1 - RHO);              // evaporação de feromônio
            pheromones[i][j] += delta_pheromones[i][j]; // adição de feromônio
        }
    }
}

// Função de probabilidade para escolher o próximo ponto baseado em feromônio e visibilidade
int choose_next(double pheromones[N][N], double (*distances)[N], bool visited[N], int current)
{
    double total = 0.0;
    double probabilities[N];

    // Calcula as probabilidades para os pontos não visitados
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            probabilities[i] = pow(pheromones[current][i], ALPHA) * pow(1.0 / distances[current][i], BETA);
            total += probabilities[i];
        }
    }

    // Seleciona o próximo ponto baseado nas probabilidades
    double r = ((double)rand() / RAND_MAX) * total;
    double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            sum += probabilities[i];
            if (sum >= r)
            {
                return i;
            }
        }
    }

    // Se não foi possível selecionar um próximo ponto, retorna -1
    return -1;
}

// Resolve o problema do Caixeiro Viajante usando o algoritmo de colônia de formigas
void solve_tsp(Point points[N], double pheromones[N][N], double (*distances)[N])
{
    double delta_pheromones[N][N];
    for (int iteration = 0; iteration < ITERATIONS; iteration++)
    {
        // Inicializa as formigas em pontos aleatórios
        for (int ant = 0; ant < ANTS; ant++)
        {
            bool visited[N] = {false};
            int path[N];
            path[0] = rand() % N;
            visited[path[0]] = true;
            for (int i = 1; i < N; i++)
            {
                int next = choose_next(pheromones, distances, visited, path[i - 1]);
                path[i] = next;
                visited[next] = true;
            }

            // Calcula o comprimento do caminho
            double length = 0.0;
            for (int i = 0; i < N - 1; i++)
            {
                length += distances[path[i]][path[i + 1]];
            }
            length += distances[path[N - 1]][path[0]]; // fecha o ciclo

            // Atualiza os feromônios depositados pelas formigas
            for (int i = 0; i < N - 1; i++)
            {
                delta_pheromones[path[i]][path[i + 1]] += Q / length;
            }
            delta_pheromones[path[N - 1]][path[0]] += Q / length; // fecha o ciclo
        }

        // Atualiza os feromônios baseado na trilha deixada pelas formigas
        update_pheromones(pheromones, delta_pheromones, distances);

        // Zera a matriz de delta_pheromones para a próxima iteração
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                delta_pheromones[i][j] = 0.0;
            }
        }
    }
}

int main()
{
    srand(time(NULL));

    // Gerar pontos 3D aleatórios
    Point points[N];
    for (int i = 0; i < N; i++)
    {
        points[i].x = (double)rand() / RAND_MAX * 100;
        points[i].y = (double)rand() / RAND_MAX * 100;
        points[i].z = (double)rand() / RAND_MAX * 100;
    }

    // Calcular distâncias entre os pontos
    double distances[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            distances[i][j] = distance(points[i], points[j]);
        }
    }

    // Inicializar os feromônios
    double pheromones[N][N];
    init_pheromones(pheromones);

    // Resolver o problema do Caixeiro Viajante
    solve_tsp(points, pheromones, distances);

    // Imprimir os feromônios
    printf("Feromônios:\n");
    for (int i = 0; i < N; i++)
    {
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define N 10           // número de pontos 3D
#define ALPHA 1.0      // parâmetro de influência de feromônio
#define BETA 2.0       // parâmetro de influência de visibilidade
#define RHO 0.5        // taxa de evaporação de feromônio
#define Q 100          // quantidade de feromônio depositada
#define ITERATIONS 100 // número de iterações
#define ANTS 10        // número de formigas

        // Estrutura para representar um ponto 3D
        typedef struct
        {
            double x;
            double y;
            double z;
        } Point;

        // Calcula a distância euclidiana entre dois pontos 3D
        float distance(Point p1, Point p2)
        {
            return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
        }

        // Inicializa os feromônios entre os pontos
        void init_pheromones(double pheromones[N][N])
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    pheromones[i][j] = 1.0; // inicializa com um valor arbitrário
                }
            }
        }

        // Atualiza os feromônios baseado na trilha deixada pelas formigas
        void update_pheromones(double pheromones[N][N], double delta_pheromones[N][N], double(*distances)[N])
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    pheromones[i][j] *= (1 - RHO);              // evaporação de feromônio
                    pheromones[i][j] += delta_pheromones[i][j]; // adição de feromônio
                }
            }
        }

        // Função de probabilidade para escolher o próximo ponto baseado em feromônio e visibilidade
        int choose_next(double pheromones[N][N], double(*distances)[N], bool visited[N], int current)
        {
            double total = 0.0;
            double probabilities[N];

            // Calcula as probabilidades para os pontos não visitados
            for (int i = 0; i < N; i++)
            {
                if (!visited[i])
                {
                    probabilities[i] = pow(pheromones[current][i], ALPHA) * pow(1.0 / distances[current][i], BETA);
                    total += probabilities[i];
                }
            }

            // Seleciona o próximo ponto baseado nas probabilidades
            double r = ((double)rand() / RAND_MAX) * total;
            double sum = 0.0;
            for (int i = 0; i < N; i++)
            {
                if (!visited[i])
                {
                    sum += probabilities[i];
                    if (sum >= r)
                    {
                        return i;
                    }
                }
            }

            // Se não foi possível selecionar um próximo ponto, retorna -1
            return -1;
        }

        // Resolve o problema do Caixeiro Viajante usando o algoritmo de colônia de formigas
        void solve_tsp(Point points[N], double pheromones[N][N], double(*distances)[N])
        {
            double delta_pheromones[N][N];
            for (int iteration = 0; iteration < ITERATIONS; iteration++)
            {
                // Inicializa as formigas em pontos aleatórios
                for (int ant = 0; ant < ANTS; ant++)
                {
                    bool visited[N] = {false};
                    int path[N];
                    path[0] = rand() % N;
                    visited[path[0]] = true;
                    for (int i = 1; i < N; i++)
                    {
                        int next = choose_next(pheromones, distances, visited, path[i - 1]);
                        path[i] = next;
                        visited[next] = true;
                    }

                    // Calcula o comprimento do caminho
                    double length = 0.0;
                    for (int i = 0; i < N - 1; i++)
                    {
                        length += distances[path[i]][path[i + 1]];
                    }
                    length += distances[path[N - 1]][path[0]]; // fecha o ciclo

                    // Atualiza os feromônios depositados pelas formigas
                    for (int i = 0; i < N - 1; i++)
                    {
                        delta_pheromones[path[i]][path[i + 1]] += Q / length;
                    }
                    delta_pheromones[path[N - 1]][path[0]] += Q / length; // fecha o ciclo
                }

                // Atualiza os feromônios baseado na trilha deixada pelas formigas
                update_pheromones(pheromones, delta_pheromones, distances);

                // Zera a matriz de delta_pheromones para a próxima iteração
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        delta_pheromones[i][j] = 0.0;
                    }
                }
            }
        }

        int main()
        {
            srand(time(NULL));

            // Gerar pontos 3D aleatórios
            Point points[N];
            for (int i = 0; i < N; i++)
            {
                points[i].x = (double)rand() / RAND_MAX * 100;
                points[i].y = (double)rand() / RAND_MAX * 100;
                points[i].z = (double)rand() / RAND_MAX * 100;
            }

            // Calcular distâncias entre os pontos
            double distances[N][N];
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    distances[i][j] = distance(points[i], points[j]);
                }
            }

            // Inicializar os feromônios
            double pheromones[N][N];
            init_pheromones(pheromones);

            // Resolver o problema do Caixeiro Viajante
            solve_tsp(points, pheromones, distances);

            // Imprimir os feromônios
            printf("Feromônios:\n");
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    printf("%.2f ", pheromones[i][j]);
                }
                printf("\n");
            }

            return 0;
        }
