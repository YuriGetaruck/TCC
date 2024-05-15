import numpy as np

# Define a função de distância euclidiana entre dois pontos 3D
def distance(point1, point2):
    return np.linalg.norm(point2 - point1)

# Define a função para inicializar as trilhas de feromônio
def initialize_pheromones(num_cities):
    return np.ones((num_cities, num_cities))

# Define a função para atualizar as trilhas de feromônio
def update_pheromones(pheromones, ants, distances):
    evaporation_rate = 0.1
    for ant in ants:
        for i in range(len(ant['tour']) - 1):
            city1 = ant['tour'][i]
            city2 = ant['tour'][i + 1]
            pheromones[city1, city2] += 1.0 / distances[city1, city2]
            pheromones[city2, city1] += 1.0 / distances[city1, city2]
    pheromones *= (1.0 - evaporation_rate)
    return pheromones

# Define a função para escolher a próxima cidade para uma formiga
def choose_next_city(ant_index, pheromones, distances, allowed_cities, alpha=1.0, beta=2.0):
    current_city = allowed_cities[ant_index][-1]
    probabilities = np.zeros(len(allowed_cities[ant_index]))
    for i, city in enumerate(allowed_cities[ant_index]):
        pheromone = pheromones[current_city, city]
        distance_to_city = distances[current_city, city]
        probabilities[i] = (pheromone ** alpha) * ((1.0 / distance_to_city) ** beta)
    probabilities /= probabilities.sum()
    next_city_index = np.random.choice(range(len(allowed_cities[ant_index])), p=probabilities)
    next_city = allowed_cities[ant_index][next_city_index]
    return next_city

# Define a função principal para resolver o TSP com ACO
def solve_tsp_ants(points, num_ants=10, num_iterations=100):
    num_cities = len(points)
    distances = np.zeros((num_cities, num_cities))
    for i in range(num_cities):
        for j in range(num_cities):
            distances[i, j] = distance(points[i], points[j])
    pheromones = initialize_pheromones(num_cities)
    best_tour = None
    best_length = np.inf
    for iteration in range(num_iterations):
        ants = [{'tour': [np.random.randint(num_cities)], 'visited': set()} for _ in range(num_ants)]
        for _ in range(num_cities - 1):
            for ant_index in range(num_ants):
                allowed_cities = [list(set(range(num_cities)) - ant['visited']) for ant in ants]
                next_city = choose_next_city(ant_index, pheromones, distances, allowed_cities)
                ants[ant_index]['tour'].append(next_city)
                ants[ant_index]['visited'].add(next_city)
        for ant in ants:
            tour_length = sum(distances[ant['tour'][i], ant['tour'][i + 1]] for i in range(num_cities - 1))
            tour_length += distances[ant['tour'][-1], ant['tour'][0]]
            if tour_length < best_length:
                best_length = tour_length
                best_tour = ant['tour']
        pheromones = update_pheromones(pheromones, ants, distances)
    return best_tour, best_length

# Pontos de exemplo em 3D
points = np.random.rand(10, 3)

# Resolver o TSP com ACO
best_tour, best_length = solve_tsp_ants(points)
print("Melhor caminho encontrado:", best_tour)
print("Comprimento do melhor caminho:", best_length)
