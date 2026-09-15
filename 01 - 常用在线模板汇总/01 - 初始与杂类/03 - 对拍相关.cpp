#include <bits/stdc++.h>
using namespace std;
signed main() {
    for (int case = 1; case <= 100000; ++case) {
        cout << "==============\n";
        system("data.exe > data.in");
        double start = clock();
        system("sol.exe < data.in > sol.out");
        double end = clock();
        system("std.exe < data.in > std.out");
        if (system("fc std.out sol.out")) {
            cout << "\033[31m" << "Wrong Answer\n" << "\033[0m";
            return 0;
        }
        cout<< "\033[32m" << "Accept\n"<< "\033[0m"; 
        cout << "测试点 #" << Case << "，用时"  << end - start << "ms\n";
    }
    return 0;
}