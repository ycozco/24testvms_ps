#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

int maximo(int a, int b) {
    return (a > b) ? a : b;
}

void intercambiar(string& a, string& b) {
    string temp = a;
    a = b;
    b = temp;
}

void ordenar_archivos(vector<string>& archivos) {
    int n = (int)archivos.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (archivos[j] > archivos[j + 1]) {
                intercambiar(archivos[j], archivos[j + 1]);
            }
        }
    }
}

int c_max(const vector<int>& seq, const vector<vector<int>>& p) {
    int n = (int)seq.size();
    int m = (int)p[0].size();
    vector<vector<int>> f(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            f[i][j] = maximo(f[i - 1][j], f[i][j - 1]) + p[seq[i - 1]][j - 1];
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
    for (int i = 0; i < num_trabajos; i++) {
        for (int j = 0; j < num_maquinas; j++) {
            archivo >> matriz[i][j];
        }
    }
    archivo.close();
    return matriz;
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
    ofstream salida_csv("resultados_localSearch.csv");
    salida_csv << "Archivo,Hora Inicio,Hora Fin,Duracion (s),C_max,Secuencia\n";
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
        vector<int> secuencia_inicial(matriz.size());
        for (int i = 0; i < (int)matriz.size(); i++) {
            secuencia_inicial[i] = i;
        }
        string hora_inicio = obtener_tiempo_actual();
        auto start_time = chrono::high_resolution_clock::now();
        vector<int> mejor_secuencia = secuencia_inicial;
        int mejor_cmax = c_max(secuencia_inicial, matriz);
        sort(secuencia_inicial.begin(), secuencia_inicial.end());
        do {
            int val = c_max(secuencia_inicial, matriz);
            if (val < mejor_cmax) {
                mejor_cmax = val;
                mejor_secuencia = secuencia_inicial;
            }
        } while (next_permutation(secuencia_inicial.begin(), secuencia_inicial.end()));
        auto end_time = chrono::high_resolution_clock::now();
        string hora_fin = obtener_tiempo_actual();
        chrono::duration<double> duration = end_time - start_time;
        cout << "Secuencia generada: ";
        for (auto x : mejor_secuencia) cout << x << " ";
        cout << "\nC_max generado: " << mejor_cmax << endl;
        cout << "Hora inicio: " << hora_inicio << ", Hora fin: " << hora_fin
             << ", Duracion: " << duration.count() << " segundos\n";
        salida_csv << archivo << "," << hora_inicio << "," << hora_fin << ","
                   << duration.count() << "," << mejor_cmax << ",";
        for (auto x : mejor_secuencia) salida_csv << x << " ";
        salida_csv << "\n";
    }
    salida_csv.close();
    cout << "Resultados guardados en 'resultados_localSearch.csv'.\n";
    return 0;
}