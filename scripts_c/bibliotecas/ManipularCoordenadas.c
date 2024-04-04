#include "ManipularCoordenadas.h"
#include <math.h>

// Função para calcular a distância entre dois pontos em 3D
float calcularDistanciasd(CoordenadaEstrela ponto1, CoordenadaEstrela ponto2)
{
    float distancia;
    distancia = sqrt(pow(ponto2.x - ponto1.x, 2) + pow(ponto2.y - ponto1.y, 2) + pow(ponto2.z - ponto1.z, 2));
    return distancia;
}
