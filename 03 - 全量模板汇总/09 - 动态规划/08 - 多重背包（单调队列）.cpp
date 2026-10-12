#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
int n, W, w, v, s, f[N], g[N], q[N];
int main(){
    ios::sync_with_stdio(false);cin.tie(0);
    cin >> n >> W;
    for (int i = 0; i < n; i ++ ){
        memcpy ( g, f, sizeof f);
        cin >> w >> v >> s;
        for (int j = 0; j < w; j ++ ){
            int head = 0, tail = -1;
            for (int k = j; k <= W; k += w){
                if ( head <= tail && k - s * w > q[head] ) head ++ ;//保证队列长度 <= s 
                while ( head <= tail && g[q[tail]] - (q[tail] - j) / w * v <= g[k] - (k - j) / w * v ) tail -- ;//保证队列单调递减 
                q[ ++ tail] = k;
                f[k] = g[q[head]] + (k - q[head]) / w * v;
            }
        }
    }
    cout << f[W] << "\n";
    return 0;
}
