#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Calcula el C_max para una secuencia dada y una matriz de tiempos p
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

// Ordena las tareas en función de la suma total de sus tiempos de procesamiento
vector<int> priority_order(const vector<vector<int>>& p) {
    vector<pair<int, int>> s; // Vector de pares {suma, índice}
    for (int i = 0; i < (int)p.size(); i++) {
        int sum_ = 0;
        for (auto x : p[i]) sum_ += x;
        s.push_back({sum_, i}); // Suma total de la tarea y su índice
    }
    sort(s.begin(), s.end(), [](auto &a, auto &b) { return a.first > b.first; }); // Orden descendente
    vector<int> o;
    for (auto &x : s) o.push_back(x.second);
    return o;
}

// Determina la mejor posición para insertar una tarea en una secuencia
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

// Algoritmo heurístico NEH
vector<int> NEH_HEURISTIC() {
    vector<vector<int>> p = {{3, 2}, {1, 4}, {2, 3}}; // Matriz de tiempos
    vector<int> po = priority_order(p); // Obtener el orden de prioridad

    cout << "Priority order: ";
    for (auto x : po) cout << x << " ";
    cout << "\n";

    vector<int> seq;
    seq.push_back(po[0]); // Inicializa con la primera tarea
    cout << "Initial sequence: " << po[0] << "\n\n";

    for (int k = 1; k < (int)po.size(); k++) {
        cout << "Adding task: " << po[k] << "\n";

        // Encuentra la mejor posición para insertar la tarea
        int pos = best_insertion_position(seq, po[k], p);
        cout << "Best position to insert: " << pos << "\n";

        // Inserta la tarea en la mejor posición
        seq.insert(seq.begin() + pos, po[k]);
        cout << "Updated sequence: ";
        for (auto x : seq) cout << x << " ";
        cout << "\n";

        // Calcula y muestra el makespan actual
        int current_cmax = c_max(seq, p);
        cout << "Current C_max: " << current_cmax << "\n\n";
    }

    return seq;
}

int main() {
    vector<int> s = NEH_HEURISTIC();
    cout << "Final sequence: ";
    for (auto x : s) cout << x << " ";
    cout << "\n";
    return 0;
}
