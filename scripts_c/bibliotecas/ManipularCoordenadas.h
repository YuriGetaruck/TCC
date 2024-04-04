#ifndef MANIPULARCOORDENADAS_H
#define MANIPULARCOORDENADAS_H

// Estrutura para representar uma coordenada
typedef struct {
    int id;
    float x;
    float y;
    float z;
} CoordenadaEstrela;

// Função para calcular a distância entre duas coordenadas
float calcularDistanciaEntreDuasEstrelas(CoordenadaEstrela etr1, CoordenadaEstrela etr2);


#endif /* MANIPULARCOORDENADAS_H */
