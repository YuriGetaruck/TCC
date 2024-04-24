import numpy as np

class Point:
    def __init__(self, id, x, y, z):
        self.id = id
        self.x = x
        self.y = y
        self.z = z

def ler_coordenadas(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        coordenadas = []
        for i, linha in enumerate(arquivo): 
            partes = linha.strip().split()
            x, y, z = map(float, partes[:3])
            coordenadas.append(Point(i, x, y, z))  # Adicionando o ID como o número da linha
    return coordenadas

# Calcula a distância euclidiana entre dois pontos 3D
def distance(p1, p2):
    return np.sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2 + (p2.z - p1.z)**2)

def calcular_distancia_total(sequencia, pontos):
    distancia_total = 0.0
    for i in range(len(sequencia) - 1):
        ponto_atual = pontos[sequencia[i]]
        proximo_ponto = pontos[sequencia[i + 1]]
        distancia_total += distance(ponto_atual, proximo_ponto)
    # Adicione a distância do último ponto de volta ao primeiro ponto para completar o circuito
    ponto_final = pontos[sequencia[-1]]
    ponto_inicial = pontos[sequencia[0]]
    distancia_total += distance(ponto_final, ponto_inicial)
    return distancia_total

def calcular_distancia_total_string(sequencia_string, pontos):
    sequencia = list(map(int, sequencia_string.split()))
    return calcular_distancia_total(sequencia, pontos)

# Carregar coordenadas do arquivo
nome_arquivo = "C:\\Users\\Getaruck\\Documents\\TCC\\coordenadas\\star100.xyz.txt"
points = ler_coordenadas(nome_arquivo)

sequencia_otima = [0, 5, 10, 34, 48, 50, 85, 79, 80, 62, 65, 36, 25, 29, 77, 82, 69, 63, 89, 55, 67, 59, 45, 31, 27, 20, 33, 53, 49, 94, 92, 88, 37, 39, 60, 78, 81, 51, 57, 54, 61, 26, 72, 21, 15, 6, 22, 73, 97, 74, 76, 68, 56, 46, 64, 44, 90, 96, 71, 83, 58, 93, 84, 70, 52, 47, 32, 14, 13, 11, 12, 24, 17, 43, 86, 91, 75, 66, 99, 98, 95, 87, 42, 8, 19, 18, 30, 40, 38, 35, 28, 9, 16, 41, 23, 7, 4, 2, 1, 3]

distancia_total_otima = calcular_distancia_total(sequencia_otima, points)
print("Distacia total percorrida:", distancia_total_otima)


sequencia_string = "0 5 10 34 48 50 85 79 80 65 62 36 25 67 89 59 74 97 33 53 49 94 92 88 37 39 60 78 81 51 72 21 15 6 26 57 54 61 22 73 28 18 19 8 42 87 95 98 52 47 32 14 13 12 11 24 17 43 70 84 93 58 83 71 96 90 44 64 46 86 91 75 66 99 30 40 38 35 56 68 76 41 23 9 16 20 27 31 45 55 63 69 82 77 29 7 4 2 1 3"
distancia_total_string = calcular_distancia_total_string(sequencia_string, points)
print("Distância total percorrida:", distancia_total_string)