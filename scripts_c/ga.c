#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define N 100              // número de pontos 3D
#define POP_SIZE 100       // tamanho da população
#define MUTATION_RATE 0.01 // taxa de mutação
#define ITERATIONS 100000  // número de iterações

// Estrutura para representar um ponto 3D
typedef struct
{
    int id;
    float x;
    float y;
    float z;
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
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

// Calcula o comprimento do caminho de um indivíduo
double calculate_fitness(Individual individual, Point points[N])
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
void initialize_population(Individual population[POP_SIZE])
{
    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < N; j++)
        {
            population[i].path[j] = j;
        }
        // Embaralha o caminho para criar indivíduos aleatórios
        for (int j = 0; j < N; j++)
        {
            int temp = population[i].path[j];
            int random_index = rand() % N;
            population[i].path[j] = population[i].path[random_index];
            population[i].path[random_index] = temp;
        }
        population[i].fitness = 0.0;
    }
}

// Seleciona indivíduos para reprodução usando torneio binário
Individual tournament_selection(Individual population[POP_SIZE])
{
    Individual parent1 = population[rand() % POP_SIZE];
    Individual parent2 = population[rand() % POP_SIZE];
    return (parent1.fitness < parent2.fitness) ? parent1 : parent2;
}

// Realiza o cruzamento entre dois indivíduos para gerar um novo indivíduo
Individual crossover(Individual parent1, Individual parent2)
{
    Individual child;
    int start = rand() % N;
    int end = rand() % N;
    if (start > end)
    {
        int temp = start;
        start = end;
        end = temp;
    }
    for (int i = start; i <= end; i++)
    {
        child.path[i] = parent1.path[i];
    }
    int index = 0;
    for (int i = 0; i < N; i++)
    {
        if (index == start)
        {
            index = end + 1;
        }
        bool contains = false;
        for (int j = start; j <= end; j++)
        {
            if (parent2.path[i] == child.path[j])
            {
                contains = true;
                break;
            }
        }
        if (!contains)
        {
            child.path[index] = parent2.path[i];
            index++;
        }
    }
    child.fitness = 0.0;
    return child;
}

// Realiza mutação em um indivíduo
void mutate(Individual *individual)
{
    if ((double)rand() / RAND_MAX < MUTATION_RATE)
    {
        int index1 = rand() % N;
        int index2 = rand() % N;
        int temp = individual->path[index1];
        individual->path[index1] = individual->path[index2];
        individual->path[index2] = temp;
    }
}

// Evolui a população por uma geração
void evolve_population(Individual population[POP_SIZE], Point points[N])
{
    Individual new_population[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++)
    {
        Individual parent1 = tournament_selection(population);
        Individual parent2 = tournament_selection(population);
        Individual child = crossover(parent1, parent2);
        mutate(&child);
        child.fitness = calculate_fitness(child, points);
        new_population[i] = child;
    }
    for (int i = 0; i < POP_SIZE; i++)
    {
        population[i] = new_population[i];
    }
}

// Encontra o melhor indivíduo na população
Individual find_best_individual(Individual population[POP_SIZE])
{
    Individual best = population[0];
    for (int i = 1; i < POP_SIZE; i++)
    {
        if (population[i].fitness < best.fitness)
        {
            best = population[i];
        }
    }
    return best;
}

// Função para extrair coordenadas
Point *lerCoordenadas(const char *nomeArquivo, int *tamanho)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Contar o número de linhas no arquivo
    int linhas = 0;
    float dummy_x, dummy_y, dummy_z;
    while (fscanf(arquivo, "%f %f %f", &dummy_x, &dummy_y, &dummy_z) == 3)
    {
        linhas++;
    }

    rewind(arquivo); // Voltar ao início do arquivo

    // Alocar dinamicamente o vetor de coordenadas
    Point *coordenadas = malloc(linhas * sizeof(Point));
    if (coordenadas == NULL)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Ler as coordenadas e armazenar no vetor
    for (int i = 0; i < linhas; i++)
    {
        if (fscanf(arquivo, "%f %f %f", &coordenadas[i].x, &coordenadas[i].y, &coordenadas[i].z) != 3)
        {
            fprintf(stderr, "Erro ao ler coordenadas do arquivo\n");
            exit(EXIT_FAILURE);
        }
        coordenadas[i].id = i + 1;
    }

    fclose(arquivo);

    // Definir o tamanho do vetor
    *tamanho = linhas;

    // Retornar o vetor de coordenadas
    return coordenadas;
}

int main()
{
    srand(time(NULL));

    // TESTANDO OBTENCAO DE COORDENADAS

    const char *nomeArquivo = "/Users/yurigetaruck/Documents/faculdade/TCC/coordenadas/star100.xyz.txt";
    int tamanho = 100; // Altere de acordo com o número total de coordenadas no arquivo
    Point *points = malloc(tamanho * sizeof(Point));

    if (points == NULL)
    {
        return EXIT_FAILURE;
    }

    points = lerCoordenadas(nomeArquivo, &tamanho);

    // Teste: imprimir as coordenadas
    for (int i = 0; i < tamanho; i++)
    {
        printf("ID: %d, X: %.6f, Y: %.6f, Z: %.6f\n", points[i].id, points[i].x, points[i].y, points[i].z);
    }

    // Inicializar a população
    Individual population[POP_SIZE];
    initialize_population(population);

    // Evoluir a população
    for (int i = 0; i < ITERATIONS; i++)
    {
        evolve_population(population, points);
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
