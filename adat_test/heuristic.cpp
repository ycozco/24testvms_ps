#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

// Función para calcular el C_max de una secuencia y matriz de tiempos
int c_max(const vector<int>& seq, const vector<vector<int>>& p) {
    int n = (int)seq.size();
    int m = (int)p[0].size();
    vector<vector<int>> f(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            f[i][j] = max(f[i - 1][j], f[i][j - 1]) + p[seq[i - 1]][j - 1];
        }
    }
    return f[n][m];
}

// Función para leer la matriz de tiempos desde un archivo
vector<vector<int>> leer_matriz_desde_archivo(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    int num_trabajos, num_maquinas;
    archivo >> num_trabajos >> num_maquinas;

    vector<vector<int>> matriz(num_trabajos, vector<int>(num_maquinas));

    for (int i = 0; i < num_trabajos; ++i) {
        for (int j = 0; j < num_maquinas; ++j) {
            archivo >> matriz[i][j];
        }
    }

    archivo.close();
    return matriz;
}

// Función para ordenar tareas por suma de tiempos usando Bubble Sort
vector<int> ordenar_por_suma(const vector<pair<int, int>>& tareas) {
    vector<pair<int, int>> copia = tareas;
    int n = copia.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (copia[j].first < copia[j + 1].first) { // Orden descendente
                swap(copia[j], copia[j + 1]);
            }
        }
    }

    // Extraer los índices ordenados
    vector<int> orden;
    for (const auto& tarea : copia) {
        orden.push_back(tarea.second);
    }
    return orden;
}

// Determinar la mejor posición para insertar un trabajo en la secuencia
int best_insertion_position(const vector<int>& seq, int job, const vector<vector<int>>& p) {
    int best = -1;
    int val = -1;
    for (int i = 0; i <= (int)seq.size(); i++) {
        vector<int> test = seq;
        test.insert(test.begin() + i, job);
        int tmp = c_max(test, p);
        if (best == -1 || tmp < val) {
            val = tmp;
            best = i;
        }
    }
    return best;
}

// Algoritmo NEH
vector<int> NEH_HEURISTIC(const vector<vector<int>>& p) {
    vector<pair<int, int>> tareas;
    for (int i = 0; i < (int)p.size(); i++) {
        int sum_ = 0;
        for (auto x : p[i]) sum_ += x;
        tareas.push_back({sum_, i});
    }

    vector<int> po = ordenar_por_suma(tareas);

    vector<int> seq;
    seq.push_back(po[0]);

    for (int k = 1; k < (int)po.size(); k++) {
        int pos = best_insertion_position(seq, po[k], p);
        seq.insert(seq.begin() + pos, po[k]);
    }

    return seq;
}

// Ordenar nombres de archivos alfabéticamente usando Bubble Sort
void ordenar_archivos(vector<string>& archivos) {
    int n = archivos.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (archivos[j] > archivos[j + 1]) { // Orden ascendente
                swap(archivos[j], archivos[j + 1]);
            }
        }
    }
}

// Función principal para procesar múltiples archivos
int main() {
    string directorio = "."; // Cambia si los archivos están en otro directorio
    vector<string> archivos_procesados;

    for (const auto& entrada : fs::directory_iterator(directorio)) {
        string archivo = entrada.path().filename().string();
        // Filtrar solo archivos con prefijo "ta"
        if (archivo.find("ta") == 0) {
            archivos_procesados.push_back(archivo);
        }
    }

    // Ordenar los archivos alfabéticamente
    ordenar_archivos(archivos_procesados);

    // Procesar cada archivo
    for (const auto& archivo : archivos_procesados) {
        cout << "Procesando archivo: " << archivo << endl;

        // Leer matriz
        vector<vector<int>> matriz = leer_matriz_desde_archivo(archivo);

        // Ejecutar algoritmo NEH
        vector<int> secuencia_generada = NEH_HEURISTIC(matriz);
        int cmax_generado = c_max(secuencia_generada, matriz);

        // Imprimir resultados
        cout << "Secuencia generada: ";
        for (auto x : secuencia_generada) cout << x << " ";
        cout << "\nC_max generado: " << cmax_generado << "\n" << endl;
    }

    return 0;
}
