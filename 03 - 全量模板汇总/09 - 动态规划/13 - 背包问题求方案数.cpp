#include <bits/stdc++.h>
using namespace std;
#define LL long long
const int mod = 1e9 + 7, N = 1010;
LL n, W, cnt[N], f[N], w, v;
int main(){
    cin >> n >> W;
    for (int i = 0; i <= W; i ++ )
        cnt[i] = 1;
    for (int i = 0; i < n; i ++ ){
        cin >> w >> v;
        for (int j = W; j >= w; j -- )
            if (f[j] < f[j - w] + v){
                f[j] = f[j - w] + v;
                cnt[j] = cnt[j - w];
            }
            else if (f[j] == f[j - w] + v){
                cnt[j] = (cnt[j] + cnt[j - w]) % mod;
            }
    }
    cout << cnt[W] << "\n";
    return 0;
}
