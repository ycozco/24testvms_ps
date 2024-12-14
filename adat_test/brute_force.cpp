#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

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

vector<int> BRUTE_FORCE(const vector<vector<int>>& p) {
    int n = (int)p.size();
    vector<int> best_seq(n), seq(n);
    for (int i = 0; i < n; i++) { best_seq[i] = i; seq[i] = i; }
    int best_val = c_max(best_seq, p);
    while (next_permutation(seq.begin(), seq.end())) {
        int val = c_max(seq, p);
        if (val < best_val) {
            best_val = val;
            best_seq = seq;
        }
    }
    return best_seq;
}

void ordenar_archivos(vector<string>& archivos) {
    int n = archivos.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (archivos[j] > archivos[j + 1]) {
                swap(archivos[j], archivos[j + 1]);
            }
        }
    }
}

string obtener_tiempo_actual() {
    auto now = chrono::system_clock::now();
    time_t tiempo_actual = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&tiempo_actual), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int main() {
    string directorio = ".";
    vector<string> archivos_procesados;
    ofstream salida_csv("resultados_bruteForce.csv");

    salida_csv << "Archivo,Hora Inicio,Hora Fin,Duración (s),C_max,Secuencia\n";

    for (const auto& entrada : fs::directory_iterator(directorio)) {
        string archivo = entrada.path().filename().string();
        if (archivo.find("ta") == 0) {
            archivos_procesados.push_back(archivo);
        }
    }

    ordenar_archivos(archivos_procesados);

    for (const auto& archivo : archivos_procesados) {
        cout << "Procesando archivo: " << archivo << endl;

        vector<vector<int>> matriz = leer_matriz_desde_archivo(archivo);

        string hora_inicio = obtener_tiempo_actual();
        auto start_time = chrono::high_resolution_clock::now();

        vector<int> secuencia_generada = BRUTE_FORCE(matriz);
        int cmax_generado = c_max(secuencia_generada, matriz);

        auto end_time = chrono::high_resolution_clock::now();
        string hora_fin = obtener_tiempo_actual();
        chrono::duration<double> duration = end_time - start_time;

        cout << "Secuencia generada: ";
        for (auto x : secuencia_generada) cout << x << " ";
        cout << "\nC_max generado: " << cmax_generado << endl;
        cout << "Hora inicio: " << hora_inicio << ", Hora fin: " << hora_fin
             << ", Duración: " << duration.count() << " segundos\n";

        salida_csv << archivo << "," << hora_inicio << "," << hora_fin << ","
                   << duration.count() << "," << cmax_generado << ",";
        for (auto x : secuencia_generada) salida_csv << x << " ";
        salida_csv << "\n";
    }

    salida_csv.close();
    cout << "Resultados guardados en 'resultados_bruteForce.csv'.\n";

    return 0;
}
// exec : g++ -std=c++17 -o bruteForce bruteForce.cpp
// exec : ./bruteForce