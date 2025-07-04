#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<long long int>> vvi;

vvi read_matrix_input(int mode) {
    vvi a;
    int n, m;
    if (mode == 0) {
        cin >> n >> m;
        a.assign(n, vector<long long int>(m));
        for (auto &i: a) {
            for (auto &j: i) {
                cin >> j;
            }
        }
    }
    else {
        string fname; cin >> fname;
        ifstream f(fname);
        f >> n >> m;
        a.assign(n, vector<long long int>(m));
        for (auto &i: a) {
            for (auto &j: i) {
                f >> j;
            }
        }
        f.close();
    }
    return a;
}

void print_matrix(vvi &a, const string& fname, int mode) {
    if (mode) {
        ofstream f(fname);
        f << a.size() << " " << a[0].size() << endl;
        for (auto &i: a) {
            for (auto &j: i) {
                f << j << " ";
            }
            f << endl;
        }
        f.close();
    }
    else {
        cout << a.size() << " " << a[0].size() << endl;
        for (auto &i: a) {
            for (auto &j: i) {
                cout << j << " ";
            }
            cout << endl;
        }
    }
}

long long determinant(vvi &a) {
    if (a.size() == 1) return a[0][0];
    long long ret = 0;
    for (int i = 0; i < a.size(); i++) {
        vvi b;
        for (int j = 1; j < a.size(); j++) {
            vector<long long int> row;
            for (int k = 0; k < a.size(); k++) {
                if (k != i) row.push_back(a[j][k]);
            }
            b.push_back(row);
        }
        ret += (i % 2 ? -1 : 1) * a[0][i] * determinant(b);
    }
    return ret;
}

int main() {
    const string history_file = "good_history";
    int tc; cin >> tc;
    while (tc--) {
        ofstream history(history_file, ios::app);
        string query; cin >> query;

        int mode;
        if (query != "history") {
            cin >> mode;
            history << "LOG::" << query << " " << mode << endl;
        }
        else {
            history << "LOG::" << query << endl;
        }
        history.close();

        if (query == "history") {
            ifstream history_rd(history_file);
            string line;
            while (getline(history_rd, line)) {
                cout << line << endl;
            }
            history_rd.close();
            continue;
        }

        vvi c;
        int err = 0;
        if (query == "add_matrix") {
            vvi a(read_matrix_input(mode));
            vvi b(read_matrix_input(mode));
            if (a.size() != b.size() || a[0].size() != b[0].size()) {
                err = 1;
            }
            else {
                c.assign(a.size(), vector<long long int>(a[0].size()));
                for (int i = 0; i < a.size(); i++) {
                    for (int j = 0; j < a[0].size(); j++) {
                        c[i][j] = a[i][j] + b[i][j];
                    }
                }
            }
        }
        else if (query == "mult_matrix") {
            vvi a(read_matrix_input(mode));
            vvi b(read_matrix_input(mode));
            if (a[0].size() != b.size()) {
                err = 1;
            }
            else {
                c.assign(a.size(), vector<long long int>(b[0].size(), 0));
                for (int i = 0; i < a.size(); i++) {
                    for (int j = 0; j < b[0].size(); j++) {
                        for (int k = 0; k < a[0].size(); k++) {
                            c[i][j] += a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
        else if (query == "scalar_mult_matrix") {
            long long int s; cin >> s;
            vvi a(read_matrix_input(mode));
            c.assign(a.size(), vector<long long int>(a[0].size()));
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < a[0].size(); j++) {
                    c[i][j] = s * a[i][j];
                }
            }
        }
        else if (query == "transpose_matrix") {
            vvi a(read_matrix_input(mode));
            c.assign(a[0].size(), vector<long long int>(a.size()));
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < a[0].size(); j++) {
                    c[j][i] = a[i][j];
                }
            }
        }
        else if (query == "determinant") {
             vvi a(read_matrix_input(mode));
             if (a.size() != a[0].size()) {
                 cout << "ERROR: INVALID ARGUMENT\n";
             }
             else
                 cout << determinant(a) << endl;
             continue;
        }
        else {
            cout << "ERROR: INVALID COMMAND: " << query << "\n";
            exit(1);
        }

        string output;
        if (mode)
            cin >> output;
        if (err) {
            cout << "ERROR: INVALID ARGUMENT\n";
        }
        else
             print_matrix(c, output, mode);
    }
}
