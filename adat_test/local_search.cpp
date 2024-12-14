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

vector<int> random_order(const vector<int>& seq) {
    vector<int> s = seq;
    rotate(s.begin(), s.begin() + 1, s.end());
    return s;
}

vector<int> LOCAL_SEARCH(const vector<int>& pi, const vector<vector<int>>& p) {
    vector<int> pi_cur = pi;
    vector<int> pi_r = random_order(pi_cur);
    int k = 0;
    int c = 0;
    int best = c_max(pi_cur, p);
    int n = (int)pi_cur.size();
    while (c < n) {
        int x = pi_r[k];
        vector<int> new_seq;
        for (auto j : pi_cur) if (j != x) new_seq.push_back(j);
        int k_pos = best_insertion_position(new_seq, x, p);
        new_seq.insert(new_seq.begin() + k_pos, x);
        pi_cur = new_seq;
        int val = c_max(pi_cur, p);
        if (val < best) {
            best = val;
            c = 0;
        } else {
            c++;
        }
        k = (k + 1) % n;
    }
    return pi_cur;
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
    ofstream salida_csv("resultados_localSearch.csv");

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

        vector<int> secuencia_inicial(matriz.size());
        for (int i = 0; i < (int)matriz.size(); i++) {
            secuencia_inicial[i] = i;
        }

        string hora_inicio = obtener_tiempo_actual();
        auto start_time = chrono::high_resolution_clock::now();

        vector<int> secuencia_generada = LOCAL_SEARCH(secuencia_inicial, matriz);
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
    cout << "Resultados guardados en 'resultados_localSearch.csv'.\n";

    return 0;
}
//exec: g++ -std=c++17 -o localSearch local_search.cpp
// exec : ./localSearch