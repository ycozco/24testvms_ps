#include <iostream>
#include <vector>
#include <fstream>
#include <experimental/filesystem>
#include <chrono>
#include <iomanip>
#include <algorithm> // por si se necesita swap, max, etc.
#include <sstream>
#include <ctime>

using namespace std;
namespace fs = std::experimental::filesystem;

vector<vector<int>> leer_matriz(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    int num_trabajos, num_maquinas;
    archivo >> num_trabajos >> num_maquinas;

    vector<vector<int>> p(num_trabajos, vector<int>(num_maquinas));
    for (int i = 0; i < num_trabajos; ++i) {
        for (int j = 0; j < num_maquinas; ++j) {
            archivo >> p[i][j];
        }
    }

    archivo.close();
    return p;
}

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

void bubble_sort(vector<pair<int,int>>& s) {
    int n = (int)s.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (s[j].first < s[j + 1].first) {
                swap(s[j], s[j + 1]);
            }
        }
    }
}

vector<int> priority_order(const vector<vector<int>>& p) {
    vector<pair<int,int>> s;
    for (int i = 0; i < (int)p.size(); i++) {
        int sum_ = 0;
        for (auto x : p[i]) sum_ += x;
        s.push_back(make_pair(sum_, i));
    }
    bubble_sort(s);
    vector<int> o;
    for (size_t i = 0; i < s.size(); i++) {
        o.push_back(s[i].second);
    }
    return o;
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

vector<int> NEH_HEURISTIC(const vector<vector<int>>& p) {
    vector<int> po = priority_order(p);
    vector<int> seq;
    seq.push_back(po[0]);

    for (int k = 1; k < (int)po.size(); k++) {
        int pos = best_insertion_position(seq, po[k], p);
        seq.insert(seq.begin() + pos, po[k]);
    }

    return seq;
}

string obtener_hora_actual() {
    auto now = chrono::system_clock::now();
    time_t tiempo_actual = chrono::system_clock::to_time_t(now);
    stringstream ss;
    // put_time está disponible desde C++11, no hay problema en C++14
    ss << put_time(localtime(&tiempo_actual), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int main() {
    string directorio = ".";
    vector<string> archivos;

    ofstream salida_csv("resultados_NEH.csv");
    salida_csv << "Archivo,Hora Inicio,Hora Fin,Duracion (s),C_max,Secuencia\n";

    for (auto& entrada : fs::directory_iterator(directorio)) {
        string archivo = entrada.path().filename().string();
        if (archivo.find("ta") == 0) {
            archivos.push_back(archivo);
        }
    }

    // Ordenar los archivos manualmente (sin sort de C++11, aunque podría usarse std::sort)
    for (size_t i = 0; i < archivos.size(); ++i) {
        for (size_t j = i + 1; j < archivos.size(); ++j) {
            if (archivos[i] > archivos[j]) {
                swap(archivos[i], archivos[j]);
            }
        }
    }

    for (size_t idx = 0; idx < archivos.size(); ++idx) {
        const string& archivo = archivos[idx];
        vector<vector<int>> matriz = leer_matriz(archivo);

        string hora_inicio = obtener_hora_actual();
        auto start_time = chrono::high_resolution_clock::now();

        vector<int> secuencia = NEH_HEURISTIC(matriz);
        int cmax = c_max(secuencia, matriz);

        auto end_time = chrono::high_resolution_clock::now();
        string hora_fin = obtener_hora_actual();
        chrono::duration<double> duracion = end_time - start_time;

        salida_csv << archivo << "," << hora_inicio << "," << hora_fin << ","
                   << duracion.count() << "," << cmax << ",";

        cout << archivo << "," << hora_inicio << "," << hora_fin << ","
             << duracion.count() << "," << cmax << ",";

        for (size_t i = 0; i < secuencia.size(); i++) {
            salida_csv << secuencia[i] << " ";
            cout << secuencia[i] << " ";
        }
        salida_csv << "\n";
        cout << "\n";
    }

    salida_csv.close();
    return 0;
}
