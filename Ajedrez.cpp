#include <iostream>
#include <vector>
using namespace std;

// Tamaño del tablero
const int N = 8;

// Tablero de ajedrez
vector<vector<char>> tablero(N, vector<char>(N, ' '));

// Inicializar ambiente (colocar piezas)
void inicializarAmbiente()
{
    // Piezas negras
    tablero[0] = {'t', 'c', 'a', 'q', 'k', 'a', 'c', 't'};
    tablero[1] = {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};

    // Piezas blancas
    tablero[6] = {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};
    tablero[7] = {'T', 'C', 'A', 'Q', 'K', 'A', 'C', 'T'};
}

// Estado actual del ambiente
void mostrarAmbiente()
{
    cout << "\n    A   B   C   D   E   F   G   H\n";
    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < N; i++)
    {
        cout << 8 - i << " |";
        for (int j = 0; j < N; j++)
        {
            cout << " " << tablero[i][j] << " |";
        }
        cout << " " << 8 - i << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    A   B   C   D   E   F   G   H\n";
}

// Función para ejecutar un movimiento
void moverPieza(int filaOrigen, int colOrigen, int filaDestino, int colDestino)
{
    // Verifica si la casilla de origen tiene una pieza
    if (tablero[filaOrigen][colOrigen] == ' ')
    {
        cout << "No hay pieza en la casilla seleccionada.\n";
        return;
    }

    // Mover la pieza
    tablero[filaDestino][colDestino] = tablero[filaOrigen][colOrigen];
    tablero[filaOrigen][colOrigen] = ' ';

    cout << "Movimiento realizado.\n";
}

int main()
{
    inicializarAmbiente();
    mostrarAmbiente();

    // Ejemplo de movimiento: Peón blanco de E2 a E4
    cout << "\nEjemplo: mover peon blanco de E2 a E4...\n";
    moverPieza(6, 4, 4, 4); // fila 6,col 4 → fila 4,col 4
    mostrarAmbiente();

    return 0;
}
