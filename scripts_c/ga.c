#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define N 100                // número de pontos 3D
#define POP_SIZE 30          // tamanho da população
#define MUTATION_RATE 0.05   // taxa de mutação
#define ITERATIONS 1000000   // número de iterações
#define TARGET_DISTANCE 1700 // valor específico para o critério de parada

// Estrutura para representar um ponto 3D
typedef struct
{
    int id;
    double x;
    double y;
    double z;
} Point;

// Estrutura para representar um indivíduo (caminho)
typedef struct
{
    int path[N];
    double fitness;
} Individual;

// Calcula a distância euclidiana entre dois pontos 3D
double distance(Point p1, Point p2)
{
    float distancia;
    distancia = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
    return distancia;
}

// Calcula o comprimento do caminho de um indivíduo
double calculate_fitness(Individual individual, Point points[])
{
    double length = 0.0;
    for (int i = 0; i < N - 1; i++)
    {
        length += distance(points[individual.path[i]], points[individual.path[i + 1]]);
    }
    length += distance(points[individual.path[N - 1]], points[individual.path[0]]); // fecha o ciclo
    return length;
}

// Inicializa uma população de indivíduos com caminhos aleatórios
void initialize_population(Individual population[], Point points[])
{
    // Loop através de cada indivíduo na população
    for (int i = 0; i < POP_SIZE; i++)
    {
        // Inicializar caminho com ordem sequencial
        for (int j = 0; j < N; j++)
        {
            population[i].path[j] = j;
        }

        // Embaralhar os pontos exceto o primeiro (ponto de partida)
        for (int j = N - 1; j > 0; j--)
        {
            // Gerar um índice aleatório dentro do intervalo [0, j]
            int k = rand() % (j + 1);

            // Trocar os valores dos índices j e k (exceto para o ponto inicial)
            if (k != 0) // Não embaralhar o ponto de partida
            {
                int temp = population[i].path[j];
                population[i].path[j] = population[i].path[k];
                population[i].path[k] = temp;
            }
        }

        // Calcular o fitness do indivíduo
        population[i].fitness = calculate_fitness(population[i], points);
    }
}

// Seleciona indivíduos para reprodução usando torneio binário
Individual tournament_selection(Individual population[])
{
    Individual parent1 = population[rand() % POP_SIZE];
    Individual parent2 = population[rand() % POP_SIZE];
    return (parent1.fitness < parent2.fitness) ? parent1 : parent2;
}

// Realiza o cruzamento entre dois indivíduos para gerar um novo indivíduo
Individual crossover(Individual parent1, Individual parent2, Point *points)
{
    int start = rand() % N;
    int end = rand() % N;
    if (start > end)
    {
        int temp = start;
        start = end;
        end = temp;
    }
    Individual child;
    for (int i = 0; i < N; i++)
    {
        if (i >= start && i <= end)
        {
            child.path[i] = parent1.path[i];
        }
        else
        {
            child.path[i] = -1;
        }
    }
    int index = 0;
    for (int i = 0; i < N; i++)
    {
        if (index == start)
        {
            index = end + 1;
        }
        int gene = parent2.path[i];
        if (child.path[index] == -1)
        {
            bool contains = false;
            for (int j = 0; j < N; j++)
            {
                if (child.path[j] == gene)
                {
                    contains = true;
                    break;
                }
            }
            if (!contains)
            {
                child.path[index++] = gene;
            }
        }
    }
    child.fitness = calculate_fitness(child, points);
    return child;
}

// Realiza mutação em um indivíduo
void mutate(Individual *individual)
{
    if ((double)rand() / RAND_MAX < MUTATION_RATE)
    {
        int start = rand() % (N - 1) + 1; // Ignorar o ponto de partida
        int end = rand() % (N - 1) + 1;   // Ignorar o ponto de partida
        if (start > end)
        {
            int temp = start;
            start = end;
            end = temp;
        }
        while (start < end)
        {
            int temp = individual->path[start];
            individual->path[start] = individual->path[end];
            individual->path[end] = temp;
            start++;
            end--;
        }
    }
    mutate_reverse_subpath(individual);
}

void mutate_reverse_subpath(Individual *individual)
{
    if ((double)rand() / RAND_MAX < MUTATION_RATE)
    {
        int num_reversals = rand() % 3 + 1; // Escolha aleatoriamente entre 1, 2 ou 3 índices para inverter
        for (int i = 0; i < num_reversals; i++)
        {
            int start = rand() % (N - 2) + 1; // Ignorar o ponto de partida e o último ponto
            int end = start + 1;
            int temp = individual->path[start];
            individual->path[start] = individual->path[end];
            individual->path[end] = temp;
        }
    }
}

// Evolui a população por uma geração e verifica o critério de parada
bool evolve_population(Individual population[], Point points[])
{
    Individual new_population[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++)
    {
        Individual parent1 = tournament_selection(population);
        Individual parent2 = tournament_selection(population);
        Individual child = crossover(parent1, parent2, points);
        mutate(&child);
        new_population[i] = child;
        if (child.fitness < TARGET_DISTANCE)
        { // Verifica o critério de parada
            for (int j = 0; j < POP_SIZE; j++)
            {
                population[j] = new_population[j];
            }
            return true;
        }
    }
    for (int i = 0; i < POP_SIZE; i++)
    {
        population[i] = new_population[i];
    }
    return false;
}

// Encontra o melhor indivíduo na população
Individual find_best_individual(Individual population[])
{
    Individual best_individual = population[0];
    for (int i = 1; i < POP_SIZE; i++)
    {
        if (population[i].fitness < best_individual.fitness)
        {
            best_individual = population[i];
        }
    }
    return best_individual;
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
    for (int i = 0; i < N; i++)
    {
        printf("ID: %d, X: %.6f, Y: %.6f, Z: %.6f\n", points[i].id, points[i].x, points[i].y, points[i].z);
    }
    fclose(arquivo);
}

int main()
{

    // Carregar coordenadas do arquivo
    const char *nome_arquivo = "C:\\Users\\Getaruck\\Documents\\TCC\\coordenadas\\star100.xyz.txt";
    Point points[N];
    ler_coordenadas(nome_arquivo, points);

    // Inicializar a população
    Individual population[POP_SIZE];
    initialize_population(population, points);

    // No loop principal, ajuste o controle de parada
    for (int i = 0; i < ITERATIONS; i++)
    {
        if (evolve_population(population, points))
        {
            printf("Parada atingida: caminho menor que %.2f encontrado\n", TARGET_DISTANCE);
            break;
        }
        if (i % 1000 == 0)
        {
            printf("Iteracao: %d\n", i);
            Individual best_individual = find_best_individual(population);
            printf("Comprimento do melhor caminho: %.2f\n", best_individual.fitness);
            printf("Caminho:\n");
            for (int i = 0; i < N; i++)
            {
                printf("%d ", best_individual.path[i]);
            }
            printf("\n");
        }
    }

    // Encontrar o melhor indivíduo
    Individual best_individual = find_best_individual(population);

    // Imprimir o melhor caminho encontrado
    printf("Melhor caminho encontrado:\n");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", best_individual.path[i]);
    }
    printf("\n");

    // Imprimir o comprimento do melhor caminho encontrado
    printf("Comprimento do melhor caminho: %.2f\n", best_individual.fitness);

    return 0;
}
