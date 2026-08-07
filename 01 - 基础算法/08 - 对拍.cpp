#include <bits/stdc++.h>
using namespace std;

#define int long long
#define pii array<int, 2>
#define endl "\n"

signed main(signed ac, char* av[]) {
    int n = 10000;
    if (ac >= 2) n = atoi(av[1]);
    for (int _ = 1; _ <= n; ++_) {
        if (system("data.exe > data.in") != 0) {
            cerr << "data.exe failed on test " << _ << "\n";
            return 1;
        }
        if (system("std.exe < data.in > std.out") != 0) {
            cerr << "std.exe failed on test " << _ << "\n";
            return 1;
        }
        if (system("solve.exe < data.in > solve.out") != 0) {
            cerr << "solve.exe failed on test " << _ << "\n";
            return 1;
        }

        int dif = system("fc std.out solve.out > diff.log");
        if (dif == 1) {
            cout << "WA on test " << _ << "\n";
            break;
        }
        if (dif != 0) {
            cerr << "fc failed on test " << _ << "\n";
            return 1;
        }
        cout << "AC " << _ << "\n";
    }
    return 0;
}
