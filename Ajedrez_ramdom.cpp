#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <iomanip> // Para formateo de salida (setw)
#include <cctype>  // Para toupper()
#include <map>     // Para map (diccionarios)
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

const int N = 8;
using Board = vector<vector<string>>;
Board tablero(N, vector<string>(N, " "));

// ---------- Inicialización del tablero ----------
void inicializarAmbiente()
{
    tablero[0] = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"};
    tablero[1] = {"♙", "♙", "♙", "♙", "♙", "♙", "♙", "♙"};
    tablero[6] = {"♟", "♟", "♟", "♟", "♟", "♟", "♟", "♟"};
    tablero[7] = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"};
}

// ---------- Mostrar tablero ----------
void mostrarAmbienteBoard(const Board &b)
{
    cout << "\n    A   B   C   D   E   F   G   H\n";
    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < N; i++)
    {
        cout << 8 - i << " |"; // Número de fila
        for (int j = 0; j < N; j++)
            cout << " " << b[i][j] << " |"; // Cada celda con pieza
        cout << " " << 8 - i << "\n";       // Número de fila al final
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    A   B   C   D   E   F   G   H\n";
}

// ---------- Helpers coordenadas ----------
int letraAColumna(char letra) { return toupper(letra) - 'A'; } // 'A'->0, 'B'->1, etc.
int numeroAFila(char numero) { return 8 - (numero - '0'); }    // '1'->7, '2'->6, etc.

string coords(int fila, int col)
{
    string s;
    s.push_back('A' + col);        // Convierte columna a letra
    s.push_back('0' + (8 - fila)); // Convierte fila a número
    return s;
}

// ---------- Clasificación ----------
bool esBlanca(const string &p)
{
    return (p == "♟" || p == "♜" || p == "♞" || p == "♝" || p == "♛" || p == "♚");
}
bool esNegra(const string &p)
{
    return (p == "♙" || p == "♖" || p == "♘" || p == "♗" || p == "♕" || p == "♔");
}

// ---------- Función auxiliar para verificar piezas del mismo color ----------
bool mismaPiezaColor(const string &pieza1, const string &pieza2)
{
    if (pieza1 == " " || pieza2 == " ")
        return false;
    return (esBlanca(pieza1) && esBlanca(pieza2)) ||
           (esNegra(pieza1) && esNegra(pieza2));
}

// ---------- Validación de movimientos ----------
bool esMovimientoValido(const Board &b, const string &pieza, int filO, int colO, int filD, int colD)
{
    int df = filD - filO;
    int dc = colD - colO;

    // Verificar límites del tablero
    if (filD < 0 || filD >= N || colD < 0 || colD >= N)
        return false;

    // No moverse al mismo lugar
    if (df == 0 && dc == 0)
        return false;

    // Verificar que no se capture una pieza del mismo color
    if (b[filD][colD] != " " && mismaPiezaColor(pieza, b[filD][colD]))
        return false;

    if (pieza == "♟")
    { // Peón negro
        if (dc == 0 && b[filD][colD] == " ")
        {
            if (df == -1)
                return true;
            if (filO == 6 && df == -2 && b[filO - 1][colO] == " ")
                return true;
        }
        // Comer en diagonal
        if (abs(dc) == 1 && df == -1 && b[filD][colD] != " " && esNegra(b[filD][colD]))
            return true;
        return false;
    }
    if (pieza == "♙")
    { // Peón blanco
        if (dc == 0 && b[filD][colD] == " ")
        {
            if (df == 1)
                return true;
            if (filO == 1 && df == 2 && b[filO + 1][colO] == " ")
                return true;
        }
        // Comer en diagonal
        if (abs(dc) == 1 && df == 1 && b[filD][colD] != " " && esBlanca(b[filD][colD]))
            return true;
        return false;
    }
    if (pieza == "♜" || pieza == "♖")
    { // Torre
        if (df == 0 || dc == 0)
        {
            // Verifica que no haya piezas en el camino
            int pasoF = (df == 0 ? 0 : (df > 0 ? 1 : -1));
            int pasoC = (dc == 0 ? 0 : (dc > 0 ? 1 : -1));
            int f = filO + pasoF, c = colO + pasoC;
            while (f != filD || c != colD) // Recorre todas las casillas intermedias
            {
                if (b[f][c] != " ")
                    return false; // Pieza en el camino
                f += pasoF;
                c += pasoC;
            }
            return true;
        }
        return false;
    }
    if (pieza == "♞" || pieza == "♘")
    { // Caballo
        return (abs(df) == 2 && abs(dc) == 1) || (abs(df) == 1 && abs(dc) == 2);
    }
    if (pieza == "♝" || pieza == "♗")
    { // Alfil
        if (abs(df) == abs(dc))
        {
            int pasoF = (df > 0 ? 1 : -1), pasoC = (dc > 0 ? 1 : -1);
            int f = filO + pasoF, c = colO + pasoC;
            while (f != filD && c != colD)
            {
                if (b[f][c] != " ")
                    return false;
                f += pasoF;
                c += pasoC;
            }
            return true;
        }
        return false;
    }
    if (pieza == "♛" || pieza == "♕")
    { //  Reina
        if (df == 0 || dc == 0 || abs(df) == abs(dc))
        {
            int pasoF = (df == 0 ? 0 : (df > 0 ? 1 : -1));
            int pasoC = (dc == 0 ? 0 : (dc > 0 ? 1 : -1));
            int f = filO + pasoF, c = colO + pasoC;
            while (f != filD || c != colD)
            {
                if (b[f][c] != " ")
                    return false;
                f += pasoF;
                c += pasoC;
            }
            return true;
        }
        return false;
    }
    if (pieza == "♚" || pieza == "♔")
    { // Rey
        return abs(df) <= 1 && abs(dc) <= 1;
    }

    return false;
}

// ---------- Estructura de movimiento ----------
struct Movimiento
{
    string pieza;
    string origen;
    string destino;
};

// ---------- Tabla agrupada con registro ----------
map<int, vector<Movimiento>> movimientosAgrupados; // Agrupa movimientos por ID

void mostrarJugadasPosibles(const Board &b, bool turnoBlancas)
{
    movimientosAgrupados.clear();
    cout << "+----+--------+---------+------------------+\n";
    cout << "| ID | Pieza  | Origen  | Posibles Destinos|\n";
    cout << "+----+--------+---------+------------------+\n";

    int id = 1;
    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = b[fil][col];
            if (pieza == " ")
                continue;
            if (turnoBlancas && !esBlanca(pieza))
                continue;
            if (!turnoBlancas && !esNegra(pieza))
                continue;

            vector<string> destinos;
            string origen = string(1, 'A' + col) + to_string(8 - fil);

            for (int f = 0; f < 8; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (esMovimientoValido(b, pieza, fil, col, f, c))
                    {
                        string destino = string(1, 'A' + c) + to_string(8 - f);
                        destinos.push_back(destino);
                        movimientosAgrupados[id].push_back({pieza, origen, destino});
                    }
                }
            }

            if (!destinos.empty())
            {
                cout << "| " << setw(2) << id << " | " << setw(6) << pieza
                     << " | " << setw(7) << origen << " | ";
                for (size_t i = 0; i < destinos.size(); i++)
                {
                    cout << destinos[i];
                    if (i < destinos.size() - 1)
                        cout << ", ";
                }
                cout << " |\n";
                id++;
            }
        }
    }
    cout << "+----+--------+---------+------------------+\n";
}

// ---------- Aplicar movimiento ----------
void aplicarMovimiento(Board &b, const Movimiento &m)
{
    int colO = m.origen[0] - 'A';
    int filO = 8 - (m.origen[1] - '0');
    int colD = m.destino[0] - 'A';
    int filD = 8 - (m.destino[1] - '0');
    b[filD][colD] = b[filO][colO];
    b[filO][colO] = " ";
}

// ---------- Movimiento automático para negras ----------
void movimientoAutomaticoNegras()
{
    if (movimientosAgrupados.empty())
    {
        cout << "No hay movimientos posibles para las negras.\n";
        return;
    }

    // Elegir un ID aleatorio de los disponibles
    vector<int> idsDisponibles;
    for (const auto &par : movimientosAgrupados)
    {
        idsDisponibles.push_back(par.first);
    }

    int idElegido = idsDisponibles[rand() % idsDisponibles.size()];
    auto &movimientosPieza = movimientosAgrupados[idElegido];

    // Elegir un destino aleatorio para esa pieza
    int destinoElegido = rand() % movimientosPieza.size();
    Movimiento movimientoElegido = movimientosPieza[destinoElegido];

    // Aplicar el movimiento
    aplicarMovimiento(tablero, movimientoElegido);

    cout << "La máquina mueve: " << movimientoElegido.pieza
         << " de " << movimientoElegido.origen
         << " a " << movimientoElegido.destino << endl;
}

// ---------- Main ----------
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    // Inicializar semilla para números aleatorios
    srand(time(0));

    inicializarAmbiente();
    int turno = 0;

    while (true)
    {
        bool turnoBlancas = (turno % 2 == 0);
        cout << "\nTurno de " << (turnoBlancas ? "BLANCAS" : "NEGRAS (Máquina)") << "\n";
        mostrarAmbienteBoard(tablero);

        if (turnoBlancas)
        {
            // Turno del jugador humano (blancas)
            mostrarJugadasPosibles(tablero, turnoBlancas);

            if (movimientosAgrupados.empty())
            {
                cout << "No hay movimientos posibles. Fin del juego.\n";
                break;
            }

            cout << "Elige el ID de la pieza (o 'r' para rendirse): ";
            string entrada;
            cin >> entrada;
            if (entrada == "r" || entrada == "R")
                break;

            int id;
            try
            {
                id = stoi(entrada);
            }
            catch (...)
            {
                cout << "Entrada invalida\n";
                continue;
            }

            if (movimientosAgrupados.find(id) == movimientosAgrupados.end())
            {
                cout << "ID no valido.\n";
                continue;
            }

            auto &movs = movimientosAgrupados[id];
            cout << "Has elegido la pieza " << movs[0].pieza
                 << " en " << movs[0].origen << endl;

            if (movs.size() == 1)
            {
                aplicarMovimiento(tablero, movs[0]);
                cout << "Movimiento automatico a " << movs[0].destino << endl;
            }
            else
            {
                cout << "Destinos disponibles:\n";
                for (size_t i = 0; i < movs.size(); i++)
                {
                    cout << i + 1 << ") " << movs[i].destino << "  ";
                }
                cout << endl;

                int opcion;
                cout << "Elige un destino: ";
                cin >> opcion;

                if (opcion < 1 || opcion > static_cast<int>(movs.size()))
                {
                    cout << "Destino no valido.\n";
                    continue;
                }
                aplicarMovimiento(tablero, movs[opcion - 1]);
            }
        }
        else
        {
            // Turno de la máquina (negras)
            // Calcular movimientos posibles pero no mostrar la tabla
            movimientosAgrupados.clear();
            for (int fil = 0; fil < 8; fil++)
            {
                for (int col = 0; col < 8; col++)
                {
                    string pieza = tablero[fil][col];
                    if (pieza == " ")
                        continue;
                    if (!esNegra(pieza))
                        continue;

                    string origen = string(1, 'A' + col) + to_string(8 - fil);
                    for (int f = 0; f < 8; f++)
                    {
                        for (int c = 0; c < 8; c++)
                        {
                            if (esMovimientoValido(tablero, pieza, fil, col, f, c))
                            {
                                string destino = string(1, 'A' + c) + to_string(8 - f);
                                // Usamos ID 1 temporalmente para agrupar
                                movimientosAgrupados[1].push_back({pieza, origen, destino});
                            }
                        }
                    }
                }
            }

            movimientoAutomaticoNegras();
        }

        turno++;
    }
    return 0;
}