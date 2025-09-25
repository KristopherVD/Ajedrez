#include <iostream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

const int N = 8;
vector<vector<string>> tablero(N, vector<string>(N, " "));

void inicializarAmbiente()
{
    // Negras
    tablero[0] = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"};
    tablero[1] = {"♟", "♟", "♟", "♟", "♟", "♟", "♟", "♟"};
    // Blancas
    tablero[6] = {"♙", "♙", "♙", "♙", "♙", "♙", "♙", "♙"};
    tablero[7] = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"};
}

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

int letraAColumna(char letra)
{
    return letra - 'A';
}

int numeroAFila(char numero)
{
    return 8 - (numero - '0');
}

bool moverPieza(string origen, string destino)
{
    int colO = letraAColumna(toupper(origen[0]));
    int filO = numeroAFila(origen[1]);
    int colD = letraAColumna(toupper(destino[0]));
    int filD = numeroAFila(destino[1]);

    if (filO < 0 || filO >= N || colO < 0 || colO >= N ||
        filD < 0 || filD >= N || colD < 0 || colD >= N)
    {
        cout << "Movimiento fuera del tablero.\n";
        return false;
    }

    if (tablero[filO][colO] == " ")
    {
        cout << "No hay pieza en la casilla de origen.\n";
        return false;
    }

    tablero[filD][colD] = tablero[filO][colO];
    tablero[filO][colO] = " ";
    return true;
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Fuerza UTF-8 en Windows
#endif

    inicializarAmbiente();
    mostrarAmbiente();

    string origen, destino;
    int turno = 0;

    while (true)
    {
        cout << "\nTurno de " << (turno % 2 == 0 ? "BLANCAS" : "NEGRAS") << "\n";
        cout << "Ingresa movimiento (ej: E2 E4) o 'rendirse' para terminar: ";
        cin >> origen;

        if (origen == "rendirse")
        {
            cout << "La partida ha finalizado.\n";
            break;
        }

        cin >> destino;
        if (moverPieza(origen, destino))
        {
            mostrarAmbiente();
            turno++;
        }
    }

    return 0;
}
