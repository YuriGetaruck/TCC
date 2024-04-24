import numpy as np
import random

N = 100                # número de pontos 3D
POP_SIZE = 10         # tamanho da população
MUTATION_RATE = 0.10   # taxa de mutação
ITERATIONS = 1000000   # número de iterações
TARGET_DISTANCE = 2000 # valor específico para o critério de parada

# Estrutura para representar um ponto 3D
class Point:
    def __init__(self, id, x, y, z):
        self.id = id
        self.x = x
        self.y = y
        self.z = z

# Estrutura para representar um indivíduo (caminho)
class Individual:
    def __init__(self, path):
        self.path = path
        self.fitness = 0.0

# Calcula a distância euclidiana entre dois pontos 3D
def distance(p1, p2):
    return np.sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2 + (p2.z - p1.z)**2)

# Calcula o comprimento do caminho de um indivíduo
def calculate_fitness(individual, points):
    length = 0.0
    for i in range(N - 1):
        length += distance(points[individual.path[i]], points[individual.path[i + 1]])
    length += distance(points[individual.path[N - 1]], points[individual.path[0]]) # fecha o ciclo
    return length

# Inicializa uma população de indivíduos com caminhos aleatórios
def initialize_population():
    population = []
    for _ in range(POP_SIZE):
        path = list(range(N))
        random.shuffle(path)
        population.append(Individual(path))
    return population

# Seleciona indivíduos para reprodução usando torneio binário
def tournament_selection(population):
    parent1 = random.choice(population)
    parent2 = random.choice(population)
    return parent1 if parent1.fitness < parent2.fitness else parent2

# Realiza o cruzamento entre dois indivíduos para gerar um novo indivíduo
def crossover(parent1, parent2):
    start = random.randint(0, N - 1)
    end = random.randint(0, N - 1)
    if start > end:
        start, end = end, start
    child = [-1] * N
    for i in range(start, end + 1):
        child[i] = parent1.path[i]
    index = 0
    for i in range(N):
        if index == start:
            index = end + 1
        if parent2.path[i] not in child:
            child[index] = parent2.path[i]
            index += 1
    return Individual(child)

# Realiza mutação em um indivíduo
def mutate(individual):
    if random.random() < MUTATION_RATE:
        index1 = random.randint(0, N - 1)
        index2 = random.randint(0, N - 1)
        individual.path[index1], individual.path[index2] = individual.path[index2], individual.path[index1]

# Evolui a população por uma geração e verifica o critério de parada
def evolve_population(population, points):
    new_population = []
    for _ in range(POP_SIZE):
        parent1 = tournament_selection(population)
        parent2 = tournament_selection(population)
        child = crossover(parent1, parent2)
        mutate(child)
        child.fitness = calculate_fitness(child, points)
        new_population.append(child)
        if child.fitness < TARGET_DISTANCE: # Verifica o critério de parada
            return True
    population[:] = new_population
    return False

# Encontra o melhor indivíduo na população
def find_best_individual(population):
    return min(population, key=lambda x: x.fitness)

# Função para ler coordenadas de um arquivo
def ler_coordenadas(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        coordenadas = []
        for linha in arquivo:
            partes = linha.strip().split()
            x, y, z = map(float, partes[:3])  # Ajuste para pegar apenas os três primeiros elementos da linha
            coordenadas.append(Point(None, x, y, z))  # Não usaremos o ID
    return coordenadas

random.seed()

# Carregar coordenadas do arquivo
nome_arquivo = "C:\\Users\\Getaruck\\Documents\\TCC\\coordenadas\\star100.xyz.txt"
points = ler_coordenadas(nome_arquivo)

# Inicializar a população
population = initialize_population()

# No loop principal, ajuste o controle de parada
for i in range(ITERATIONS):
    if evolve_population(population, points):
        print("Parada atingida: caminho menor que %.2f encontrado" % TARGET_DISTANCE)
        break
    if i % 1000 == 0:
        print("Iteração:", i)
        best_individual = find_best_individual(population)
        print("Comprimento do melhor caminho: %.2f" % best_individual.fitness)

# Encontrar o melhor indivíduo
best_individual = find_best_individual(population)

# Imprimir o melhor caminho encontrado
print("Melhor caminho encontrado:")
print(best_individual.path)

# Imprimir o comprimento do melhor caminho encontrado
print("Comprimento do melhor caminho: %.2f" % best_individual.fitness)
