#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

//-----------------------------------------
// Funciones Auxiliares
//-----------------------------------------

// Calcula el valor C_max para una secuencia dada y su matriz de tiempos.
int calcular_cmax(const vector<int>& secuencia, const vector<vector<int>>& p) {
    int n = (int)secuencia.size();
    int m = (int)p[0].size();

    // f[i][j] representará el tiempo mínimo al terminar el trabajo i en máquina j
    vector<vector<int>> f(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            f[i][j] = max(f[i - 1][j], f[i][j - 1]) + p[secuencia[i - 1]][j - 1];
        }
    }
    return f[n][m];
}

// Intercambia dos elementos en el vector (función auxiliar para generar_permutaciones)
inline void intercambiar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Genera todas las permutaciones posibles de la secuencia y actualiza el mejor C_max.
// Esta función se implementa recursivamente.
void generar_permutaciones(vector<int>& secuencia,
                           int inicio,
                           vector<int>& mejor_secuencia,
                           int& mejor_cmax,
                           const vector<vector<int>>& matriz) {
    if (inicio == (int)secuencia.size()) {
        int val = calcular_cmax(secuencia, matriz);
        if (val < mejor_cmax) {
            mejor_cmax = val;
            mejor_secuencia = secuencia;
        }
        return;
    }

    for (int i = inicio; i < (int)secuencia.size(); i++) {
        intercambiar(secuencia[inicio], secuencia[i]);
        generar_permutaciones(secuencia, inicio + 1, mejor_secuencia, mejor_cmax, matriz);
        intercambiar(secuencia[inicio], secuencia[i]); // revertir intercambio
    }
}

// Método de fuerza bruta para encontrar la mejor secuencia y el C_max mínimo.
pair<vector<int>, int> brute_force(const vector<vector<int>>& matriz) {
    vector<int> secuencia_inicial(matriz.size());
    for (int i = 0; i < (int)matriz.size(); i++) {
        secuencia_inicial[i] = i;
    }

    vector<int> mejor_secuencia = secuencia_inicial;
    int mejor_cmax = calcular_cmax(secuencia_inicial, matriz);

    // Generar manualmente todas las permutaciones
    generar_permutaciones(secuencia_inicial, 0, mejor_secuencia, mejor_cmax, matriz);

    return {mejor_secuencia, mejor_cmax};
}

// Lee la matriz desde un archivo dado su nombre.
vector<vector<int>> leer_matriz_desde_archivo(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    int num_trabajos, num_maquinas;
    archivo >> num_trabajos >> num_maquinas;

    vector<vector<int>> matriz(num_trabajos, vector<int>(num_maquinas));
    for (int i = 0; i < num_trabajos; i++) {
        for (int j = 0; j < num_maquinas; j++) {
            archivo >> matriz[i][j];
        }
    }
    archivo.close();
    return matriz;
}

// Retorna la hora actual en formato de cadena.
string obtener_tiempo_actual() {
    auto now = chrono::system_clock::now();
    time_t tiempo_actual = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&tiempo_actual), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Procesa un archivo y calcula la mejor secuencia y C_max, retornando la información.
struct Resultado {
    string archivo;
    string hora_inicio;
    string hora_fin;
    double duracion;
    int cmax;
    vector<int> secuencia;
};

Resultado procesar_archivo(const string& archivo) {
    cout << "Procesando archivo: " << archivo << endl;
    vector<vector<int>> matriz = leer_matriz_desde_archivo(archivo);

    string hora_inicio = obtener_tiempo_actual();
    auto start_time = chrono::high_resolution_clock::now();

    // Fuerza bruta para encontrar la mejor secuencia
    auto [mejor_secuencia, mejor_cmax] = brute_force(matriz);

    auto end_time = chrono::high_resolution_clock::now();
    string hora_fin = obtener_tiempo_actual();

    chrono::duration<double> duration = end_time - start_time;

    cout << "Secuencia generada: ";
    for (auto x : mejor_secuencia) cout << x << " ";
    cout << "\nC_max generado: " << mejor_cmax << endl;
    cout << "Hora inicio: " << hora_inicio << ", Hora fin: " << hora_fin
         << ", Duracion: " << duration.count() << " segundos\n";

    Resultado r;
    r.archivo = archivo;
    r.hora_inicio = hora_inicio;
    r.hora_fin = hora_fin;
    r.duracion = duration.count();
    r.cmax = mejor_cmax;
    r.secuencia = mejor_secuencia;
    return r;
}

// Escribe los resultados en un archivo CSV.
void escribir_resultados_csv(const string& nombre_csv, const vector<Resultado>& resultados) {
    ofstream salida_csv(nombre_csv);
    if (!salida_csv) {
        cerr << "No se pudo abrir el archivo de resultados: " << nombre_csv << endl;
        return;
    }

    salida_csv << "Archivo,Hora Inicio,Hora Fin,Duracion (s),C_max,Secuencia\n";
    for (const auto& res : resultados) {
        salida_csv << res.archivo << "," << res.hora_inicio << "," << res.hora_fin << ","
                   << res.duracion << "," << res.cmax << ",";
        for (auto x : res.secuencia) salida_csv << x << " ";
        salida_csv << "\n";
    }
    salida_csv.close();
    cout << "Resultados guardados en '" << nombre_csv << "'.\n";
}

//-----------------------------------------
// Función principal
//-----------------------------------------
int main() {
    string directorio = ".";
    vector<string> archivos_procesados;

    // Filtrar archivos que inicien con "ta"
    for (const auto& entrada : fs::directory_iterator(directorio)) {
        string archivo = entrada.path().filename().string();
        if (archivo.find("ta") == 0) {
            archivos_procesados.push_back(archivo);
        }
    }

    // Ordenar la lista de archivos a procesar
    sort(archivos_procesados.begin(), archivos_procesados.end());

    // Procesar cada archivo y almacenar resultados
    vector<Resultado> resultados;
    for (const auto& archivo : archivos_procesados) {
        Resultado r = procesar_archivo(archivo);
        resultados.push_back(r);
    }

    // Guardar resultados en CSV
    escribir_resultados_csv("resultados_localSearch.csv", resultados);

    return 0;
}