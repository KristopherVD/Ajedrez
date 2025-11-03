#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <iomanip>
#include <cctype>
#include <map>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

const int N = 8;
using Board = vector<vector<string>>;
Board tablero(N, vector<string>(N, " "));

// ---------- Tipos de jugador ----------
enum TipoJugador
{
    HUMANO,
    REACTIVO,
    MINMAX
};

// Configuración de jugadores
TipoJugador jugadorBlancas = HUMANO;
TipoJugador jugadorNegras = REACTIVO;

// ---------- Declaraciones anticipadas ----------
bool esMovimientoValido(const Board &b, const string &pieza, int filO, int colO, int filD, int colD);
bool esBlanca(const string &p);
bool esNegra(const string &p);
int evaluarTablero(const Board &b);
int minMax(Board &b, int profundidad, bool esMaximizador, int alfa, int beta);

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
        cout << 8 - i << " |";
        for (int j = 0; j < N; j++)
            cout << " " << b[i][j] << " |";
        cout << " " << 8 - i << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    A   B   C   D   E   F   G   H\n";
}

// ---------- Helpers coordenadas ----------
int letraAColumna(char letra) { return toupper(letra) - 'A'; }
int numeroAFila(char numero) { return 8 - (numero - '0'); }

string coords(int fila, int col)
{
    string s;
    s.push_back('A' + col);
    s.push_back('0' + (8 - fila));
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
        if (abs(dc) == 1 && df == 1 && b[filD][colD] != " " && esBlanca(b[filD][colD]))
            return true;
        return false;
    }
    if (pieza == "♜" || pieza == "♖")
    { // Torre
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
    { // Reina
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
    bool esCaptura;
    int valor; // Para MinMax
};

// ---------- Tabla agrupada con registro ----------
map<int, vector<Movimiento>> movimientosAgrupados;

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
                        bool captura = (b[f][c] != " ");
                        movimientosAgrupados[id].push_back({pieza, origen, destino, captura, 0});
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

// ---------- Encontrar posición del rey ----------
pair<int, int> encontrarRey(const Board &b, bool buscarBlanco)
{
    string reyBuscado = buscarBlanco ? "♚" : "♔";
    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (b[fil][col] == reyBuscado)
            {
                return {fil, col};
            }
        }
    }
    return {-1, -1};
}

// ---------- Verificar jaque ----------
bool estaEnJaque(const Board &b, bool esReyBlanco)
{
    auto posRey = encontrarRey(b, esReyBlanco);
    int reyFil = posRey.first;
    int reyCol = posRey.second;

    if (reyFil == -1)
        return false; // No hay rey (no debería pasar)

    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = b[fil][col];
            if (pieza == " ")
                continue;

            if ((esReyBlanco && esNegra(pieza)) || (!esReyBlanco && esBlanca(pieza)))
            {
                if (esMovimientoValido(b, pieza, fil, col, reyFil, reyCol))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// ---------- Verificar si hay jaque mate ----------
bool esJaqueMate(const Board &b, bool esReyBlanco)
{
    if (!estaEnJaque(b, esReyBlanco))
        return false;

    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = b[fil][col];
            if (pieza == " ")
                continue;

            if ((esReyBlanco && !esBlanca(pieza)) || (!esReyBlanco && !esNegra(pieza)))
                continue;

            for (int f = 0; f < 8; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (esMovimientoValido(b, pieza, fil, col, f, c))
                    {
                        Board tableroTemp = b;
                        string origen = coords(fil, col);
                        string destino = coords(f, c);
                        Movimiento movTemp = {pieza, origen, destino, false, 0};
                        aplicarMovimiento(tableroTemp, movTemp);

                        if (!estaEnJaque(tableroTemp, esReyBlanco))
                            return false;
                    }
                }
            }
        }
    }

    return true;
}

// ---------- Función de evaluación para MinMax ----------
int evaluarTablero(const Board &b)
{
    int valor = 0;

    // Valores de las piezas
    map<string, int> valores = {
        {"♙", 10}, {"♖", 50}, {"♘", 30}, {"♗", 30}, {"♕", 90}, {"♔", 900}, {"♟", -10}, {"♜", -50}, {"♞", -30}, {"♝", -30}, {"♛", -90}, {"♚", -900}};

    // Sumar valores de todas las piezas
    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = b[fil][col];
            if (pieza != " ")
            {
                valor += valores[pieza];
            }
        }
    }

    return valor;
}

// ---------- Algoritmo MinMax con poda alfa-beta ----------
int minMax(Board &b, int profundidad, bool esMaximizador, int alfa, int beta)
{
    if (profundidad == 0)
    {
        return evaluarTablero(b);
    }

    vector<Movimiento> movimientosPosibles;

    // Generar todos los movimientos posibles
    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = b[fil][col];
            if (pieza == " ")
                continue;

            bool piezaEsBlanca = esBlanca(pieza);
            if ((esMaximizador && !piezaEsBlanca) || (!esMaximizador && piezaEsBlanca))
                continue;

            for (int f = 0; f < 8; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (esMovimientoValido(b, pieza, fil, col, f, c))
                    {
                        string origen = coords(fil, col);
                        string destino = coords(f, c);
                        bool captura = (b[f][c] != " ");
                        movimientosPosibles.push_back({pieza, origen, destino, captura, 0});
                    }
                }
            }
        }
    }

    if (movimientosPosibles.empty())
    {
        return evaluarTablero(b);
    }

    if (esMaximizador)
    {
        int maxEval = -10000;
        for (auto &mov : movimientosPosibles)
        {
            Board tableroTemp = b;
            aplicarMovimiento(tableroTemp, mov);
            int eval = minMax(tableroTemp, profundidad - 1, false, alfa, beta);
            maxEval = max(maxEval, eval);
            alfa = max(alfa, eval);
            if (beta <= alfa)
                break;
        }
        return maxEval;
    }
    else
    {
        int minEval = 10000;
        for (auto &mov : movimientosPosibles)
        {
            Board tableroTemp = b;
            aplicarMovimiento(tableroTemp, mov);
            int eval = minMax(tableroTemp, profundidad - 1, true, alfa, beta);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alfa)
                break;
        }
        return minEval;
    }
}

// ---------- Movimiento del Agente Reactivo ----------
Movimiento movimientoReactivo(bool esBlancas)
{
    vector<Movimiento> captures;
    vector<Movimiento> nonCaptures;

    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = tablero[fil][col];
            if (pieza == " ")
                continue;
            if ((esBlancas && !esBlanca(pieza)) || (!esBlancas && !esNegra(pieza)))
                continue;

            string origen = string(1, 'A' + col) + to_string(8 - fil);

            for (int f = 0; f < 8; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (!esMovimientoValido(tablero, pieza, fil, col, f, c))
                        continue;
                    string destino = string(1, 'A' + c) + to_string(8 - f);

                    bool captura = false;
                    if (esBlancas && tablero[f][c] != " " && esNegra(tablero[f][c]))
                        captura = true;
                    else if (!esBlancas && tablero[f][c] != " " && esBlanca(tablero[f][c]))
                        captura = true;

                    if (captura)
                        captures.push_back({pieza, origen, destino, true, 0});
                    else
                        nonCaptures.push_back({pieza, origen, destino, false, 0});
                }
            }
        }
    }

    if (!captures.empty())
    {
        return captures[rand() % captures.size()];
    }
    else if (!nonCaptures.empty())
    {
        return nonCaptures[rand() % nonCaptures.size()];
    }

    return {"", "", "", false, 0};
}

// ---------- Movimiento del Agente MinMax ----------
Movimiento movimientoMinMax(bool esBlancas)
{
    vector<Movimiento> mejoresMovimientos;
    int mejorValor = esBlancas ? -10000 : 10000;
    int profundidad = 2; // Profundidad del árbol de búsqueda

    // Generar todos los movimientos posibles
    for (int fil = 0; fil < 8; fil++)
    {
        for (int col = 0; col < 8; col++)
        {
            string pieza = tablero[fil][col];
            if (pieza == " ")
                continue;
            if ((esBlancas && !esBlanca(pieza)) || (!esBlancas && !esNegra(pieza)))
                continue;

            string origen = string(1, 'A' + col) + to_string(8 - fil);

            for (int f = 0; f < 8; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (!esMovimientoValido(tablero, pieza, fil, col, f, c))
                        continue;
                    string destino = string(1, 'A' + c) + to_string(8 - f);

                    // Simular movimiento
                    Board tableroTemp = tablero;
                    Movimiento mov = {pieza, origen, destino, false, 0};
                    aplicarMovimiento(tableroTemp, mov);

                    // Evaluar con MinMax
                    int valor = minMax(tableroTemp, profundidad - 1, !esBlancas, -10000, 10000);
                    mov.valor = valor;

                    if (esBlancas)
                    {
                        if (valor > mejorValor)
                        {
                            mejorValor = valor;
                            mejoresMovimientos.clear();
                            mejoresMovimientos.push_back(mov);
                        }
                        else if (valor == mejorValor)
                        {
                            mejoresMovimientos.push_back(mov);
                        }
                    }
                    else
                    {
                        if (valor < mejorValor)
                        {
                            mejorValor = valor;
                            mejoresMovimientos.clear();
                            mejoresMovimientos.push_back(mov);
                        }
                        else if (valor == mejorValor)
                        {
                            mejoresMovimientos.push_back(mov);
                        }
                    }
                }
            }
        }
    }

    if (!mejoresMovimientos.empty())
    {
        return mejoresMovimientos[rand() % mejoresMovimientos.size()];
    }

    return movimientoReactivo(esBlancas); // Fallback a reactivo si no hay movimientos
}

// ---------- Movimiento del Jugador Humano ----------
Movimiento movimientoHumano(bool esBlancas)
{
    mostrarJugadasPosibles(tablero, esBlancas);

    if (movimientosAgrupados.empty())
    {
        return {"", "", "", false, 0};
    }

    cout << "Elige el ID de la pieza: ";
    int id;
    cin >> id;

    if (movimientosAgrupados.find(id) == movimientosAgrupados.end())
    {
        cout << "ID no válido.\n";
        return movimientoHumano(esBlancas);
    }

    auto &movs = movimientosAgrupados[id];
    cout << "Has elegido la pieza " << movs[0].pieza << " en " << movs[0].origen << endl;

    if (movs.size() == 1)
    {
        return movs[0];
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
            cout << "Destino no válido.\n";
            return movimientoHumano(esBlancas);
        }
        return movs[opcion - 1];
    }
}

// ---------- Selección de tipo de jugador ----------
void seleccionarJugadores()
{
    cout << "=== SELECCIÓN DE JUGADORES ===\n";
    cout << "Tipos disponibles:\n";
    cout << "0 - HUMANO\n";
    cout << "1 - AGENTE REACTIVO\n";
    cout << "2 - AGENTE MINMAX\n\n";

    int eleccion;

    cout << "Selecciona el tipo para las BLANCAS (0-2): ";
    cin >> eleccion;
    jugadorBlancas = static_cast<TipoJugador>(eleccion);

    cout << "Selecciona el tipo para las NEGRAS (0-2): ";
    cin >> eleccion;
    jugadorNegras = static_cast<TipoJugador>(eleccion);

    cout << "\nConfiguración:\n";
    cout << "Blancas: ";
    switch (jugadorBlancas)
    {
    case HUMANO:
        cout << "HUMANO";
        break;
    case REACTIVO:
        cout << "AGENTE REACTIVO";
        break;
    case MINMAX:
        cout << "AGENTE MINMAX";
        break;
    }
    cout << "\nNegras: ";
    switch (jugadorNegras)
    {
    case HUMANO:
        cout << "HUMANO";
        break;
    case REACTIVO:
        cout << "AGENTE REACTIVO";
        break;
    case MINMAX:
        cout << "AGENTE MINMAX";
        break;
    }
    cout << "\n\n";
}

// ---------- Main ----------
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    srand(static_cast<unsigned int>(time(nullptr)));

    // Seleccionar tipos de jugador
    seleccionarJugadores();

    inicializarAmbiente();
    int turno = 0;

    cout << "=== AJEDREZ INTELIGENTE ===" << endl;

    while (true)
    {
        bool turnoBlancas = (turno % 2 == 0);
        TipoJugador jugadorActual = turnoBlancas ? jugadorBlancas : jugadorNegras;

        cout << "\n=== Turno " << (turno + 1) << " - " << (turnoBlancas ? "BLANCAS" : "NEGRAS") << " ===" << endl;

        // Mostrar tipo de jugador
        cout << "Jugador: ";
        switch (jugadorActual)
        {
        case HUMANO:
            cout << "HUMANO";
            break;
        case REACTIVO:
            cout << "AGENTE REACTIVO";
            break;
        case MINMAX:
            cout << "AGENTE MINMAX";
            break;
        }
        cout << endl;

        mostrarAmbienteBoard(tablero);

        // Verificar jaque y jaque mate
        bool jaqueActual = estaEnJaque(tablero, !turnoBlancas);
        if (jaqueActual)
        {
            cout << "!!! ¡JAQUE AL REY " << (!turnoBlancas ? "BLANCO" : "NEGRO") << " !!!" << endl;

            if (esJaqueMate(tablero, !turnoBlancas))
            {
                cout << "\n*** ¡JAQUE MATE! ***" << endl;
                cout << "Las " << (turnoBlancas ? "NEGRAS" : "BLANCAS") << " ganan!" << endl;
                mostrarAmbienteBoard(tablero);
                break;
            }
        }

        // Obtener movimiento según el tipo de jugador
        Movimiento movimiento;

        switch (jugadorActual)
        {
        case HUMANO:
            movimiento = movimientoHumano(turnoBlancas);
            break;
        case REACTIVO:
            movimiento = movimientoReactivo(turnoBlancas);
            break;
        case MINMAX:
            cout << "Pensando... (esto puede tomar unos segundos)" << endl;
            movimiento = movimientoMinMax(turnoBlancas);
            break;
        }

        if (movimiento.pieza == "")
        {
            cout << "No hay movimientos posibles para " << (turnoBlancas ? "blancas" : "negras") << endl;
            cout << "Fin del juego - Empate" << endl;
            break;
        }

        // Aplicar movimiento
        string piezaCapturada = tablero[8 - (movimiento.destino[1] - '0')][movimiento.destino[0] - 'A'];
        aplicarMovimiento(tablero, movimiento);

        // Mostrar información del movimiento
        cout << "Movimiento: " << movimiento.pieza
             << " de " << movimiento.origen
             << " a " << movimiento.destino;

        if (piezaCapturada != " ")
        {
            cout << " - CAPTURA " << piezaCapturada;
        }
        cout << endl;

        // Pequeña pausa de 3s para mejor visualización
#ifdef _WIN32
        Sleep(3000);
#else
        sleep(1);
#endif

        turno++;

        // Límite de turnos
        if (turno > 100)
        {
            cout << "\nLímite de turnos alcanzado. Juego terminado." << endl;
            break;
        }
    }

    cout << "\n=== FIN DEL JUEGO ===" << endl;
    return 0;
}