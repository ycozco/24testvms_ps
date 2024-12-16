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

// Función para calcular el valor C_max
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

// Función para intercambiar dos elementos en un vector
void intercambiar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Función recursiva para generar permutaciones manualmente
void generar_permutaciones(vector<int>& secuencia, int inicio, vector<int>& mejor_secuencia,
                           int& mejor_cmax, const vector<vector<int>>& matriz) {
    if (inicio == (int)secuencia.size()) {
        int val = c_max(secuencia, matriz);
        if (val < mejor_cmax) {
            mejor_cmax = val;
            mejor_secuencia = secuencia;
        }
        return;
    }
    for (int i = inicio; i < (int)secuencia.size(); i++) {
        intercambiar(secuencia[inicio], secuencia[i]);
        generar_permutaciones(secuencia, inicio + 1, mejor_secuencia, mejor_cmax, matriz);
        intercambiar(secuencia[inicio], secuencia[i]); // Deshacer el cambio
    }
}

// Función Brute Force: Encuentra la mejor secuencia
pair<vector<int>, int> brute_force(const vector<vector<int>>& matriz) {
    vector<int> secuencia_inicial(matriz.size());
    for (int i = 0; i < (int)matriz.size(); i++) {
        secuencia_inicial[i] = i;
    }
    vector<int> mejor_secuencia = secuencia_inicial;
    int mejor_cmax = c_max(secuencia_inicial, matriz);

    // Generación manual de permutaciones
    generar_permutaciones(secuencia_inicial, 0, mejor_secuencia, mejor_cmax, matriz);

    return {mejor_secuencia, mejor_cmax};
}

// Función para leer la matriz desde un archivo
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

// Función para obtener el tiempo actual
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

    sort(archivos_procesados.begin(), archivos_procesados.end());

    for (const auto& archivo : archivos_procesados) {
        cout << "Procesando archivo: " << archivo << endl;
        vector<vector<int>> matriz = leer_matriz_desde_archivo(archivo);

        string hora_inicio = obtener_tiempo_actual();
        auto start_time = chrono::high_resolution_clock::now();

        // Llamada a la función Brute Force
        auto [mejor_secuencia, mejor_cmax] = brute_force(matriz);

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