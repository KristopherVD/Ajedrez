#include <iostream>
#include <vector>
#include <string>
#include <cctype>
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
    // Top: "negras visuales"
    tablero[0] = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"};
    tablero[1] = {"♙", "♙", "♙", "♙", "♙", "♙", "♙", "♙"};
    // Bottom: "blancas visuales"
    tablero[6] = {"♟", "♟", "♟", "♟", "♟", "♟", "♟", "♟"};
    tablero[7] = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"};
}

// ---------- Mostrar un tablero cualquiera (no solo el global) ----------
void mostrarAmbienteBoard(const Board &b)
{
    cout << "\n    A   B   C   D   E   F   G   H\n";

    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < N; i++)
    {
        cout << 8 - i << " |";
        for (int j = 0; j < N; j++)
        {
            cout << " " << b[i][j] << " |";
        }
        cout << " " << 8 - i << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }

    cout << "    A   B   C   D   E   F   G   H\n";
}

// ---------- Helpers de coordenadas ----------
int letraAColumna(char letra) { return toupper(letra) - 'A'; }
int numeroAFila(char numero) { return 8 - (numero - '0'); }

// ---------- Clasificación de piezas por "bando" ----------
bool esBlanca(const string &p)
{
    // Blancas = símbolos rellenitos (♟, ♜, ♞, ♝, ♛, ♚)
    return (p == "♟" || p == "♜" || p == "♞" || p == "♝" || p == "♛" || p == "♚");
}
bool esNegra(const string &p)
{
    // Negras = símbolos huecos (♙, ♖, ♘, ♗, ♕, ♔)
    return (p == "♙" || p == "♖" || p == "♘" || p == "♗" || p == "♕" || p == "♔");
}

// ---------- Validación de movimientos ----------
// IMPORTANTE: ahora usa 'b' (el tablero pasado) en todas las comprobaciones
bool esMovimientoValido(const Board &b, const string &pieza, int filO, int colO, int filD, int colD)
{
    int df = filD - filO;
    int dc = colD - colO;

    // Fuera de rango
    if (filD < 0 || filD >= N || colD < 0 || colD >= N)
        return false;

    // No permitir quedarse en el mismo lugar
    if (df == 0 && dc == 0)
        return false;

    // --- PEONES ---
    if (pieza == "♟") // "blanco" lógico (relleno)
    {
        // Avance simple o doble (sin captura)
        if (dc == 0 && b[filD][colD] == " ")
        {
            if (df == -1)
                return true;
            if (filO == 6 && df == -2 && b[filO - 1][colO] == " ")
                return true;
        }
        // Captura diagonal
        if (abs(dc) == 1 && df == -1 && esNegra(b[filD][colD]))
            return true;
        return false;
    }
    if (pieza == "♙") // "negro" lógico (hueco)
    {
        if (dc == 0 && b[filD][colD] == " ")
        {
            if (df == 1)
                return true;
            if (filO == 1 && df == 2 && b[filO + 1][colO] == " ")
                return true;
        }
        if (abs(dc) == 1 && df == 1 && esBlanca(b[filD][colD]))
            return true;
        return false;
    }

    // --- TORRES ---
    if (pieza == "♜" || pieza == "♖")
    {
        if (df == 0 || dc == 0)
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

    // --- CABALLOS ---
    if (pieza == "♞" || pieza == "♘")
    {
        if ((abs(df) == 2 && abs(dc) == 1) || (abs(df) == 1 && abs(dc) == 2))
            return true;
        return false;
    }

    // --- ALFILES ---
    if (pieza == "♝" || pieza == "♗")
    {
        if (abs(df) == abs(dc))
        {
            int pasoF = (df > 0 ? 1 : -1);
            int pasoC = (dc > 0 ? 1 : -1);
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

    // --- REINA ---
    if (pieza == "♛" || pieza == "♕")
    {
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

    // --- REY ---
    if (pieza == "♚" || pieza == "♔")
    {
        if (abs(df) <= 1 && abs(dc) <= 1)
            return true;
        return false;
    }

    return false;
}

// ---------- Estructura para almacenar un estado posible ----------
struct EstadoPosible
{
    Board board;
    string mov; // "A2 A4" representación del movimiento que generó este estado
};

// ---------- Generar todos los estados posibles para el jugador en turno ----------
vector<EstadoPosible> generarEstadosPosibles(const Board &b, bool turnoBlancas)
{
    vector<EstadoPosible> resultados;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            string pieza = b[i][j];
            if (pieza == " ")
                continue;
            // comprobar si la pieza pertenece al jugador en turno
            if (turnoBlancas && !esBlanca(pieza))
                continue;
            if (!turnoBlancas && !esNegra(pieza))
                continue;

            // probar todas las casillas destino
            for (int di = 0; di < N; ++di)
            {
                for (int dj = 0; dj < N; ++dj)
                {
                    // evitar mover a la misma casilla
                    if (di == i && dj == j)
                        continue;

                    // validar movimiento según reglas y ocupación (ahora usa 'b')
                    if (!esMovimientoValido(b, pieza, i, j, di, dj))
                        continue;

                    // validar que no capture pieza propia
                    string destinoP = b[di][dj];
                    if ((esBlanca(pieza) && esBlanca(destinoP)) || (esNegra(pieza) && esNegra(destinoP)))
                        continue;

                    // crear copia del tablero y aplicar movimiento
                    Board copia = b;
                    copia[di][dj] = copia[i][j];
                    copia[i][j] = " ";

                    // construir notación simple (e.g. "A2 A4")
                    char colO = 'A' + j;
                    char filO = '0' + (8 - i);
                    char colD = 'A' + dj;
                    char filD = '0' + (8 - di);
                    string mov;
                    mov.push_back(colO);
                    mov.push_back(filO);
                    mov.push_back(' ');
                    mov.push_back(colD);
                    mov.push_back(filD);

                    resultados.push_back(EstadoPosible{copia, mov});
                }
            }
        }
    }

    return resultados;
}

// ---------- Contar piezas en un tablero dado ----------
pair<int, int> contarPiezasBoard(const Board &b)
{
    int blancas = 0, negras = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            if (esBlanca(b[i][j]))
                blancas++;
            if (esNegra(b[i][j]))
                negras++;
        }
    return {blancas, negras};
}

// ---------- moverPieza simplificado: usa indices ya validados  ----------
void aplicarMovimientoDirecto(Board &b, int filO, int colO, int filD, int colD)
{
    b[filD][colD] = b[filO][colO];
    b[filO][colO] = " ";
}

// ---------- Principal ----------
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // UTF-8 en Windows
#endif

    inicializarAmbiente();

    int turno = 0; // 0 -> blancas, 1 -> negras

    while (true)
    {
        bool turnoBlancas = (turno % 2 == 0);
        cout << "\nEstado actual (turno de " << (turnoBlancas ? "BLANCAS" : "NEGRAS") << "):\n";
        mostrarAmbienteBoard(tablero);
        auto cuentas = contarPiezasBoard(tablero);
        cout << "Piezas en juego -> Blancas: " << cuentas.first << ", Negras: " << cuentas.second << "\n";

        // Generar todos los estados siguientes posibles
        auto estados = generarEstadosPosibles(tablero, turnoBlancas);
        if (estados.empty())
        {
            cout << "\nNo hay movimientos legales para este jugador. Fin de la partida o pase.\n";
            break;
        }

        cout << "\nSe generaron " << estados.size() << " estados posibles. Mostrando cada uno numerado:\n\n";

        // Mostrar cada estado con su índice y movimiento asociado
        for (size_t k = 0; k < estados.size(); ++k)
        {
            cout << "---- Estado " << (k + 1) << "  (Movimiento: " << estados[k].mov << ") ----\n";
            mostrarAmbienteBoard(estados[k].board);
            auto c = contarPiezasBoard(estados[k].board);
            cout << "Piezas -> Blancas: " << c.first << ", Negras: " << c.second << "\n\n";
        }

        // Pedir al jugador que elija índice o se rinda
        cout << "Ingresa el numero del estado que quieres jugar (1 - " << estados.size() << ") (o 'r' para rendirte): ";
        string entrada;
        cin >> entrada;
        if (entrada == "r" || entrada == "R" || entrada == "rendirse")
        {
            cout << "Jugador en turno se rindió. Fin de la partida.\n";
            break;
        }

        // validar índice
        int idx;
        try
        {
            idx = stoi(entrada);
        }
        catch (...)
        {
            cout << "Entrada inválida. Intenta de nuevo.\n";
            continue;
        }
        if (idx < 1 || idx > (int)estados.size())
        {
            cout << "Indice fuera de rango. Intenta de nuevo.\n";
            continue;
        }

        // Actualizar tablero principal con la copia elegida
        tablero = estados[idx - 1].board;
        cout << "Se jugó el movimiento: " << estados[idx - 1].mov << "\n";

        // Avanzar turno
        turno++;
    }

    return 0;
}