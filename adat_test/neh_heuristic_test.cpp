#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;

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

vector<int> priority_order(const vector<vector<int>>& p) {
    vector<pair<int, int>> s;
    for (int i = 0; i < (int)p.size(); i++) {
        int sum_ = 0;
        for (auto x : p[i]) sum_ += x;
        s.push_back({sum_, i});
    }
    sort(s.begin(), s.end(), [](auto &a, auto &b) { return a.first > b.first; });
    vector<int> o;
    for (auto &x : s) o.push_back(x.second);
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

int main() {
    string directorio = ".";
    ofstream salida_csv("resultados_NEH.csv");
    salida_csv << "Archivo,C_max,Secuencia\n";

    for (const auto& entrada : fs::directory_iterator(directorio)) {
        string archivo = entrada.path().filename().string();
        if (archivo.find("ta") == 0) {
            cout << "Procesando archivo: " << archivo << endl;

            vector<vector<int>> matriz = leer_matriz(archivo);
            vector<int> secuencia = NEH_HEURISTIC(matriz);
            int cmax = c_max(secuencia, matriz);

            salida_csv << archivo << "," << cmax << ",";
            for (auto x : secuencia) salida_csv << x << " ";
            salida_csv << "\n";

            cout << "C_max: " << cmax << "\nSecuencia: ";
            for (auto x : secuencia) cout << x << " ";
            cout << "\n";
        }
    }

    salida_csv.close();
    cout << "Resultados guardados en 'resultados_NEH.csv'.\n";

    return 0;
}
//exec: g++ -std=c++17 -o neh_heuristic neh_heuristic.cpp
// exec: ./neh_heuristic