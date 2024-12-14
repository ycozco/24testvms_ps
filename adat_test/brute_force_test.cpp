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

vector<int> BRUTE_FORCE() {
    vector<vector<int>> p = {{3, 2}, {1, 4}, {2, 3}};
    int n = (int)p.size();
    vector<int> best_seq(n), seq(n);
    for (int i = 0; i < n; i++) {
        best_seq[i] = i;
        seq[i] = i;
    }
    int best_val = c_max(best_seq, p);
    
    cout << "Initial sequence: ";
    for (auto x : seq) cout << x << " ";
    cout << "\nC_max: " << best_val << "\n\n";

    while (next_permutation(seq.begin(), seq.end())) {
        int val = c_max(seq, p);

        // Print current permutation and its C_max
        cout << "Current sequence: ";
        for (auto x : seq) cout << x << " ";
        cout << "\nC_max: " << val << "\n";

        if (val < best_val) {
            best_val = val;
            best_seq = seq;
            cout << "New best sequence found!\n";
        }
        cout << "\n";
    }

    cout << "Best sequence: ";
    for (auto x : best_seq) cout << x << " ";
    cout << "\nFinal C_max: " << best_val << "\n";

    return best_seq;
}

int main() {
    vector<int> s = BRUTE_FORCE();
    return 0;
}
