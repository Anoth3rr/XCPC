#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    int n = 10000;
    if (argc >= 2)
        n = atoi(argv[1]);
    for (int _ = 1; _ <= n; ++_) {
        system("data.exe > data.in");
        system("std.exe < data.in > std.out");
        system("solve.exe < data.in > solve.out");
        if (system("fc std.out solve.out > diff.log")) {
            cout << "WA on test" << _ << "\n";
            break;
        }
        cout << "AC " << _ << "\n";
    }
    return 0;
}