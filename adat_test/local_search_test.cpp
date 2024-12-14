#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

vector<int> LOCAL_SEARCH(const vector<int>& pi) {
    vector<vector<int>> p = {{3, 2}, {1, 4}, {2, 3}}; // Matriz de tiempos
    vector<int> pi_cur = pi;
    vector<int> pi_r = random_order(pi_cur);
    int k = 0;
    int c = 0;
    int best = c_max(pi_cur, p);
    int n = (int)pi_cur.size();

    cout << "Initial sequence: ";
    for (auto x : pi_cur) cout << x << " ";
    cout << "\nInitial C_max: " << best << "\n\n";

    while (c < n) {
        int x = pi_r[k]; // Tarea seleccionada
        cout << "Task selected (x): " << x << "\n";

        // Eliminar tarea x de la secuencia actual
        vector<int> new_seq;
        for (auto j : pi_cur)
            if (j != x) new_seq.push_back(j);

        // Determinar mejor posición para insertar x
        int k_pos = best_insertion_position(new_seq, x, p);
        cout << "Best position to insert x: " << k_pos << "\n";

        // Insertar tarea en la mejor posición
        new_seq.insert(new_seq.begin() + k_pos, x);
        cout << "New sequence after insertion: ";
        for (auto j : new_seq) cout << j << " ";
        cout << "\n";

        // Actualizar la secuencia actual
        pi_cur = new_seq;
        int val = c_max(pi_cur, p);
        cout << "New C_max: " << val << "\n";

        // Verificar si hay mejora
        if (val < best) {
            best = val;
            c = 0; // Reiniciar contador de iteraciones sin mejora
            cout << "Improvement found! Updated best C_max: " << best << "\n\n";
        } else {
            c++;
            cout << "No improvement. Iteration without improvement: " << c << "\n\n";
        }

        // Actualizar el índice para recorrer pi_r
        k = (k + 1) % n;
    }

    cout << "Final sequence: ";
    for (auto x : pi_cur) cout << x << " ";
    cout << "\nFinal C_max: " << best << "\n";

    return pi_cur;
}

int main() {
    vector<int> seq = {0, 1, 2}; // Secuencia inicial
    vector<int> s = LOCAL_SEARCH(seq);
    return 0;
}
